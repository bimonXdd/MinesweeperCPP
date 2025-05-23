#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QTimer>
#include "grid.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    private slots:
        void handleButtonClick(int x, int y);
    void updateTimer();

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    Ui::MainWindow *ui;
    grid gameGrid;
    QVector<QVector<QPushButton*>> buttonGrid;
    QTimer *gameTimer;
    int elapsedSeconds = 0;
    int lives = 3;

    void updateUI();
    void setupButtons();
    void resetGame();
};

#endif // MAINWINDOW_H
