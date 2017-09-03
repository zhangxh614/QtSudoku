#include "undo.h"

SudokuCommand::SudokuCommand(CalWidget *parent,int index,QString oldStr, QString newStr) {
    m=parent;
    m_index=index;
    m_oldStr=oldStr;
    m_newStr=newStr;
}

void SudokuCommand::redo() {
    m->updateBoard(m_index,m_newStr);
}

void SudokuCommand::undo() {
    m->updateBoard(m_index,m_oldStr);
}
