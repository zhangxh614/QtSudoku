#ifndef BUTTON_H
#define BUTTON_H

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>

class Button:public QLabel {

    Q_OBJECT

private:
    bool checkable;
    bool checked;

public:
    explicit Button(QWidget *parent=0);
    Button(const QString &text,QWidget *parent=0);
    void setBaseColor(int r,int g,int b,int rr,int gg,int bb);
    bool isCheckable();
    void setCheckable(bool a);
    bool isChecked();
    void setChecked(bool a);
    bool updateText(QString a);
    void resetFont(int);

protected:
    void mouseReleaseEvent(QMouseEvent *);

signals:
    void clicked();
};

#endif // BUTTON_H
