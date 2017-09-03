#include "button.h"
#include <QColor>
#include <QDebug>

Button::Button(QWidget *parent):QLabel(parent) {
    resetFont(30);

    checkable=false;
    checked=false;
}

Button::Button(const QString &text, QWidget *parent):QLabel(parent) {
    setText(text);
    resetFont(30);

    checkable=false;
    checked=false;
}

void Button::mouseReleaseEvent(QMouseEvent *e) {
    if(e->button()==Qt::LeftButton) {
        checked=!checked;
        emit clicked();
    }
}

void Button::setBaseColor(int r, int g, int b, int rr, int gg, int bb) {
    QPalette p;
    p.setColor(QPalette::Background,QColor(r,g,b));
    p.setColor(QPalette::WindowText,QColor(rr,gg,bb));
    this->setAutoFillBackground(true);
    this->setPalette(p);
}

bool Button::isCheckable() {
    return checkable;
}

void Button::setCheckable(bool a) {
    checkable=a;
}

bool Button::isChecked() {
    return checked;
}

void Button::setChecked(bool a) {
    checked=a;
}

bool Button::updateText(QString a) {
    int len=this->text().length();
    if(len>=10) return false;
    //qDebug()<<len;
    if(len==0) {
        this->setText(a);
    } else {
        QString text=this->text();
        for(int i=0;i<len;i++) {
            if(a==QString(text.at(i)))
                return false;
        }
        resetFont(15);
        if(len==4) {
            text.append("\n").append(a);
        } else {
            text.append(a);
        }
        this->setText(text);
    }
    if(len!=this->text().length())
        return true;
    return false;
}

void Button::resetFont(int size) {
    QFont ft;
    ft.setPointSize(size);
    this->setFont(ft);
}
