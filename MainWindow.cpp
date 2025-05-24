#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMouseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupButtons();
    resetGame();

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &MainWindow::updateTimer);
    gameTimer->start(1000);

    connect(ui->uusMangNupp, &QPushButton::clicked, this, &MainWindow::resetGame);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupButtons() {
    buttonGrid.clear();
    for (int i = 0; i < 10; ++i) {
        QVector<QPushButton*> row;
        for (int j = 0; j < 10; ++j) {
            int index = i * 10 + j + 1;
            QString name = (index == 1) ? "pushButton" : "pushButton_" + QString::number(index);
            QPushButton *btn = findChild<QPushButton*>(name);
            if (btn) {
                btn->setFixedSize(40, 40);
                row.append(btn);
                btn->setProperty("gridX", i);
                btn->setProperty("gridY", j);
                btn->installEventFilter(this);
            }
        }
        buttonGrid.append(row);
    }
}

void MainWindow::resetGame() {
    gameGrid.width = 10;
    gameGrid.height = 10;
    gameGrid.data.clear();
    gameGrid.init(10);
    gameGrid.reveal_all = false;
    gameGrid.number_bombs();
    elapsedSeconds = 0;
    lives = 3;

    ui->Aeg->setText("Aeg: 0");
    ui->Miinid->setText("Miinid: 10");
    ui->Elud->setText("Elud: 3");

    for (auto &row : buttonGrid) {
        for (auto &btn : row) {
            btn->setText("");
            btn->setEnabled(true);
            btn->setStyleSheet("");
        }
    }
    if (gameTimer) {
        gameTimer->stop();         // kindluse mõttes peata vana
        gameTimer->start(1000);    // ja käivita uuesti
    }
}

void MainWindow::handleButtonClick(int x, int y) {
    if (gameGrid.onAMine(x, y)) {
        lives--;
        ui->Elud->setText("Elud: " + QString::number(lives));
        if (lives == 0) {
            gameGrid.reveal_all = true;
            updateUI();
            gameTimer->stop();
            QMessageBox::information(this, "Kaotus", "Astusid liiga paljudele miinidele.");
            return;
        }
    }

    std::vector<int> loc = {x, y};
    gameGrid.reveal(loc);
    updateUI();

    if (gameGrid.reveal_all) {
        updateUI();
        for (auto &row : buttonGrid)
            for (auto &btn : row)
                btn->setEnabled(false);
        gameTimer->stop();
        return;
    }

    if (gameGrid.gameWon()) {
        gameTimer->stop();
        QMessageBox::information(this, "Võit", "Tubli! Sa võitsid mängu!");
    }
}

void MainWindow::updateUI() {
    for (int i = 0; i < gameGrid.height; ++i) {
        for (int j = 0; j < gameGrid.width; ++j) {
            const cell &c = gameGrid.data[i][j];
            QPushButton *btn = buttonGrid[i][j];

            if (!c.hidden || gameGrid.reveal_all) {
                if (c.mine) {
                    btn->setText("💣");
                    btn->setStyleSheet("background-color: red; color: white; font-weight: bold;");
                } else {
                    if (c.value > 0) {
                        btn->setText(QString::number(c.value));

                        QString color;
                        switch (c.value) {
                            case 1:  color = "blue"; break;
                            case 2:  color = "green"; break;
                            case 3:  color = "#8b2323"; break;
                            case 4:  color = "black"; break;
                            case 5:  color = "gold"; break;
                            case 6:  color = "#ffcc00"; break;
                            case 7:  color = "orange"; break;
                            case 8:  color = "#ff6600"; break;
                            case 9:  color = "#ff3333"; break;
                            case 10: color = "red"; break;
                            case 11: color = "#cc0000"; break;
                            case 12: color = "#990000"; break;
                            case 13: color = "#990033"; break;
                            case 14: color = "#800080"; break;
                            case 15: color = "#4B0082"; break;
                            case 16: color = "black"; break;
                            default: color = "gray"; break;
                        }

                        btn->setStyleSheet("background-color: #a0a0a0; color: " + color + "; font-weight: bold;");
                    } else {
                        btn->setText("");
                        btn->setStyleSheet("background-color: #a0a0a0;");
                    }

                    btn->setEnabled(false);
                }
            } else if (c.flag) {
                btn->setText("🚩");
                btn->setStyleSheet("");
            } else {
                btn->setText("");
                btn->setStyleSheet("");
            }
        }
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        QPushButton *btn = qobject_cast<QPushButton*>(obj);
        if (!btn) return false;

        int x = btn->property("gridX").toInt();
        int y = btn->property("gridY").toInt();

        if (x < 0 || x >= gameGrid.height || y < 0 || y >= gameGrid.width)
            return false; // ohutus

        cell &c = gameGrid.data[x][y];

        if (mouseEvent->button() == Qt::RightButton) {
            if (c.hidden) {
                c.flag = !c.flag;
                updateUI();
            }
            return true;
        }

        if (mouseEvent->button() == Qt::LeftButton) {
            if (!c.flag) {
                handleButtonClick(x, y);
            }
            return true;
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::updateTimer() {
    elapsedSeconds++;
    ui->Aeg->setText("Aeg: " + QString::number(elapsedSeconds));
}


