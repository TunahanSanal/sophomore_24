import numpy as np
import matplotlib.pyplot as plt

data_rate = 5000
hop_rate = 100
duration = 10
sampling_rate = 500000
snr_db_values = np.arange(0, 21, 2)

start_freq = 2.4e9
end_freq = 2.5e9
num_channels = 10
frequencies = np.linspace(start_freq, end_freq, num_channels)

num_bits = int(data_rate * duration)
total_samples = int(duration * sampling_rate)
samples_per_bit = int(sampling_rate / data_rate)
samples_per_hop = int(sampling_rate / hop_rate)
time = np.linspace(0, duration, total_samples, endpoint=False)

def generate_bits(n):
    return np.random.randint(0, 2, n)

def modulate(bits):
    return 2 * bits - 1

def generate_fhss_signal(data, hop_freqs):
    signal = np.zeros_like(time)
    for i, bit in enumerate(data):
        start = i * samples_per_bit
        end = start + samples_per_bit
        freq = hop_freqs[i // (samples_per_hop // samples_per_bit)]
        t_slice = time[start:end]
        signal[start:end] = bit * np.cos(2 * np.pi * freq * t_slice)
    return signal

def apply_rayleigh_fading(signal, samples_per_hop):
    faded = np.zeros_like(signal)
    num_hops = len(signal) // samples_per_hop
    for i in range(num_hops):
        fading = np.random.rayleigh(scale=0.3)
        faded[i * samples_per_hop:(i + 1) * samples_per_hop] = signal[i * samples_per_hop:(i + 1) * samples_per_hop] * fading
    return faded

def add_awgn(signal, snr_db):
    signal_power = np.mean(signal**2)
    snr_linear = 10**(snr_db / 10)
    noise_power = signal_power / snr_linear
    noise = np.random.normal(0, np.sqrt(noise_power), size=signal.shape)
    return signal + noise

def add_jammer(signal, frequencies):
    jammer_freq = np.random.choice(frequencies)
    jammer = 0.3 * np.sin(2 * np.pi * jammer_freq * time)
    return signal + jammer

def demodulate(received_signal, hop_freqs):
    received_bits = []
    for i in range(num_bits):
        start = i * samples_per_bit
        end = start + samples_per_bit
        freq = hop_freqs[i // (samples_per_hop // samples_per_bit)]
        t_slice = time[start:end]
        phase_offset = np.random.uniform(-np.pi/4, np.pi/4)
        correlator = np.cos(2 * np.pi * freq * t_slice + phase_offset)
        product = received_signal[start:end] * correlator
        integrated = np.sum(product)
        received_bits.append(1 if integrated > 0 else 0)
    return np.array(received_bits)

def calculate_ber(original, received):
    return np.sum(original != received) / len(original)

ber_values = []

for snr in snr_db_values:
    bits = generate_bits(num_bits)
    modulated = modulate(bits)
    hop_freqs = np.random.choice(frequencies, size=(num_bits // (samples_per_hop // samples_per_bit)) + 1)

    tx_signal = generate_fhss_signal(modulated, hop_freqs)
    tx_signal = apply_rayleigh_fading(tx_signal, samples_per_hop)
    tx_signal = add_jammer(tx_signal, frequencies)
    rx_signal = add_awgn(tx_signal, snr)

    demodulated_bits = demodulate(rx_signal, hop_freqs)
    ber = calculate_ber(bits, demodulated_bits)

    print(f"SNR: {snr} dB | BER: {ber:.8f}")
    ber_values.append(ber)

plt.figure(figsize=(8, 5))
plt.semilogy(snr_db_values, ber_values, marker='o')
plt.title("SNR vs BER (FHSS System)")
plt.xlabel("SNR (dB)")
plt.ylabel("BER")
plt.grid(True, which='both', linestyle='--')
plt.tight_layout()
plt.show()
