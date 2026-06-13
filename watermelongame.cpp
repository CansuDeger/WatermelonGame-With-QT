//CENNET CANSU DEGER
//23100011023
#include "WatermelonGame.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QScreen>
#include <QGuiApplication>
#include <QFile>
#include <QTextStream>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>

Watermelon::Watermelon(QWidget *parent) : QLabel(parent), m_isCut(false), animation(nullptr) {

    QPixmap originalImage(":/resimler/images/watermelon.png");

    QPixmap scaledImage = originalImage.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    setPixmap(scaledImage);

    setFixedSize(scaledImage.size());
    setCursor(Qt::PointingHandCursor);
}

void Watermelon::startFallingAnimation(int startX, int startY, int endY, int duration) {
    animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(duration);

    QRect startGeom = geometry();
    startGeom.moveTopLeft(QPoint(startX, startY));
    QRect endGeom = startGeom;
    endGeom.moveTop(endY);

    animation->setStartValue(startGeom);
    animation->setEndValue(endGeom);

    animation->setEasingCurve(QEasingCurve::InQuad);

    animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void Watermelon::stopFallingAnimation() {
    if (animation && animation->state() == QAbstractAnimation::Running) {
        animation->stop();
    }
}

void Watermelon::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && !m_isCut) {
        emit clicked(this);
    }
    QLabel::mousePressEvent(event);
}

WatermelonGame::WatermelonGame(QWidget *parent)
    : QMainWindow(parent), timeLeft(30), cutCount(0), missedCount(0), gameRunning(false)
{

    QSize screenSize = getScreenSize();
    resize(screenSize);

    timeLabel = new QLabel("Süre: 30", this);
    cutCountLabel = new QLabel("Kesilen: 0", this);
    missedCountLabel = new QLabel("Kaçırılan: 0", this);
    startButton = new QPushButton("Oyunu Başlat", this);
    timeLabel->setFixedSize(140, 50);
    cutCountLabel->setFixedSize(140, 50);
    missedCountLabel->setFixedSize(140, 50);
    startButton->setFixedSize(200,50);

    QFont font("Arial", 14, QFont::Bold);
    timeLabel->setFont(font);
    cutCountLabel->setFont(font);
    missedCountLabel->setFont(font);
    timeLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 150); padding: 5px;");
    cutCountLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 150); padding: 5px;");
    missedCountLabel->setStyleSheet("color: white; background-color: rgba(0, 0, 0, 150); padding: 5px;");

    timeLabel->move(20, 20);
    cutCountLabel->move(screenSize.width() / 2 - 50, 20);
    missedCountLabel->move(screenSize.width() - 150, 20);
    startButton->move(screenSize.width() / 2 - 75, screenSize.height() / 2);

    gameTimer = new QTimer(this);
    spawnTimer = new QTimer(this);

    connect(startButton, &QPushButton::clicked, this, &WatermelonGame::startGame);
    connect(gameTimer, &QTimer::timeout, this, &WatermelonGame::updateTime);
    connect(spawnTimer, &QTimer::timeout, this, &WatermelonGame::spawnWatermelon);


    loadPositionsFromFile();
}

WatermelonGame::~WatermelonGame() {
    for (auto watermelon : watermelons) {
        delete watermelon;
    }
    watermelons.clear();
}

QSize WatermelonGame::getScreenSize() {
    QScreen *screen = QGuiApplication::primaryScreen();
    return screen->availableSize();
}


void WatermelonGame::loadPositionsFromFile() {
    positions.clear();
    currentPositionIndex = 0;

    QString filePath = "C:/Users/DELL/Desktop/QT/WatermelonGame/konumlar.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            QStringList coords = line.split(' ', Qt::SkipEmptyParts);

            if (coords.size() >= 2) {
                bool okX, okY;
                int x = coords[0].toInt(&okX);
                int y = coords[1].toInt(&okY);

                if (okX && okY) {
                    positions.append(QPair<int, int>(x, y));
                }
            }
        }
        file.close();

        if (positions.isEmpty()) {
            qDebug() << "Konumlar dosyası boş veya geçerli konum içermiyor!";
        } else {
            qDebug() << "Konumlar dosyasından " << positions.size() << " konum yüklendi.";
        }
    } else {
        qDebug() << "Konumlar dosyası açılamadı!";
    }
}

void WatermelonGame::startGame() {
    if (gameRunning) return;

    timeLeft = 30;
    cutCount = 0;
    missedCount = 0;
    currentPositionIndex = 0;


    loadPositionsFromFile();

    for (auto watermelon : watermelons) {
        delete watermelon;
    }
    watermelons.clear();

    timeLabel->setText("Süre: " + QString::number(timeLeft));
    cutCountLabel->setText("Kesilen: " + QString::number(cutCount));
    missedCountLabel->setText("Kaçırılan: " + QString::number(missedCount));
    startButton->hide();

    gameTimer->start(1000);
    spawnTimer->start(1200);

    gameRunning = true;
}

void WatermelonGame::updateTime() {
    timeLeft--;
    timeLabel->setText("Süre: " + QString::number(timeLeft));

    if (timeLeft <= 0) {
        endGame();
    }
}

void WatermelonGame::spawnWatermelon() {
    if (!gameRunning) return;

    int screenWidth = width();
    int screenHeight = height();
    int posX, posY;


    if (!positions.isEmpty()) {
        if (currentPositionIndex >= positions.size()) {
            currentPositionIndex = 0;
        }

        QPair<int, int> pos = positions.at(currentPositionIndex);
        posX = pos.first;
        posY = pos.second;
        currentPositionIndex++;


        if (posX < 0) posX = 0;
        if (posX > screenWidth - 100) posX = screenWidth - 100;

        if (posY < -100) posY = -100;
        if (posY > screenHeight - 100) posY = screenHeight - 100;
    } else {

        posX = QRandomGenerator::global()->bounded(100, screenWidth - 150);
        posY = -80; // Default starting Y
    }

    Watermelon *watermelon = new Watermelon(this);
    watermelon->move(posX, posY);
    watermelon->show();

    connect(watermelon, &Watermelon::clicked, this, &WatermelonGame::cutWatermelon);
    watermelons.append(watermelon);

    int endY = screenHeight + watermelon->height();
    int duration = QRandomGenerator::global()->bounded(8000, 10000);

    watermelon->startFallingAnimation(posX, posY, endY, duration);

    QTimer::singleShot(duration, this, [this, watermelon]() {
        if (!gameRunning || !watermelon || watermelon->isCut() || !watermelons.contains(watermelon))
            return;

        missedCount++;
        missedCountLabel->setText("Kaçırılan: " + QString::number(missedCount));

        watermelon->hide();
        watermelons.removeOne(watermelon);
        watermelon->deleteLater();
    });
}


void WatermelonGame::cutWatermelon(Watermelon* watermelon) {
    if (!gameRunning || !watermelon) return;

    if (watermelon->isCut()) return;

    watermelon->stopFallingAnimation();

    watermelon->setCut(true);

    QPixmap cutImage(":/resimler/images/cut_watermelon.png");
    QPixmap scaledCutImage = cutImage.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    watermelon->setPixmap(scaledCutImage);
    watermelon->setFixedSize(scaledCutImage.size());

    cutCount++;
    cutCountLabel->setText("Kesilen: " + QString::number(cutCount));

    QTimer::singleShot(1000, this, [this, watermelon]() {
        if (watermelon && watermelon->isCut()) {
            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(watermelon);
            watermelon->setGraphicsEffect(effect);

            QPropertyAnimation *fadeAnim = new QPropertyAnimation(effect, "opacity");
            fadeAnim->setDuration(500);
            fadeAnim->setStartValue(1.0);
            fadeAnim->setEndValue(0.0);
            fadeAnim->setEasingCurve(QEasingCurve::OutQuad);

            connect(fadeAnim, &QPropertyAnimation::finished, this, [this, watermelon]() {

                watermelon->hide();
                watermelons.removeOne(watermelon);
                watermelon->deleteLater();
            });

            fadeAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }
    });
}

void WatermelonGame::endGame() {
    gameRunning = false;

    gameTimer->stop();
    spawnTimer->stop();

    showGameResults();
    saveScore();

    startButton->show();
}

void WatermelonGame::saveScore() {
    QString filePath = "C:/Users/DELL/Desktop/QT/WatermelonGame/skorlar.txt";
    QFile file(filePath);

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << cutCount << "\n";
        file.close();
    } else {
        qDebug() << "Skorlar dosyası açılamadı!";
    }

}

int WatermelonGame::getHighScore() {
    int highScore = 0;
    QString filePath = "C:/Users/DELL/Desktop/QT/WatermelonGame/skorlar.txt";
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            int score = in.readLine().toInt();
            if (score > highScore) {
                highScore = score;
            }
        }
        file.close();
    } else {
        qDebug() << "Skorlar dosyası açılamadı!";
    }

    return highScore;
}

void WatermelonGame::showGameResults() {
    int highScore = getHighScore();
    QString message;

    message +="Kaçırılan    : " + QString::number(missedCount) + "\n";
    message+= "Kesilen :"+ QString::number(cutCount) + "\n";
    message += "Mevcut Skor: " + QString::number(cutCount) + "\n";
    message += "Yüksek Skor : " + QString::number(highScore) + "\n";

    if (cutCount > highScore) {
        message += "🎉 Tebrikler! Yeni bir yüksek skor elde ettiniz! 🎉";
    } else if (cutCount == highScore && highScore != 0) {
        message += "👏 Mevcut yüksek skoru yakaladınız!";
    } else {
        message += "😢 Maalesef yüksek skoru geçemediniz. Daha iyi şanslar!";
    }

    QMessageBox::information(this, "Oyun Sonucu", message);
}

void WatermelonGame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    QPixmap backgroundPixmap(":/resimler/images/background.jpg");

    painter.drawPixmap(0, 100, backgroundPixmap);

    QMainWindow::paintEvent(event);
}
