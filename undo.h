#ifndef UNDO_H
#define UNDO_H

#include "subwidget.h"

#include <QUndoCommand>

class SudokuCommand:public QUndoCommand {
public:
    explicit SudokuCommand(CalWidget *parent,int old,QString oldStr,QString freshStr);
    void redo();
    void undo();
private:
    CalWidget* m;
    int m_index;
    QString m_oldStr,m_newStr;
};

#endif // UNDO_H
