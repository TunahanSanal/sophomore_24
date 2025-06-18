import subprocess
import pandas as pd
from sklearn.ensemble import IsolationForest
import os

def run_octave_script():
    # Octave komutunu çalıştır
    # Eğer Windows kullanıyorsan 'octave-cli' veya 'octave' olabilir
    # Kendi sisteminde hangisi çalışıyorsa onu yaz
    try:
        result = subprocess.run(['octave', '--silent', 'new.m'], capture_output=True, text=True)
        if result.returncode != 0:
            print("Octave çalıştırılırken hata çıktı:")
            print(result.stderr)
            return False
        print("Octave script başarıyla çalıştırıldı.")
        return True
    except FileNotFoundError:
        print("Hata: Octave bulunamadı. Octave'ı yükleyip PATH değişkenine eklediğinden emin ol.")
        return False

def analyze_data():
    csv_file = 'isentropik_akis_verisi.csv'
    if not os.path.exists(csv_file):
        print(f"Hata: '{csv_file}' dosyası bulunamadı.")
        return

    df = pd.read_csv(csv_file, header=None, names=['time', 'pressure', 'temperature', 'velocity'])

    # Model için özellikler
    X = df[['pressure', 'temperature', 'velocity']].values

    model = IsolationForest(contamination=0.1, random_state=42)
    model.fit(X)

    df['anomaly'] = model.predict(X)

    warning_threshold = 5
    consecutive_count = 0

    for index, row in df.iterrows():
        if row['anomaly'] == -1:
            consecutive_count += 1
            if consecutive_count >= warning_threshold:
                print(f"📢 ERKEN UYARI SİSTEMİ UYARILARI:\nZaman {int(row['time'])}: STALL TEHLİKESİ ALGILANDI!")
        else:
            consecutive_count = 0

if __name__ == '__main__':
    if run_octave_script():
        analyze_data()
