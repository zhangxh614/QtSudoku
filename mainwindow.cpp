#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStackedWidget>
#include <QSignalMapper>
#include <QWidget>
#include <QDebug>
#include <QPalette>
#include <QTimer>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainWidget=new QStackedWidget();
    setCentralWidget(mainWidget);

    timer=new QTimer;
    timeRecord=new QTime(0,0,0);
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));


    home=new HomeWidget;
    connect(&home->solveBtn,SIGNAL(clicked()),this,SLOT(goSolve()));
    connect(home,SIGNAL(setLevel(int)),this,SLOT(changeWidget(int)));

    play=new PlayWidget;

    connect(&play->pauseBtn,SIGNAL(clicked()),this,SLOT(clickPause()));
    play->lcd.setText(timeRecord->toString("mm:ss"));
    connect(play,SIGNAL(over()),this,SLOT(win()));
    connect(&play->homeBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    end=new WinWidget;
    connect(&end->homeBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    pause=new PauseWidget;
    connect(&pause->homeBtn,SIGNAL(clicked()),this,SLOT(clickBack()));
    connect(&pause->returnBtn,SIGNAL(clicked()),this,SLOT(returnPlay()));

    solve=new SolveWidget;
    connect(&solve->homeBtn,SIGNAL(clicked()),this,SLOT(clickBack()));

    mainWidget->addWidget(home);
    mainWidget->addWidget(play);
    mainWidget->addWidget(end);
    mainWidget->addWidget(pause);
    mainWidget->addWidget(solve);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::changeWidget(int n) {
    dif="Level"+QString::number(n,10);
    play->setBoard(n);
    startWatch();
    mainWidget->setCurrentIndex(1);
}

void MainWindow::goSolve() {
    mainWidget->setCurrentIndex(4);
    solve->initBoard();
}

void MainWindow::clickBack() {
    mainWidget->setCurrentIndex(0);
    resetWatch();
}

void MainWindow::startWatch() {
    isStart=true;
    timer->start(1000);
}

void MainWindow::resetWatch() {
    isStart=false;
    timer->stop();
    timeRecord->setHMS(0,0,0);
    play->lcd.setText(timeRecord->toString("mm:ss"));
}

void MainWindow::stopWatch() {
    isStart=false;
    timer->stop();
}

void MainWindow::updateTime() {
    *timeRecord=timeRecord->addSecs(1);
    play->lcd.setText(timeRecord->toString("mm:ss"));
}

void MainWindow::clickPause() {
    mainWidget->setCurrentIndex(3);
    stopWatch();
}

void MainWindow::returnPlay() {
    mainWidget->setCurrentIndex(1);
    startWatch();
}

void MainWindow::win() {
    stopWatch();
    end->setInfo(dif,timeRecord->toString("mm:ss"));
    mainWidget->setCurrentIndex(2);
}
