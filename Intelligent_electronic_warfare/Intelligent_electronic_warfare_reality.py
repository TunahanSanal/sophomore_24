import numpy as np
import matplotlib.pyplot as plt
from scipy.fft import fft
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report

fs = 10000
win_size = int(0.1 * fs)
t_full = np.arange(0, 1, 1/fs)

def generate_sample(has_target, fs, win_size):
    t = np.arange(0, win_size) / fs
    signal = np.zeros_like(t)

    if has_target:
        f = np.random.choice([1000, 1500, 2000, 2500])
        fading = np.random.rayleigh(scale=0.5, size=len(t))
        signal += np.sin(2 * np.pi * f * t) * fading

    noise = np.random.normal(0, 0.3, len(t))
    env = (
        0.2 * np.sin(2 * np.pi * 400 * t) +
        0.2 * np.sin(2 * np.pi * 2700 * t)
    )
    return signal + noise + env, has_target

X, y = [], []
for _ in range(1000):
    s, label = generate_sample(True, fs, win_size)
    X.append(np.abs(fft(s)[:win_size//2]))
    y.append(1)
for _ in range(1000):
    s, label = generate_sample(False, fs, win_size)
    X.append(np.abs(fft(s)[:win_size//2]))
    y.append(0)

X = np.array(X)
y = np.array(y)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2)
clf = RandomForestClassifier(n_estimators=100)
clf.fit(X_train, y_train)

print(classification_report(y_test, clf.predict(X_test)))

target_freqs = [1000, 2000, 3000]
target_signal = np.zeros_like(t_full)
segments_with_target = [2, 4, 7]

for i in segments_with_target:
    start = i * win_size
    end = start + win_size
    for f in target_freqs:
        fading = np.random.rayleigh(scale=0.5, size=win_size)
        target_signal[start:end] += np.sin(2 * np.pi * f * t_full[start:end]) * fading

env_noise = (
    0.2 * np.sin(2 * np.pi * 400 * t_full) +
    0.2 * np.sin(2 * np.pi * 2700 * t_full)
)
white_noise = np.random.normal(0, 0.3, len(t_full))
received_signal = target_signal + env_noise + white_noise

adaptive_jam = np.zeros_like(t_full)

for i in range(0, len(received_signal) - win_size, win_size):
    seg = received_signal[i:i+win_size]
    spectrum = np.abs(fft(seg)[:win_size//2])
    freqs = np.fft.fftfreq(win_size, 1/fs)[:win_size//2]
    detected = freqs[spectrum > np.max(spectrum)*0.7]

    for f in detected:
        adaptive_jam[i:i+win_size] += 2 * np.sin(2 * np.pi * f * t_full[i:i+win_size])

jammed_signal = received_signal + adaptive_jam

segments = [jammed_signal[i:i+win_size] for i in range(0, len(jammed_signal)-win_size, win_size)]
features = [np.abs(fft(seg)[:win_size//2]) for seg in segments]
predictions = clf.predict(features)

ground_truth = np.zeros(len(predictions))
for i in segments_with_target:
    ground_truth[i] = 1

def compute_snr(signal, noise):
    power_signal = np.mean(signal**2)
    power_noise = np.mean(noise**2)
    snr = 10 * np.log10(power_signal / power_noise)
    return snr

snr_list = []
for i in range(len(segments)):
    sig_segment = target_signal[i*win_size:(i+1)*win_size]
    noise_segment = jammed_signal[i*win_size:(i+1)*win_size] - sig_segment
    if np.any(sig_segment != 0):
        snr_val = compute_snr(sig_segment, noise_segment)
        snr_list.append(snr_val)

print(f"Average SNR (dB) (in target segments): {np.mean(snr_list):.2f}")

plt.figure(figsize=(14, 6))

plt.subplot(2,1,1)
plt.plot(t_full[:2000], jammed_signal[:2000])
plt.title("Jammed Signal (Adaptive) (First 0.2s)")

plt.subplot(2,1,2)
plt.step(range(len(predictions)), ground_truth, where='mid', label='Ground Truth', linewidth=2)
plt.step(range(len(predictions)), predictions, where='mid', linestyle='--', label='AI Prediction', linewidth=2)
plt.ylim(-0.1, 1.1)
plt.title("Target Signal Detection (Every 0.1 second)")
plt.xlabel("Time Window (0.1s)")
plt.ylabel("Signal Present/Absent")
plt.grid()
plt.legend()

plt.tight_layout()
plt.show()
