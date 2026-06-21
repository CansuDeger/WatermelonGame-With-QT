# 🍉 Watermelon Game

Qt/C++ ile geliştirilmiş, fare ile karpuz kesme temalı bir reflex/arcade oyunu. Ekrandan düşen karpuzları süre dolmadan keserek skorunuzu yükseltmeye çalışın!

## 🎮 Oynanış

- **Oyunu Başlat** butonuna basarak 30 saniyelik bir tur başlatın
- Ekranın üstünden aşağı doğru düşen karpuzlara tıklayarak onları **kesin**
- Kesemediğiniz (ekranın altına ulaşan) her karpuz **kaçırılan** sayacına eklenir
- Süre dolduğunda kesilen/kaçırılan istatistikleriniz ve **yüksek skorunuz** bir bilgi penceresinde gösterilir

## ✨ Özellikler

- 🎬 **Akıcı düşme animasyonu** — `QPropertyAnimation` ile her karpuz farklı süre ve hızda düşer (8-10 saniye arası rastgele)
- ✂️ **Kesme efekti** — tıklanan karpuz görseli değişir ve kısa bir solma (fade-out) animasyonuyla ekrandan kaybolur
- ⏱️ Gerçek zamanlı **süre, kesilen ve kaçırılan** sayaçları
- 🏆 **Yüksek skor takibi** — önceki oyunlarla karşılaştırmalı sonuç ekranı
- 📍 Karpuzların düşme konumları isteğe bağlı olarak bir metin dosyasından (`konumlar.txt`) okunur; dosya yoksa konumlar rastgele üretilir
- 🖥️ Pencere boyutu, çalıştırılan ekranın çözünürlüğüne göre otomatik ayarlanır

## 🛠️ Teknoloji Yığını

| Katman | Teknoloji |
|---|---|
| Arayüz / Grafik | Qt Widgets, `QPainter`, `QPixmap` |
| Animasyon | `QPropertyAnimation`, `QGraphicsOpacityEffect` |
| Zamanlama | `QTimer` |
| Veri Saklama | Düz metin dosyaları (`.txt`) |
| Dil / Standart | C++ |

## 📁 Proje Yapısı

```
├── main.cpp                  # Uygulama giriş noktası
├── watermelongame.h/.cpp     # Oyun mantığı, Watermelon ve WatermelonGame sınıfları
├── resources.qrc             # Karpuz, kesilmiş karpuz ve arka plan görselleri
└── WatermelonGame.pro        # Qt proje dosyası

```

## 🍓 Sınıf Yapısı

- **`Watermelon`** (`QLabel` türevi) — tek bir karpuzun görselini, düşme animasyonunu ve tıklama olayını yönetir
- **`WatermelonGame`** (`QMainWindow` türevi) — oyun döngüsü, sayaçlar, karpuz üretimi (`spawnWatermelon`) ve skor yönetimini kapsar

## 🚀 Kurulum

1. [Qt](https://www.qt.io/download) (Qt Creator önerilir, Qt 5.12+ veya Qt 6) kurulu olmalı
2. Repoyu klonlayın:
   ```bash
   git clone https://github.com/kullanici-adi/watermelon-game.git
   ```
3. `WatermelonGame.pro` dosyasını Qt Creator ile açın
4. Projeyi derleyip çalıştırın (`Ctrl+R`)

## ⚠️ Bilinmesi Gerekenler

- Karpuz konumları (`konumlar.txt`) ve skor kayıtları (`skorlar.txt`) şu anda **sabit (hardcoded) bir Windows yoluna** yazılıyor:
  ```
  C:/Users/DELL/Desktop/QT/WatermelonGame/
  ```
  Bu nedenle projeyi farklı bir bilgisayarda veya farklı bir işletim sisteminde çalıştırırken:
  - `konumlar.txt` bulunamazsa karpuzlar **rastgele konumlardan** düşmeye devam eder (oyun yine de oynanabilir)
  - `skorlar.txt` bulunamazsa **skor kaydedilmez** ve yüksek skor her zaman 0 görünür

  Kendi ortamınızda çalıştırmak isterseniz `watermelongame.cpp` içindeki `filePath` değişkenlerini (`loadPositionsFromFile()` ve `saveScore()` / `getHighScore()` fonksiyonlarında) kendi dizininize göre güncellemeniz gerekir.

## 📌 Notlar

- Bu proje bir ders ödevi kapsamında geliştirilmiştir

---

**Geliştirici:** Cennet Cansu Değer
