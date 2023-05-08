#include <iostream>

#include "parser.h"

//=============================================================================
// Методы сканера
//=============================================================================

//--------------------------------------------------------------------------
// Идентификатор. Не должен быть ключевым словом
bool ExportCompiler::isId() {
    ///std::cout <<"-->isId\n";
    storePos();
    // Предварительная очистка временного буфера:
    std::string tmpValue{""};
//_0:
    if(isLetter(symbol) || isSymbol('_')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    while(isLetter(symbol) || isDigit(symbol) || isSymbol('_')) {
        tmpValue += symbol;
        nextSym();
    }
    // Cохраняем идентификатор или ключевое слово
    lexValue = tmpValue;
    // Проверка, что это одно из ключевых слов
    if(isOneOfReservedWord(lexValue)) {
        restorePos();
        ///std::cout << "Id: it is key word " << lexValue << " instead identifier!\n";
        return false;
    }
    Ignore();
    return true;
}

//--------------------------------------------------------------------------
// Проверка, что это одно из ключевых слов
// Передаваемый идентификатор сравнивается с ключевыми словами из таблицы
bool ExportCompiler::isOneOfReservedWord(std::string v) {
    for(auto kw: keyWords) {
        if(kw == v) {
            return true;
        }
    }
    return false;
}

//--------------------------------------------------------------------------
// Простое имя, являющееся идентификатором без дополнительных полей
// Отличается от обычного идентификатора тем, что за ним не может идти точка,
// Характеризующая квалифицированный идентификатор
bool ExportCompiler::isSimpleName() {
//_0:
    if(isId()) {
        ///tmpValue +=value;
        goto _1;
    }
    return false;
_1:
    // Точка для простого имени недопустима
    if(isSymbol('.')) {
        Err("SimpleName: Incorrect SimpleName. The Point (.) may be only in Qual Id.");
        return false;
    }
    // Возврат сформированного идентификатора
    ////std::cout << "it's SimpleName --> " << lexValue << std::endl;
    return true;
}

//--------------------------------------------------------------------------
// Квалифицированный идентификатор (идентификаторы, разделенные точками)
// призвольной длины
// Проверка на нужную длину определяется за счет добавления счетчика...
// Допускаются варианты:
// Импортируемое имя
// Поле структуры
// Вложения структур
// Вложение структур и вложений структур в импортируемый артефакт
bool ExportCompiler::isQualId() {
    // Фиксация текущей позиции для возврата при неудаче
    //int startPos = iSim;
    std::string tmpValue{""};      // Предварительная очистка временного буфера
    auto count = 0;          // Счетчик идентификаторов
    qualCount = 0;       // Обнуление на всякий случай
//_0:
    if(isId()) {
        tmpValue += lexValue;
        count++;
        goto _1;
    }
    return false;
_1:
    if(isSymbol('.')) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    goto _end;
_2:
    if(isId()) {
        tmpValue += lexValue;
        count++;
        goto _1;
    }
    Err("QualId: Incorrect Qual Id");
    return false;
_end:
    lexValue = tmpValue;
    qualCount = count;
    return true;
}

//--------------------------------------------------------------------------
// Зарезервированное слово
bool ExportCompiler::isReservedWord(std::string &&v) {
    ///fmt.Println("Reserved:", v, " iSym:", iSym, " Symbol:", symbol)
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym
    ///startSymbol := symbol
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера:
//_0:
    if(isLetter(symbol) || isSymbol('_')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    while(isLetter(symbol) || isDigit(symbol) || isSymbol('_')) {
        tmpValue += symbol;
        nextSym();
    }
    // Cохраняем идентификатор
    lexValue = tmpValue;
    ///fmt.Println("Value:", Value, " vs Reserved:", v)
    // Проверка, что это заданное ключевое слово
    if(lexValue == v) {
        return true;
    }
    // В противном случае откат к началу
    ///iSym = startPos // Восстановление начальной позиции
    ///symbol = startSymbol
    restorePos();
    return false;
}

//--------------------------------------------------------------------------
// Комментарий
bool ExportCompiler::isComment() {
    // Одновременная проверка на многострочный и однострочный комментарий.
    // При этом для многострочного комментария допускается вложенность.
//_0:
    if(isOneStringComment()) {
        goto _end;
    }
    if(isManyStringComment()) {
        goto _end;
    }
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Однострочный комментарий.
bool ExportCompiler::isOneStringComment() {
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym
    //startSymbol := symbol
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
//_0:
    if(isSymbol('/')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    if(isSymbol('/')) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///symbol = startSymbol
    restorePos();
    return false;
_2:
    if(isSymbol('\n') || isEnd()) {
        tmpValue += '\n';
        nextSym();
        goto _end;
    }
    // Любой символ внутри комментария
    tmpValue += symbol;
    nextSym();
    goto _2;
_end:
    lexValue = tmpValue;
    Ignore();
    return true;
}

//--------------------------------------------------------------------------
// Многострочный комментарий.
bool ExportCompiler::isManyStringComment() {
    // Фиксация текущей позиции для возврата при неудаче
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
//_0:
    if(isSymbol('/')) {
        tmpValue += symbol;
        nextSym();
        //fmt.Println("Symbol:", symbol, "-> goto _1")
        goto _1;
    }
    return false;
_1:
    if(isSymbol('*')) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///symbol = startSymbol
    restorePos();
    return false;;
_2:
    if(isSymbol('*')) { // Возможное начало завершения комментария
        tmpValue += symbol;
        nextSym();
        goto _3;
    }
    if(isEnd()) { // Возможный конец текста. Комментарий не закрыт
        Err("ManyStringComment: Incorrect Comment");
        return false;
    }
    if(isManyStringComment()) {
        goto _2;
    }
    // Или любой другой символ, ведущий к заполнению комментария
    tmpValue += symbol;
    nextSym();
    goto _2;
_3:
    if(isSymbol('/')) { // Закрытие комментария
        tmpValue += symbol;
        nextSym();
        goto _end;
    }
    // Иной символ. Поэтому смотрим на следующий.
    tmpValue += symbol;
    nextSym();
    goto _2;
_end:
    lexValue = tmpValue;
    Ignore();
    return true;
}

//--------------------------------------------------------------------------
// Левое обозначение
bool ExportCompiler::isLeftAssign() {
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym;
    ///startSymbol := Symbol
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
// 0:
    if(isSymbol('<')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    if(isSymbol('<')) {
        tmpValue += symbol;
        nextSym();
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    return false;
_end:
    lexValue = tmpValue;
    return true;
}

//--------------------------------------------------------------------------
// Правое обозначение
bool ExportCompiler::isRightAssign() {
    // Фиксация текущей позиции для возврата при неудаче
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
// 0:
    if(isSymbol('>')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    if(isSymbol('>')) {
        tmpValue += symbol;
        nextSym();
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    return false;
_end:
    lexValue = tmpValue;
    return true;
}

//--------------------------------------------------------------------------
// Разделитель аргумента и результата
bool ExportCompiler::isArrow() {
    // Фиксация текущей позиции для возврата при неудаче
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
// 0:
    if(isSymbol('-')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    return false;
_1:
    if(isSymbol('>')) {
        tmpValue += symbol;
        nextSym();
        goto _end;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    return false;
_end:
    lexValue = tmpValue;
    return true;
}

//--------------------------------------------------------------------------
// Булева константа true | false
bool ExportCompiler::isBoolean() {
    ///fmt.Println("Boolean")
    // Фиксация текущей позиции для возврата при неудаче
    ///startPos := iSym;
    ///startSymbol := Symbol
    storePos();

    //if(isReservedWord("false") || isReservedWord("true")) {
    if(isReservedWord("false")) {
        Ignore();
        return true;
    }
    if(isReservedWord("true")) {
        Ignore();
        return true;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    return false;
}

//--------------------------------------------------------------------------
// Целочисленная константа со знаком или без знака
bool ExportCompiler::isInteger() {
    //fmt.Println("-->isInteger")
    // Фиксация текущей позиции для возврата при неудаче
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
    std::string oneSym{""};
// 0:
    //fmt.Println("    i->", symbol)
    if(isSymbol('-') || isSymbol('+')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    return false;
_1:
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    // При откате назад сообщение об ошибке не нужно. Пробуется другая версия.
    // ErrorOut("Integer: Incorrect symbol after sign. Digit wated.");
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    return false;
_2:
    // Накопление цифр целого числа
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    // Символы, которые не должны появляться после целого числа
    if(isLetter(symbol) || isSymbol('_') || isSymbol('.')) {
        //iSim = startPos; // Восстановление начальной позиции
        Err("Integer: Incorrect symbol '" + (oneSym = symbol) + "' after digits!");
        return false;
    }
    goto _end;
_end:
    lexValue = tmpValue;
    Ignore();
    return true;
}

//--------------------------------------------------------------------------
// Действительная константа со знаком или без знака
bool ExportCompiler::isFloat() {
    // Фиксация текущей позиции для возврата при неудаче
    storePos();
    std::string tmpValue{""};      // Предварительная очистка временного буфера
    std::string oneSym{""};
// 0:
    //fmt.Println("    f->", Symbol)
    if(isSymbol('-') || isSymbol('+')) {
        tmpValue += symbol;
        nextSym();
        goto _1;
    }
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    return false;
_1:
    // Обязательная первая цифра после знака
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    ///iSym = startPos     // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    // ErrorOut("Real: Incorrect symbol after sign. Digit wated.");
    return false;
_2:
    // Накопление цифр целой части числа
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _2;
    }
    // Точка, отделяющая целую часть от дробной
    if(isSymbol('.')) {
        tmpValue += symbol;
        nextSym();
        goto _3;
    }
    // Начало Порядка числа
    if(isSymbol('E') || isSymbol('e')) {
        tmpValue += symbol;
        nextSym();
        goto _5;
    }
    ///iSym = startPos // Восстановление начальной позиции
    ///Symbol = startSymbol
    restorePos();
    // При откате назад сообщение об ошибке не нужно. Пробуется другая версия.
    //ErrorOut("Real: Incorrect Real Number.");
    return false;
_3:
    // Первая (обязательная) цифра дробной части
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _4;
    }
_4:
    // Накопление цифр дробной части числа
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _4;
    }
    // Начало Порядка числа
    if(isSymbol('E') || isSymbol('e')) {
        tmpValue += symbol;
        nextSym();
        goto _5;
    }
    // Символы, которые не должны появляться после числа
    if(isLetter(symbol) || isSymbol('_') || isSymbol('.')) {
        // iSim = startPos; // Восстановление начальной позиции
        Err("Integer: Incorrect symbol" + (oneSym = symbol) + " after digits");
        return false;
    }
    goto _end;  // Произошло накопление действительного числа без порядка
_5:
    // Возможен знак перед порядком
    if(isSymbol('-') || isSymbol('+')) {
        tmpValue += symbol;
        nextSym();
        goto _6;
    }
    // startSymbolЗнак порядка отсутствует
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _7;
    }
_6:
    // Обязательная первая цифра после знака порядка
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _7;
    }
    // iSim = startPos; // Восстановление начальной позиции
    Err("Real: Incorrect symbol " + (oneSym = symbol) + " after sign. Digit wated");
    return false;;
_7:
    // Накопление цифр порядка
    if(isDigit(symbol)) {
        tmpValue += symbol;
        nextSym();
        goto _7;
    }
    // Символы, которые не должны появляться после числа
    if(isLetter(symbol) || isSymbol('_') || isSymbol('.')) {
        // iSim = startPos; // Восстановление начальной позиции
        Err("Integer: Incorrect symbol " + (oneSym = symbol) + " after digits.");
        return false;
    }
    goto _end;  // Произошло накопление действительного числа
_end:
    lexValue = tmpValue;
    Ignore();
    return true;
}
