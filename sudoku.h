#ifndef SUDOKU_H
#define SUDOKU_H
#include <QString>

class SudoKu {
private:
    enum { len = 11 };

    QString *ans,*result;

    int board[81];
    int wholeBoard[81];
    int tmpBoard[81];
    int count;
    int level;

public:
    SudoKu();
    ~SudoKu();

    //improve search
    QString* generate();
    int generateRandomNumber(int range);
    bool isOver(QString* str);
    bool isLegal(int x,int n);
    void checkNum();
    void getWholeBoard();
    void calOnly(int sum);
    void calWholeBoard(int sum);
    void copy(int* a,int* b,int length);
    void setLEvel(int n);
    bool isRight(int n,QString str);
    QString* solve(QStringList *str);
    QString get(int n);
};

#endif // SUDOKU_H
