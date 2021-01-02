#include <iostream>

#include "parser.h"

//=============================================================================
// Вспомогательные методы
//=============================================================================

//------------------------------------------------------------------------------
// Читает следующий символ из входной строки
void ExportCompiler::nextSym() {
    column++;
    if(!isEnd()) {
        symbol = currentDcl[column];
    } else {
        symbol = -1;
    }
}

// Сохранение текущей позиции для возврата назад
void ExportCompiler::storePos() {
    //oldPos = pos;
    oldSymbol = symbol;
    //oldLine = line;
    oldColumn = column;
}
    
// Восстановление позиции после отката
void ExportCompiler::restorePos() {
    //pos = oldPos;
    symbol = oldSymbol;
    //line = oldLine;
    column = oldColumn;
}

//--------------------------------------------------------------------------
// Пропуск пробельных символов и комментариев
void ExportCompiler::Ignore() {
//_0:
    while(isSymbol(' ') || isSymbol('\t')) {
        nextSym();
        //goto _0;
    }
}

//--------------------------------------------------------------------------
// Выдача сообщения об ошибке
void ExportCompiler::Err(std::string &&s) {
    errCount++;
    std::cout << "[" << line+1 << "," << column+1 << "]: " << s << "\n";
}
