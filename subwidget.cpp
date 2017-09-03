#include "subwidget.h"
#include "undo.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSignalMapper>
#include <QLabel>
#include <QStyledItemDelegate>
#include <QPalette>


HomeWidget::HomeWidget(QWidget *parent):QWidget(parent) {
    QHBoxLayout *row=new QHBoxLayout;
    QGridLayout *grid=new QGridLayout;
    QVBoxLayout *column0=new QVBoxLayout;
    QVBoxLayout *column1=new QVBoxLayout;

    column0->addSpacing(40);
    QLabel *title=new QLabel;
    title->setText("Sudoku");
    title->setStyleSheet("font-size:120px;color:#00303f");
    column0->addWidget(title);
    column0->addSpacing(40);

    QLabel *l=new QLabel;
    l->setText("  Level:");
    l->setMinimumSize(55,100);
    l->setStyleSheet("font-size:30px");
    levelSelect.setMinimumSize(110,50);
    levelSelect.setSizeAdjustPolicy(QComboBox::AdjustToContents);
    levelSelect.setStyleSheet("font-size:28px");
    QStyledItemDelegate* itemDelegate = new QStyledItemDelegate();
    levelSelect.setItemDelegate(itemDelegate);
    setStyleSheet("QComboBox QAbstractItemView::item {min-height: 40px;}");


    for(int i=0;i<10;i++) {
        levelSelect.addItem("Level "+QString::number(i,10));
    }

    QHBoxLayout *h=new QHBoxLayout;
    h->addWidget(l);
    h->addWidget(&levelSelect);
    h->addSpacing(5);

    levelBtn.setText("Go");
    solveBtn.setText("Solve");

    column1->addLayout(h);
    column1->addSpacing(30);
    //column1->addStretch();
    levelBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    levelBtn.setMaximumSize(260,100);
    levelBtn.setStyleSheet("color:#00303f;\
                            border:2px groove #00303f;\
                            border-radius:50px;\
                            padding:2px 4px;\
                            font-size:45px");
    levelBtn.setAlignment(Qt::AlignCenter);
    column1->addWidget(&levelBtn);
    column1->addSpacing(20);

    solveBtn.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    solveBtn.setMaximumSize(260,100);
    solveBtn.setStyleSheet("color:#00303f;\
                            border:2px groove #00303f;\
                            border-radius:50px;\
                            padding:2px 4px;\
                            font-size:45px");
    solveBtn.setAlignment(Qt::AlignCenter);
    column1->addWidget(&solveBtn);
    column1->addSpacing(50);
    //column1->addStretch();

    grid->addLayout(column0,1,0,3,3,Qt::AlignCenter);
    grid->addLayout(column1,4,1,1,1,Qt::AlignCenter);

    row->addStretch();
    row->addLayout(grid);
    row->addStretch();

    this->setLayout(row);

    connect(&levelBtn,SIGNAL(clicked()),this,SLOT(levelBtnClicked()));

    QPalette p;
    p.setColor(QPalette::Background,QColor(202,228,219));
    this->setAutoFillBackground(true);
    this->setPalette(p);

}

void HomeWidget::levelBtnClicked() {
    int n=levelSelect.currentIndex();
    emit setLevel(n);
}


CalWidget::CalWidget(QWidget *parent):QWidget(parent) {
    m_UndoStack = new QUndoStack(this);
    m_UndoStack->setUndoLimit(10);

    logic=new SudoKu;

    homeBtn.setText("Home");

    QPalette p;
    p.setColor(QPalette::Background,QColor(202,228,219));
    this->setAutoFillBackground(true);
    this->setPalette(p);

}

QWidget* CalWidget::midBoard() {
    board.clear();
    int size=60;

    QSignalMapper *m=new QSignalMapper(this);
    QWidget *w=new QWidget;

    w->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
    QPalette p;
    p.setColor(QPalette::Background,QColor(0,0,0));
    w->setAutoFillBackground(true);
    w->setPalette(p);

    QGridLayout *box=new QGridLayout;
    QGridLayout *smallBox=new QGridLayout[9];

    box->setSpacing(5);
    box->setContentsMargins(5,5,5,5);

    for(int i=0;i<9;i++) {
        QWidget* tmp=new QWidget;
        tmp->setLayout(&smallBox[i]);

        QPalette p;
        p.setColor(QPalette::Background,QColor(0,0,0));
        tmp->setAutoFillBackground(true);
        tmp->setPalette(p);

        box->addWidget(tmp,i/3,i%3);
        box->setRowStretch(i,1);
        box->setColumnStretch(i,1);
        for(int j=0;j<3;j++) {
            smallBox[i].setRowStretch(j,1);
            smallBox[i].setColumnStretch(j,1);
        }
        smallBox[i].setSpacing(3);
        smallBox[i].setContentsMargins(0,0,0,0);
    }

    for(int i=0;i<9;i++) {
        for(int j=0;j<9;j++) {
            Button *b=new Button("");
            b->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
            b->setMinimumSize(size,size);
            b->setAlignment(Qt::AlignCenter);
            connect(b,SIGNAL(clicked()),m,SLOT(map()));
            m->setMapping(b,i*9+j);
            int xx=i/3;
            int yy=j/3;
            smallBox[xx*3+yy].addWidget(b,i-xx*3,j-yy*3);
            board.append(b);
        }
    }

    connect(m,SIGNAL(mapped(int)),this,SLOT(clickBoard(int)));

    w->setLayout(box);

    return w;
}

QWidget* CalWidget::bottomInputBar() {
    int size=60;

    QSignalMapper *m=new QSignalMapper(this);

    QWidget *w=new QWidget;
    QHBoxLayout *row=new QHBoxLayout;

    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QGridLayout *right=new QGridLayout;
    QWidget *input=new QWidget;

    input->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    for(int i=0;i<3;i++) {
        right->setRowStretch(i,1);
        right->setColumnStretch(i,1);
    }
    right->setSpacing(10);

    for(int i=1;i<10;i++) {
        Button *b=new Button;
        b->setMinimumSize(size,size);
        b->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        b->setAlignment(Qt::AlignCenter);
        b->setText(QString::number(i,10));
        b->setStyleSheet("background-color:#00303f;color:#ffffff");
        connect(b,SIGNAL(clicked()),m,SLOT(map()));
        m->setMapping(b,i);
        right->addWidget(b,(i-1)/3,(i-1)%3);
    }

    connect(m,SIGNAL(mapped(int)),this,SLOT(digitClick(int)));

    input->setLayout(right);

    QGridLayout *side=new QGridLayout;
    QWidget *sideW=new QWidget;

    sideW->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    for(int i=0;i<2;i++) {
        side->setRowStretch(i,1);
        side->setColumnStretch(i,1);
    }
    side->setSpacing(15);

    Button *btn=new Button[3];
    for(int i=0;i<3;i++) {
        btn[i].setMinimumSize(size+10,size+10);
        btn[i].setAlignment(Qt::AlignCenter);
        btn[i].setStyleSheet("font-size:20px;background-color:#00303f;color:#ffffff");
        btn[i].setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
        side->addWidget(&btn[i],i/2,i%2);
    }

    btn[0].setText("Undo");
    connect(&btn[0],SIGNAL(clicked()),m_UndoStack,SLOT(undo()));
    btn[1].setText("Redo");
    connect(&btn[1],SIGNAL(clicked()),m_UndoStack,SLOT(redo()));
    btn[2].setText("Delete");
    connect(&btn[2],SIGNAL(clicked()),this,SLOT(del()));

    hintBtn.setText("Hint");
    hintBtn.setMinimumSize(size+5,size+5);
    hintBtn.setAlignment(Qt::AlignCenter);
    hintBtn.setStyleSheet("font-size:20px;background-color:#00303f;color:#ffffff");
    hintBtn.setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    side->addWidget(&hintBtn,1,1);

    sideW->setLayout(side);

    row->addSpacing(20);
    row->addWidget(sideW);
    row->addStretch();
    row->addWidget(input);
    row->addSpacing(30);

    w->setLayout(row);

    return w;
}

void CalWidget::updateBoard(int index, QString str) {
    board[index]->setText(str);
}

void CalWidget::paintCross(int n) {
    int col=n%9;
    int row=n/9;
    for(int i=0;i<9;i++) {
        board[i*9+col]->setStyleSheet("background-color:#efd9c1;");
        board[i+row*9]->setStyleSheet("background-color:#efd9c1");
    }
}

void CalWidget::lightNumber(int n) {
    if(board[n]->text().length()==1) {
        for(int i=0;i<board.length();i++) {
            if(board[n]->text()==board[i]->text())
                board[i]->setStyleSheet("background-color:#dcae1d");
        }
    }
}

void CalWidget::checkOK() {
    QString tmp=current.join("");
    if(logic->isOver(&tmp))
        emit over();
}

void CalWidget::clickBoard(int n) {
    for(int i=0;i<board.length();i++) {
        board[i]->setStyleSheet("");
        board[i]->setChecked(false);
    }
    if(board[n]->isCheckable()) {
        paintCross(n);
        board[n]->setChecked(true);
    }
    lightNumber(n);
}

void CalWidget::del() {
    for(int i=0;i<board.length();i++) {
        if(board[i]->isChecked()) {
            if(board[i]->text()==0) break;
            m_UndoStack->push(new SudokuCommand(this,i,board[i]->text(),QString("")));
            board[i]->setText("");
            for(int i=0;i<board.length();i++) {
                board[i]->setStyleSheet("");
            }
            paintCross(i);
            board[i]->resetFont(30);
            break;
        }
    }
}

PlayWidget::PlayWidget(QWidget *parent):CalWidget(parent) {
    QHBoxLayout *row=new QHBoxLayout;
    QVBoxLayout *col=new QVBoxLayout;

    QWidget *w=new QWidget;
    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    col->addStretch();
    col->addWidget(topBar());
    col->addSpacing(10);
    col->addWidget(midBoard());
    col->addSpacing(10);
    col->addWidget(bottomInputBar());
    col->addStretch();

    w->setLayout(col);

    row->addStretch();
    row->addWidget(w);
    row->addStretch();

    connect(&hintBtn,SIGNAL(clicked()),this,SLOT(hint()));

    this->setLayout(row);
}

QWidget* PlayWidget::topBar() {
    int width=80,height=50;
    QWidget *w=new QWidget;

    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QHBoxLayout *row=new QHBoxLayout;

    homeBtn.setMinimumSize(width,height);
    //homeBtn.setStyleSheet("background-color:#003d3f;color:white"); 
    homeBtn.setStyleSheet("color:white;background-color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:24px;\
                           padding:2px 4px;\
                           font-size:30px");


    Button* retry=new Button;
    retry->setMinimumSize(width,height);
    retry->setText("Retry");
    //retry->setStyleSheet("background-color:#003d3f;color:white");
    retry->setStyleSheet("color:white;background-color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:24px;\
                           padding:2px 4px;\
                           font-size:30px");
    connect(retry,SIGNAL(clicked()),this,SLOT(retry()));

    pauseBtn.setMinimumSize(width,height);
    pauseBtn.setText("Pause");
    //pauseBtn.setStyleSheet("background-color:#003d3f;color:white");
    pauseBtn.setStyleSheet("color:white;background-color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:24px;\
                           padding:2px 4px;\
                           font-size:30px");

    lcd.setMaximumSize(width,height);
    lcd.setStyleSheet("font-size:30px;border:none;color:00303f");

    row->addWidget(&homeBtn);
    row->addSpacing(10);
    row->addWidget(retry);
    row->addSpacing(10);
    row->addWidget(&pauseBtn);
    row->addStretch();
    row->addWidget(&lcd);
    row->addSpacing(30);

    w->setLayout(row);

    return w;
}

void PlayWidget::setBoard(int n) {
    logic->setLEvel(n);
    initial=logic->generate();
    initBoard(initial);
}

void PlayWidget::initBoard(QString *str) {
    current.clear();
    m_UndoStack->clear();
    for(int i=0;i<81;i++) {
        board[i]->setChecked(false);
        board[i]->setStyleSheet("");
        if(str->at(i)=='0') {
            board[i]->setBaseColor(202,228,219,128,0,0);
            board[i]->setCheckable(true);
            board[i]->setText("");

        } else {
            board[i]->setBaseColor(122,157,150,0,0,0);
            board[i]->setText(QString(str->at(i)));
            board[i]->setCheckable(false);
        }
        current.append(board[i]->text());
    }
}

void PlayWidget::hint() {
    for(int i=0;i<81;i++) {
        board[i]->setStyleSheet("");
    }
    for(int i=0;i<81;i++) {
        if(!logic->isRight(i,current.at(i))) {
            board[i]->setText(logic->get(i));
            board[i]->setStyleSheet("background-color:#dcae1d");
            current.replace(i,board[i]->text());
            break;
        }
    }
    checkOK();
}

void PlayWidget::retry() {
    initBoard(initial);
}

void PlayWidget::digitClick(int n) {
    for(int i=0;i<board.length();i++) {
        board[i]->setStyleSheet("");
    }
    for(int i=0;i<board.length();i++) {
        if(board[i]->isChecked()) {
            paintCross(i);
            QString *oldStr=new QString(board[i]->text());
            if(board[i]->updateText(QString::number(n,10))){
                current.replace(i,board[i]->text());
                m_UndoStack->push(new SudokuCommand(this,i,*oldStr,board[i]->text()));
            }
            lightNumber(i);
            break;
        }
    }
    checkOK();
}

SolveWidget::SolveWidget(QWidget *parent):CalWidget(parent) {
    QHBoxLayout *row=new QHBoxLayout;
    QVBoxLayout *col=new QVBoxLayout;

    col->addStretch();
    col->addWidget(topBar());
    col->addSpacing(10);
    col->addWidget(midBoard());
    col->addSpacing(10);
    col->addWidget(bottomInputBar());
    col->addStretch();

    row->addStretch();
    row->addLayout(col);
    row->addStretch();

    connect(&hintBtn,SIGNAL(clicked()),this,SLOT(hint()));

    initBoard();

    this->setLayout(row);
}

QWidget* SolveWidget::topBar() {
    int width=80,height=50;
    QWidget *w=new QWidget;

    w->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);

    QHBoxLayout *row=new QHBoxLayout;

    homeBtn.setMinimumSize(width,height);
    homeBtn.setStyleSheet("color:white;background-color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:24px;\
                           padding:2px 4px;\
                           font-size:30px");

    Button* retry=new Button;
    retry->setMinimumSize(width,height);
    retry->setText("Retry");
    //retry->setStyleSheet("background-color:#003d3f;color:white");
    retry->setStyleSheet("color:white;background-color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:24px;\
                           padding:2px 4px;\
                           font-size:30px");
    connect(retry,SIGNAL(clicked()),this,SLOT(retry()));

    row->addWidget(&homeBtn);
    row->addSpacing(10);
    row->addWidget(retry);
    row->addStretch();

    w->setLayout(row);

    return w;
}

void SolveWidget::initBoard() {
    current.clear();
    for(int i=0;i<81;i++) {
        board[i]->setChecked(false);
        board[i]->setStyleSheet("");
        board[i]->setBaseColor(202,228,219,0,0,0);
        board[i]->setCheckable(true);
        board[i]->setText("");
        current.append("");
    }
}
void SolveWidget::retry() {
    initBoard();
}

void SolveWidget::hint() {
    QString *a=logic->solve(&current);
    for(int i=0;i<81;i++) {
        board[i]->setText(a->at(i));
    }
}

void SolveWidget::digitClick(int n) {
    for(int i=0;i<board.length();i++) {
        board[i]->setStyleSheet("");
    }
    for(int i=0;i<board.length();i++) {
        if(board[i]->isChecked()) {
            paintCross(i);
            QString *oldStr=new QString(board[i]->text());
            if(board[i]->text()!=QString::number(n,10)){
                board[i]->setText(QString::number(n,10));
                current.replace(i,board[i]->text());
                m_UndoStack->push(new SudokuCommand(this,i,*oldStr,board[i]->text()));
            }
            lightNumber(i);
            break;
        }
    }
}

WinWidget::WinWidget(QWidget *parent):QWidget(parent) {
    QLabel *v=new QLabel;
    QGridLayout *grid=new QGridLayout;

    v->setText("VICTORY");
    grid->addWidget(v,0,0,3,3,Qt::AlignCenter);
    v->setStyleSheet("font-size:100px;color:white;text-align:center;");

    QWidget *info=new QWidget;
    QVBoxLayout *col=new QVBoxLayout;

    difficulty=new QLabel;
    difficulty->setText("Difficulty:");
    difficulty->setAlignment(Qt::AlignCenter);
    difficulty->setStyleSheet("font-size:35px;color:white");
    totalTime=new QLabel;
    totalTime->setText("Time:");
    totalTime->setStyleSheet("font-size:35px;color:white");
    totalTime->setAlignment(Qt::AlignCenter);

    col->addWidget(difficulty);
    col->addWidget(totalTime);
    info->setLayout(col);
    grid->addWidget(info,2,0,3,3,Qt::AlignCenter);

    homeBtn.setText("Home");
    homeBtn.setFixedSize(260,100);
    grid->addWidget(&homeBtn,4,0,3,3,Qt::AlignCenter);

    homeBtn.setStyleSheet("color:white;\
                           border:2px groove white;\
                           border-radius:50px;\
                           padding:2px 4px;\
                           font-size:40px");
    homeBtn.setAlignment(Qt::AlignCenter);

    this->setLayout(grid);

    QPalette p;
    p.setColor(QPalette::Background,QColor(107,186,167));
    this->setAutoFillBackground(true);
    this->setPalette(p);

}

void WinWidget::setInfo(QString dif, QString time) {
    difficulty->setText("Difficulty: "+dif);
    totalTime->setText("Total Time: "+time);
}

PauseWidget::PauseWidget(QWidget *parent):QWidget(parent) {
    QGridLayout *grid=new QGridLayout;

    returnBtn.setText("Return");
    homeBtn.setText("Home");

    homeBtn.setFixedSize(260,100);
    homeBtn.setAlignment(Qt::AlignCenter);
    homeBtn.setStyleSheet("color:#00303f;\
                           border:2px groove #00303f;\
                           border-radius:50px;\
                           padding:2px 4px;\
                           font-size:40px");

    returnBtn.setFixedSize(260,100);
    returnBtn.setAlignment(Qt::AlignCenter);
    returnBtn.setStyleSheet("color:#00303f;\
                             border:2px groove #00303f;\
                             border-radius:50px;\
                             padding:2px 4px;\
                             font-size:40px");

    grid->addWidget(&returnBtn,1,0,1,1,Qt::AlignCenter);
    grid->addWidget(&homeBtn,1,1,1,1,Qt::AlignCenter);

    this->setLayout(grid);

    QPalette p;
    p.setColor(QPalette::Background,QColor(202,228,219));
    this->setAutoFillBackground(true);
    this->setPalette(p);

}
