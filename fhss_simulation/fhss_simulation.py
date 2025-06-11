import numpy as np
import matplotlib.pyplot as plt
import random

def awgn(signal, snr_db):
    snr = 10 ** (snr_db / 10)
    power_signal = np.mean(signal ** 2)
    noise_power = power_signal / snr
    noise = np.sqrt(noise_power) * np.random.randn(len(signal))
    return signal + noise

def rayleigh_fading(signal):
    fading = np.random.rayleigh(scale=1.0, size=len(signal))
    return signal * fading

def generate_jammer(t, jammer_type, frequencies, f_start, f_end, hop_rate, duration):
    if jammer_type == "fixed":
        jammer_freq = float(input("Jammer frequency (GHz): ")) * 1e9
        jammer_signal = 0.5 * np.cos(2 * np.pi * jammer_freq * t)
    elif jammer_type == "sweeping":
        sweep_start = float(input("Sweeping jammer start frequency (GHz): ")) * 1e9
        sweep_end = float(input("Sweeping jammer end frequency (GHz): ")) * 1e9
        jammer_freq_t = np.linspace(sweep_start, sweep_end, len(t))
        jammer_signal = 0.5 * np.cos(2 * np.pi * jammer_freq_t * t)
    elif jammer_type == "random":
        hop_times = int(duration * hop_rate)
        jammer_hops = [random.choice(frequencies) for _ in range(hop_times)]
        samples_per_hop = len(t) // hop_times
        jammer_signal = np.zeros_like(t)
        for i, freq in enumerate(jammer_hops):
            start = i * samples_per_hop
            end = (i + 1) * samples_per_hop
            time_slice = t[start:end]
            jammer_signal[start:end] = 0.5 * np.cos(2 * np.pi * freq * time_slice)
    else:
        jammer_signal = np.zeros_like(t)
    return jammer_signal

def ber_calculate(tx_bits, rx_bits):
    errors = np.sum(tx_bits != rx_bits)
    return errors / len(tx_bits)

def main():
    print("\n--- FHSS Simulation Parameter Input ---")

    bit_rate = float(input("Bit rate (bit/s): "))
    hop_rate = float(input("Hop rate (hops/s): "))
    num_channels = int(input("Number of channels: "))
    duration = float(input("Total simulation time (s): "))
    fs = float(input("Sampling frequency (Hz): "))

    print("\nEnter frequency band in GHz.")
    f_start = float(input("Start frequency (e.g. 2.4): ")) * 1e9
    f_end = float(input("End frequency (e.g. 2.5): ")) * 1e9
    frequencies = np.linspace(f_start, f_end, num_channels)

    print("\nSelect jammer type: fixed, sweeping, random, none")
    jammer_type = input("Jammer type: ").lower()

    snr_db = float(input("\nEnter SNR (dB): "))

    t = np.linspace(0, duration, int(fs * duration))

    hop_times = int(duration * hop_rate)
    hop_sequence = [random.choice(frequencies) for _ in range(hop_times)]
    samples_per_hop = len(t) // hop_times

    tx_bits = []
    signal = np.zeros_like(t)
    for i, freq in enumerate(hop_sequence):
        bit = random.choice([1, -1])
        tx_bits.append(bit)
        start = i * samples_per_hop
        end = (i + 1) * samples_per_hop
        time_slice = t[start:end]
        carrier = bit * np.cos(2 * np.pi * freq * time_slice)
        signal[start:end] = carrier

    tx_bits = np.array(tx_bits)

    jammer_signal = generate_jammer(t, jammer_type, frequencies, f_start, f_end, hop_rate, duration)

    faded_signal = rayleigh_fading(signal)

    combined_signal = faded_signal + jammer_signal

    noisy_signal = awgn(combined_signal, snr_db)

    rx_bits = []
    for i, freq in enumerate(hop_sequence):
        start = i * samples_per_hop
        end = (i + 1) * samples_per_hop
        time_slice = t[start:end]
        received_slice = noisy_signal[start:end]
        correlator = np.cos(2 * np.pi * freq * time_slice)
        demod = np.mean(received_slice * correlator)
        rx_bit = 1 if demod > 0 else -1
        rx_bits.append(rx_bit)

    rx_bits = np.array(rx_bits)

    ber = ber_calculate(tx_bits, rx_bits)

    print("\nFHSS simulation completed.")
    print(f"Used {num_channels} channels: {np.round(frequencies / 1e6, 3)} MHz")
    print(f"Jammer type: {jammer_type}")
    if jammer_type != "none":
        print(f"Jammer signal added.")
    print(f"SNR: {snr_db} dB")
    print(f"Bit Error Rate (BER): {ber:.6f}")

    plt.figure(figsize=(12, 6))
    plt.specgram(noisy_signal, Fs=fs, NFFT=256, noverlap=128, cmap='viridis')
    plt.title("FHSS Signal with Jammer and AWGN - Time-Frequency Spectrogram")
    plt.xlabel("Time (s)")
    plt.ylabel("Frequency (Hz)")
    plt.colorbar(label='Power Density (dB)')
    plt.tight_layout()
    plt.show()

    snr_range = np.arange(0, 21, 2)
    ber_values = []
    for snr_test in snr_range:
        noisy = awgn(faded_signal + jammer_signal, snr_test)
        rx_b = []
        for i, freq in enumerate(hop_sequence):
            start = i * samples_per_hop
            end = (i + 1) * samples_per_hop
            time_slice = t[start:end]
            received_slice = noisy[start:end]
            correlator = np.cos(2 * np.pi * freq * time_slice)
            demod = np.mean(received_slice * correlator)
            rx_bit = 1 if demod > 0 else -1
            rx_b.append(rx_bit)
        rx_b = np.array(rx_b)
        ber_tmp = ber_calculate(tx_bits, rx_b)
        if ber_tmp == 0:
            ber_tmp = 1e-6
        ber_values.append(ber_tmp)

    plt.figure()
    plt.plot(snr_range, ber_values, marker='o')
    plt.grid(True)
    plt.title("BER Performance vs SNR")
    plt.xlabel("SNR (dB)")
    plt.ylabel("Bit Error Rate (BER)")
    plt.yscale('log')
    plt.show()

if __name__ == "__main__":
    main()
