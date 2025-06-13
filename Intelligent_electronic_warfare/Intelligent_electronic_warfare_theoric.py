import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft, fftfreq

fs = 10000
t = np.arange(0, 1, 1/fs)
target_freqs = [1000, 2000, 3000]
noise_power = 0.5
jam_power = 5

target_signal = sum(np.sin(2 * np.pi * f * t) for f in target_freqs)

def detect_frequencies(signal, fs, threshold=0.3):
    N = len(signal)
    yf = np.abs(fft(signal)[:N//2])
    xf = fftfreq(N, 1/fs)[:N//2]
    norm_yf = yf / np.max(yf)
    detected_freqs = xf[norm_yf > threshold]
    return detected_freqs.astype(int), xf, norm_yf

def compute_snr(signal, noise):
    signal_power = np.mean(signal**2)
    noise_power = np.mean(noise**2)
    return 10 * np.log10(signal_power / noise_power)

soft_jam = np.random.normal(0, np.sqrt(noise_power), len(t))
soft_jammed_signal = target_signal + soft_jam

detected_freqs, xf, norm_yf = detect_frequencies(target_signal, fs)
print(f"Detected Frequencies: {detected_freqs}")

adaptive_jam = np.zeros_like(t)
for f in detected_freqs:
    adaptive_jam += np.sqrt(jam_power) * np.sin(2 * np.pi * f * t)

adaptive_jammed_signal = target_signal + adaptive_jam

snr_before = compute_snr(target_signal, soft_jam)
snr_after = compute_snr(target_signal, adaptive_jam)

print(f"SNR (Soft Jammed): {snr_before:.2f} dB")
print(f"SNR (Adaptive Hard Jammed): {snr_after:.2f} dB")

plt.figure(figsize=(14, 10))

plt.subplot(4, 1, 1)
plt.title("Original Target Signal")
plt.plot(t[:1000], target_signal[:1000])

plt.subplot(4, 1, 2)
plt.title("Soft Jammed Signal")
plt.plot(t[:1000], soft_jammed_signal[:1000])

plt.subplot(4, 1, 3)
plt.title("Adaptive Hard Jammed Signal")
plt.plot(t[:1000], adaptive_jammed_signal[:1000])

plt.subplot(4, 1, 4)
plt.title("FFT Spectrum")
plt.plot(xf, norm_yf)
plt.xlabel("Frequency (Hz)")
plt.ylabel("Normalized Amplitude")
plt.grid()

plt.tight_layout()
plt.show()
