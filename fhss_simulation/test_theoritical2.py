import numpy as np
import matplotlib.pyplot as plt

def generate_bits(n_bits):
    return np.random.randint(0, 2, n_bits)

def bpsk_modulate(bits):
    return 2 * bits - 1

def add_awgn_noise(signal, snr_db):
    signal_power = np.mean(np.abs(signal)**2)
    snr_linear = 10 ** (snr_db / 10)
    noise_power = signal_power / snr_linear
    noise = np.sqrt(noise_power / 2) * (np.random.randn(*signal.shape))
    return signal + noise

def bpsk_demodulate(signal):
    return (signal > 0).astype(int)

def calculate_ber(original_bits, received_bits):
    return np.mean(original_bits != received_bits)

bit_rate = 5000
sim_time = 1
n_bits = bit_rate * sim_time
snr_range = np.arange(0, 21, 2)

ber_list = []

for snr_db in snr_range:
    bits = generate_bits(n_bits)
    modulated = bpsk_modulate(bits)
    noisy = add_awgn_noise(modulated, snr_db)
    demodulated = bpsk_demodulate(noisy)
    ber = calculate_ber(bits, demodulated)
    ber_list.append(ber)
    print(f"SNR: {snr_db} dB | BER: {ber:.8f}")

plt.figure(figsize=(8,5))
plt.semilogy(snr_range, ber_list, 'o-', label="AWGN (no jammer, no fading)")
plt.xlabel("SNR (dB)")
plt.ylabel("Bit Error Rate (BER)")
plt.grid(True, which='both')
plt.title("BER vs SNR (AWGN Only)")
plt.legend()
plt.show()
