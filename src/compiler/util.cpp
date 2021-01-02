#include <iostream>

#include "parser.h"

//=============================================================================
// Вспомогательные методы
//=============================================================================

//------------------------------------------------------------------------------
// Читает следующий символ из входной строки
void Compiler::nextSym() {
    if(isSymbol('\n')) {
        line++;
        column = 1;
    } else {
        column++;
    }
    pos++;
    if(!isEnd()) {
        symbol = artefact[pos];
    } else {
        symbol = -1;
    }
}

// Сохранение текущей позиции для возврата назад
void Compiler::storePos() {
    oldPos = pos;
    oldSymbol = symbol;
    oldLine = line;
    oldColumn = column;
}
    
// Восстановление позиции после отката
void Compiler::restorePos() {
    pos = oldPos;
    symbol = oldSymbol;
    line = oldLine;
    column = oldColumn;
}

//--------------------------------------------------------------------------
// Пропуск пробельных символов и комментариев
void Compiler::Ignore() {
_0:
    if(isSymbol(' ') || isSymbol('\n')) {
        nextSym();
        goto _0;
    } else if(isComment()) {
        goto _0;
    }
}

//--------------------------------------------------------------------------
// Выдача сообщения об ошибке
void Compiler::Err(std::string &&s) {
    errCount++;
    std::cout << "[" << line+1 << "," << column+1 << "]: " << s << "\n";
}
