//CENNET CANSU DEGER
//23100011023
#ifndef WATERMELONGAME_H
#define WATERMELONGAME_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QPropertyAnimation>
#include <QList>
#include <QPair>

class Watermelon : public QLabel {
    Q_OBJECT

public:
    explicit Watermelon(QWidget *parent = nullptr);
    void startFallingAnimation(int startX, int startY, int endY, int duration);
    void stopFallingAnimation();
    bool isCut() const { return m_isCut; }
    void setCut(bool cut) { m_isCut = cut; }

signals:
    void clicked(Watermelon* watermelon);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool m_isCut;
    QPropertyAnimation *animation;
};

class WatermelonGame : public QMainWindow {
    Q_OBJECT

public:
    explicit WatermelonGame(QWidget *parent = nullptr);
    ~WatermelonGame();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void startGame();
    void updateTime();
    void spawnWatermelon();
    void cutWatermelon(Watermelon* watermelon);

private:
    void endGame();
    void saveScore();
    int getHighScore();
    void showGameResults();
    QSize getScreenSize();

    void loadPositionsFromFile();

    QLabel *timeLabel;
    QLabel *cutCountLabel;
    QLabel *missedCountLabel;
    QPushButton *startButton;
    QTimer *gameTimer;
    QTimer *spawnTimer;
    QList<Watermelon*> watermelons;

    QList<QPair<int, int>> positions;
    int currentPositionIndex;

    int timeLeft;
    int cutCount;
    int missedCount;
    bool gameRunning;
};

#endif // WATERMELONGAME_H
