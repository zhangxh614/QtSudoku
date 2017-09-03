#ifndef SUBWIDGET_H
#define SUBWIDGET_H

#include "button.h"
#include "sudoku.h"

#include <QWidget>
#include <QVector>
#include <QStringList>
#include <QLabel>
#include <QUndoStack>
#include <QComboBox>

class HomeWidget:public QWidget {

    Q_OBJECT

public:
    explicit HomeWidget(QWidget* parent=0);
    Button levelBtn,solveBtn;
    QComboBox levelSelect;

signals:
    void setLevel(int n);

private slots:
    void levelBtnClicked();

};

class CalWidget:public QWidget {

    Q_OBJECT

public:
    explicit CalWidget(QWidget* parent=0);

    virtual QWidget* topBar()=0;
    QWidget* midBoard();
    QWidget* bottomInputBar();

    int limit=10;
    QVector<Button*> board;
    QStringList current;
    QUndoStack *m_UndoStack;
    Button homeBtn,hintBtn;
    SudoKu *logic;

    void paintCross(int n);
    void lightNumber(int n);
    void checkOK();
    void updateBoard(int index,QString str);

signals:
    void over();

private slots:
    void clickBoard(int n);
    virtual void digitClick(int n)=0;
    void del();
    virtual void hint()=0;
};


class PlayWidget:public CalWidget {

    Q_OBJECT

public:
    PlayWidget(QWidget *parent=0);
    Button pauseBtn;
    QLabel lcd;
    QString* initial;

    void setBoard(int level);
    void initBoard(QString* str);

private:
    QWidget* topBar();

private slots:
    void hint();
    void retry();
    void digitClick(int n);
};


class SolveWidget:public CalWidget {

    Q_OBJECT

public:
    SolveWidget(QWidget *parent=0);
    void initBoard();

private:
    QWidget* topBar();

private slots:
    void hint();
    void digitClick(int n);
    void retry();
};

class WinWidget:public QWidget {

    Q_OBJECT

public:
    explicit WinWidget(QWidget* parent=0);
    Button homeBtn;
    QLabel *difficulty,*totalTime;

    void setInfo(QString dif,QString time);

};

class PauseWidget:public QWidget {

    Q_OBJECT

public:
    explicit PauseWidget(QWidget* parent=0);
    Button homeBtn,returnBtn;

};

#endif // SUBWIDGET_H
