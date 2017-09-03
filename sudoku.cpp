#include "sudoku.h"

#include <QDebug>
#include <QTime>

SudoKu::SudoKu() {

}

QString* SudoKu::generate() {
    result=new QString("");
    ans=new QString;
    memset(board,0,sizeof(board));
    memset(tmpBoard,0,sizeof(tmpBoard));

    for(int i=0;i<len;i++) {
        int tmpPosition=0;
        int tmpValue=0;
        do {
            tmpPosition=generateRandomNumber(81);
        }while(tmpBoard[tmpPosition]!=0);
        do {
            tmpValue=generateRandomNumber(9)+1;
            tmpBoard[tmpPosition]=tmpValue;
        }while(!isLegal(tmpPosition,tmpValue));
    }
    copy(board,tmpBoard,81);

    getWholeBoard();

    for(int i=0;i<81;i++) {
        ans->append(QString::number(wholeBoard[i],10));
    }
    qDebug()<<*ans;
    copy(board,wholeBoard,81);
    int sum=0,position=0;

    int flag[81];
    memset(flag,0,sizeof(flag));
    do {
        position=generateRandomNumber(81);
        if(flag[position]==1) continue;
        flag[position]=1;
        int tmpValue=board[position];
        board[position]=0;
        checkNum();
        if(count!=1)
            board[position]=tmpValue;
        else
            sum++;
    }while(sum<level);

    //qDebug()<<sum<<level;
    for(int i=0;i<81;i++) {
        result->append(QString::number(board[i]));
    }

    return result;
}


int SudoKu::generateRandomNumber(int range) {
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);
    return qrand()%range;
}

void SudoKu::copy(int *a, int *b,int length) {
    for(int i=0;i<length;i++) {
        a[i]=b[i];
    }
}

bool SudoKu::isOver(QString* str) {
    return QString::compare(*str,*ans)==0;
}

bool SudoKu::isLegal(int x,int n) {
    int row=x/9;
    int col=x%9;
    for(int i=0;i<9;i++) {
        if(tmpBoard[row*9+i]==n&&i!=col) {
            return false;
        } else if(tmpBoard[i*9+col]==n&&i!=row) {
            return false;
        }
    }
    int xx = row / 3;
    int yy = col / 3;
    for(int i = xx * 3; i < xx * 3 + 3; i ++)
        for(int j = yy * 3; j < yy * 3 + 3; j ++) {
            if(tmpBoard[i*9+j] == n && (i != row || j != col)) {
                    return false;
            }
        }
    return true;
}

void SudoKu::calOnly(int sum) {
    if(sum>=81) {
        count++;
        return;
    }
    if(count>=2)
        return;
    if(tmpBoard[sum]==0) {
        for(int j=1;j<=9;j++) {
            tmpBoard[sum]=j;
            if(isLegal(sum,j)) {
                calOnly(sum+1);
            }
            tmpBoard[sum]=0;
        }
    } else {
        calOnly(sum+1);
    }
}

void SudoKu::calWholeBoard(int sum) {
    if(sum>=81) {
        count++;
        copy(wholeBoard,tmpBoard,81);
        return;
    }
    if(count>=1)
        return;
    if(tmpBoard[sum]==0) {
        for(int j=1;j<=9;j++) {
            tmpBoard[sum]=j;
            if(isLegal(sum,j)) {
                calWholeBoard(sum+1);
            }
            tmpBoard[sum]=0;
        }
    } else {
        calWholeBoard(sum+1);
    }
}

void SudoKu::checkNum() {
    count=0;
    copy(tmpBoard,board,81);
    calOnly(0);
}

void SudoKu::getWholeBoard() {
    count=0;
    copy(tmpBoard,board,81);
    calWholeBoard(0);
}

void SudoKu::setLEvel(int n) {
    level=generateRandomNumber(4)+4*n+10;
}

bool SudoKu::isRight(int n,QString str) {
    if(ans->at(n)!=str) return false;
    return true;
}

QString SudoKu::get(int n) {
    return ans->at(n);
}

QString* SudoKu::solve(QStringList *str) {
    for(int i=0;i<81;i++) {
        if(str->at(i)=="") {
            board[i]=0;
            continue;
        }
        board[i]=str->at(i).toInt();
    }
    getWholeBoard();
    QString* a=new QString("");
    for(int i=0;i<81;i++) {
        a->append(QString::number(wholeBoard[i],10));
    }
    return a;
}
