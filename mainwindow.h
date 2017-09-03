#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "subwidget.h"

#include <QMainWindow>
#include <QVector>

class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class Button;
class QStringList;
class Board;
class SudoKu;
class QLCDNumber;
class QTimer;
class QTime;
class QLabel;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QStackedWidget *mainWidget;
    QString dif;

    HomeWidget *home;
    PlayWidget *play;
    WinWidget *end;
    PauseWidget *pause;
    SolveWidget *solve;

    //Stopwatch
    QTimer *timer;
    QTime *timeRecord;
    QLabel *lcd;
    bool isStart;

    void startWatch();
    void resetWatch();
    void stopWatch();

private slots:
    void changeWidget(int);
    void goSolve();
    void clickBack();
    void clickPause();
    void updateTime();
    void returnPlay();
    void win();
};

#endif // MAINWINDOW_H
