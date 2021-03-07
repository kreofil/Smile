#include <iostream>

#include "parser.h"

//-----------------------------------------------------------------------------
// Компиляция файла экспорта в другие пакеты
void ExportCompile(std::vector<std::string> &e);

//-----------------------------------------------------------------------------
// Список ключевых и зарезервированных слов,
// которые не могут использоваться в качестве идентификатора
//std::vector<std::string> Compiler::keyWords;
std::vector<std::string> Compiler::keyWords {
    "array",
    "const",
    "func",
    "safe",
    "stream",
    "struct",
    "swarm",
    "var",
    "vector",
    "type",
};

//-----------------------------------------------------------------------------
// Начальные установки параметров компилятора и его запуск
bool Compile(std::string &a, SemanticModel* sm) {
    Compiler c(a, *sm);
    std::cout << "***** ARTEFACT *****" << std::endl;
    std::cout << a << std::endl;

    std::cout << "***** COMPILER STARTED *****" << std::endl;

    if(c.isArtefact()) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
        return false;
    }

    sm->DebugOut();
    return true;    // while
}


//-----------------------------------------------------------------------------
// Конструктор компилятора
Compiler::Compiler(std::string &a, SemanticModel &m): 
        artefact{a}, pos{0}, line{0}, column{1}, 
        errCount{0}, qualCount{0}, sm(m)
{
    symbol = artefact[0];
}    

//=============================================================================
// Начальный нетерминал
//=============================================================================

//-----------------------------------------------------------------------------
//! Artefact = ConstDefinition | FuncDefinition | ProtoDefinition
//!          | TypeDefinition  | ImportDefinition
bool Compiler::isArtefact() {
    Declaration* pdcl = nullptr;
    Func* pf = nullptr;
    std::string artefactName;
    Ignore();   // пропуск пробельных символов и комментариев перед первой лексемой
//_0: Проверка на имя артефакта или начало его определения
    // В начале идет проверка на обозначение артефакта
    if(isSimpleName()) {
        artefactName = lexValue; // Фиксация имени артефакта
        //smodel.SetArtefactName() // Фиксация имени артефакта
        //smodel.RemoveDeclaration()    // Удаление описания из таблицы описаний
        Ignore(); // пропуск пробельных символов и комментариев
        goto _1;
    }
    // Далее пошли варианты с описанием альтернативных артефактов
    // с постфиксным обозначением
    if(isConstDefinition(&pdcl)) {
        goto _3;
    }
    if(isFuncDefinition(&pdcl, &pf)) {
        sm.SetFuncDefinition(pf);
        goto _3;
    }
    //if(ProtoDefinition()) {
    //    goto _3;
    //}
    if(isTypeDefinition(&pdcl)) {
        goto _3;
    }
    if(isImportDefinition()) {
        goto _3;
    }
    Err("Artefact: It is not artefact");
    return false;
_1: // Проверка на лексему левого обозначения.
    if(isLeftAssign()) {
        Ignore();
        goto _2;
    }
    Err("Artefact: It is not Left Assignment (<<)");
    return false;
_2:
    // Далее пошли варианты с описанием альтернативных артефактов
    // после префиксного обозначения
    if (isConstDefinition(&pdcl)) {
        // Добавление артефакта в таблицу описаний (или замена старого артефакта)
        //smodel.ExportNameTab[artefactName] = *context
        goto _5;
    }
    if(isFuncDefinition(&pdcl, &pf)) {
        sm.SetFuncDefinition(pf);
        goto _5;
    }
    //if(isProtoDefinition()) {
    //    goto _5;
    //}
    if(isTypeDefinition(&pdcl)) {
        goto _5;
    }
    if(isImportDefinition()) {
        goto _5;
    }
    Err("Artefact: It is not artefact");
    return false;
_3: // Должно быть постфиксное обозначение
    if(isRightAssign()) {
        Ignore();
        goto _4;
    }
    Err("Artefact: It is not Right Assignment (>>)");
    return false;
_4: // И завершающее имя...
    if(isSimpleName()) {
        //smodel.SetArtefactName() // Фиксация имени артефакта
        //smodel.RemoveDeclaration()    // Удаление описания из таблицы описаний
        artefactName = lexValue;
        Ignore();
        goto _5;
    }
_5: // В конце возможна точка с запятой
    if(isSymbol(';')) {
        nextSym();
        Ignore();
        goto _6;
    }
    goto _6;
_6: // Завершение просмотра на конец строки
    if(isEnd()) {
        goto _end;
    }
    Err("Artefact: Incorrect symbols at  the end of Artefact");
    return false;
_end:
    Ignore();   // пропуск пробельных символов и комментариев
    // Добавление имени артефакта в семантическую модель
    sm.SetArtefactName(artefactName);
    // Добавление артефакта в таблицу описаний (или замена старого артефакта)
    pdcl->SetName(artefactName);
    if(sm.AddToExportNameTable(pdcl)) {
        return true;
    }
    Err("Artefact: Incorrect extention of export table");
    return false;
}

//=============================================================================
// Методы парсера
//=============================================================================

//--------------------------------------------------------------------------
// Определение константы
bool Compiler::isConstDefinition(Declaration** ppdcl) {
    Const* pcv;
//_0: Проверка текущей лексемы на ключевое слово const
    if(isReservedWord("const")) {
        Ignore();
        goto _1;
    }
    return false;
_1: // Проверяется корректность константного выражения
    if(isConstExpression(&pcv)) {
        *ppdcl = sm.newDeclarationConst(pcv);
        goto _end;
    }
    Err("ConstDefinition: ConstExpression wated");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение функции
bool Compiler::isFuncDefinition(Declaration** ppdcl, Func** ppf) {
    std::string argName{""};
    //std::string argTypeName{""};
    //std::string resTypeName{""};
    Type* pta = nullptr;  // Указатель на тип аргумента
    Type* ptr = nullptr;  // Указатель на тип результата
    DeclarationType* adt = nullptr; // Указатель на объявление аргумента
    DeclarationType* rdt = nullptr; // Указатель на объявление результата
    Func* pf = nullptr;
//_0: Проверка текущей лексемы на ключевое слово isConstDefinitionfunc
    if(isReservedWord("func")) {
        // В принципе после этого шага можно начинать формирование описания функции
        pf = sm.NewFunc();
        *ppf = pf;
        Ignore();
        goto _1;
    }
    return false;
_1: // Проверяется наличие идентификатора аргумента
    if (isId()) {
        // В принципе локальное имя может быть любым кроме ключевых слов
        argName = lexValue;
        pf->SetArgTypeName(argName);
        Ignore();
        goto _2;
    }
    if(isArrow()) {
        // Отсутствует аргумент. Поэтому нужно сгенерировать на выходе объявление сигнала.
        std::string argTypeName{"signal"};
        /////adt = &dclSignal("signal", &typeSignal);
        
        adt = sm.FindTypeDeclaration(argTypeName); // Поиск именованного типа
        Ignore();
        goto _6;
    }
    Err("FuncDefinition: Argument declaration or arrow wated");
    return false;
_2: // Проверка на наличие типа аргумента
    if(isSymbol('@')) {
        nextSym();
        Ignore();
        goto _3;
    }
    // Здесь также возможен кортеж
    //...
    Err("FuncDefinition: Incorrect declaration of argument type");
    return false;
_3: // Проверка наличия имени типа аргумента
    if(isQualId()) {
        // Поиск соответствующего имени и анализ его на имя типа
        /// Пока ограничимся поиском в том же пакете
        //argTypeName = lexValue; 
        adt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore();
        goto _4;
    }
    if(isTuple(&pta)) { // кортеж
        adt = sm.newDeclarationType(pta); // Создается неименованный кортеж
        Ignore();
        goto _4;
    }
    Err("FuncDefinition: Incorrect declaration of argument type");
    return false;
_4: // Проверка разделителя аргумента и результата
    if(isArrow()) {
        Ignore();
        goto _6;
    }
    Err("FuncDefinition: arrow after argument wated");
    return false;
_6: // Проверка имени типа
    if(isQualId()) {
        // Фиксация типа результата по факту
        //resTypeName = lexValue; 
        rdt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore();
        // К данному моменту получены все данные для формирования прототипа
        goto _7;
    }
    if(isTuple(&ptr)) { // кортеж
        rdt = sm.newDeclarationType(ptr); // Создается неименованный кортеж
        Ignore();
        goto _7;
    }
    Err("FuncDefinition: Incorrect declaration of result type");
    return false;
_7: 
    pf->SetArgType(adt->GetType());
    pf->SetResType(rdt->GetType());
    // Можно установить объявления в акторах аргумента и возврата
    // Формирование обязательной начальной вершины РИГ, фиксирующей объявление аргумента
    // Всегда располагается в нулевой позиции вектора вершин
    pf->AddActor(new ActorArg{adt, 0});
    // Добавление обязательной вершины возврата из функции.
    // Она следует за начальной вершиной.
    pf->AddActor(new ActorRet{rdt, 1});
    // Имя необходимо занести в локальную таблицу имен, а также установить его объявление 
    // Для хранения используется объявление актора, связанного с данным именем.
    // В данном случае это актор аргумента функции.
    DeclarationActor* pda = new DeclarationActor{argName, pf->GetActor(0)};
    pf->AddToLocalNameTable(pda);
    // Или тело функции или завершение неполного описания
    if(isBody(*pf)) {
        goto _8;
    }
    if(isSymbol(';')) {
        Ignore();
        goto _end;
    }
    // Неполное описание можно завершить без точки с запятой
    goto _end;
_8:
    // Полное описание можно завершить без точки с запятой
    if (isSymbol(';')) {
        Ignore();
        goto _end;
    }
    goto _end;
_end:
    // Формирование прототипа функции без увязки с уже существующими прототипами.
    FuncProto* funcProto = sm.NewFuncProto();
    if((adt != nullptr)&&(rdt != nullptr)) {
        // Выполняется добавление сигнатуры в список сигнатур
        funcProto->AddSig(Signature(adt, rdt));
    } else {
        Err("FuncDefinition: Incorrect type name of argument or result");
        return false;
    }
    *ppdcl = sm.newDeclarationFuncProto(funcProto);

    //pf->DebugOut();
    return true;
}

// Тело функции
bool Compiler::isBody(Func& f) {
    ////Func* pf = nullptr;
//_0: Проверка текущей лексемы на ключевое слово type
    if(isSymbol('{')) { // Начало тела функции
        nextSym();
        Ignore();
        goto _1;
    }
    return false;
_1:
    // Проверка на базовый именованный тип
    if(isElement(f)) {
        Ignore();
        goto _2;
    }
    Err("isBody: A element of body was expected");
    return false;    
_2:
    if(isSymbol(';')) {
        nextSym();
        Ignore();
        goto _3;
    }
    if(isSymbol('}')) {
        nextSym();
        Ignore();
        goto _end;
    }
    Err("isBody: ';' or '}' was expected");
    return false;    
_3:
    if(isSymbol('}')) {
        nextSym();
        Ignore();
        goto _end;
    }
    // Проверка на базовый именованный тип
    if(isElement(f)) {
        Ignore();
        goto _2;
    }
    Err("isBody: element of body or '}' was expected");
    return false;
_end:
    return true;
}

// Элемент тела функции
bool Compiler::isElement(Func& f) {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    ///auto erFlag = false;     // флаг для отката или ошибки
//_0:
    if(isId()) { // Левое обозначение выражения
        Ignore();
        goto _1;
    }
    if(isExpression(f)) { // Выражение
        Ignore();
        goto _3;
    }
    return false;
_1:
    if(isLeftAssign()) { // Левое обозначение
        Ignore();
        goto _2;
    }
    // Здесь возможен откат назад для того, чтобы перейти к анализу 
    // Выражения без левого обозначения (но, возможно, с правым обозначением)
    pos = ipos;
    symbol = isymbol;
    line = iline;
    column = icolumn;
    if(isExpression(f)) { // Выражение
        Ignore();
        goto _3;
    }
    Err("Element: It is not Left Assignment (<<)");
    return false;
_2:
    if(isExpression(f)) { // Выражение
        Ignore();
        goto _end;
    }
    Err("Element: Expression was expected");
    return false;
_3:
    if(isRightAssign()) { // Левое обозначение
        Ignore();
        goto _4;
    }
    goto _end;  // Выражение без обозначений возможны
_4:
    if(isId()) { // Правое обозначение выражения
        Ignore();
        goto _end;
    }
    Err("Element: Right Name of Expression was expected");
    return false;
_end:
    return true;
}

// Выражение в теле функции
bool Compiler::isExpression(Func& f) {
    Actor* aLeft = nullptr;
    Actor* aRight = nullptr;
    ActorInterpret* pai1 = nullptr;
    ActorInterpret* pai2 = nullptr;
//_0:
    if(isTerm(f, &aLeft)) { // Терм
        Ignore();
        goto _1;
    }
    return false;
_1:
    if(isSymbol(':')) {
        // Можно начать формирование одноаргументного оператора интерпретации
        pai1 = new ActorInterpret{f.ActorNumber()};
        f.AddActor(pai1);
        pai1->SetArg(aLeft);
        nextSym();
        Ignore();
        goto _2;
    }
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore();
        goto _3;
    }
    Err("Expression: Prefix Expression or ':' were expected");
    return false;
_2:
    if(isTerm(f, &aRight)) { // Терм
        pai1->SetFunc(aRight);
        Ignore();
        goto _3;
    }
    Err("Expression: Term was expected");
    return false;
_3:
    if(isSymbol(':')) {
        // Можно начать формирование одноаргументного оператора интерпретации
        pai2 = new ActorInterpret{f.ActorNumber()};
        f.AddActor(pai2);
        pai2->SetArg(pai1);
        pai1 = pai2;
        nextSym();
        Ignore();
        goto _2;
    }
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore();
        goto _3;
    }
    goto _end;
_end:
    return true;
}

// Префиксное подвыражение в теле функции
bool Compiler::isPrefixExpression(Func& f) {
    ///Actor* aLeft = nullptr;
    Actor* aRight = nullptr;
//_0:
    if(isSymbol('^')) {
        nextSym();
        Ignore();
        goto _1;
    }
    return false;
_1:
    if(isTerm(f, &aRight)) { // Терм
        Ignore();
        goto _2;
    }
    Err("Prefix Expression: Term was expected");
    return false;
_2:
    if(isPrefixExpression(f)) { // Префиксный оператор интерпретации
        Ignore();
        goto _end;
    }
    goto _end;
_end:
    return true;
}

// Терм
bool Compiler::isTerm(Func& f, Actor** ppa) {
    Const* pcv;
    DeclarationFunc* pdf;
//_0:
    if(isReservedWord("return")) {
        // Актор возврата уже существует и стоит на второй позиции.
        // Поэтому его формировать не нужно. Но необходимо передать оператору интерпретации
        *ppa = f.GetActor(1);
        Ignore();
        goto _end;
    }
    if(isBaseFunc(&pdf)) {
        // Создание актора, определяющего функцию для базовой функции
        ActorFunc* paf = new ActorFunc{pdf, f.ActorNumber()};
        f.AddActor(paf);
        *ppa = paf;
        Ignore();
        goto _end;
    }
    if(isAtom(&pcv)) {
        // Создание константного актора, который добавляется в список акторов
        Declaration* pdc = sm.newDeclarationConst(pcv);
        ActorConst* pac = new ActorConst{pdc, f.ActorNumber()};
        f.AddActor(pac);
        *ppa = pac;
        Ignore();
        goto _end;
    }
    if(isQualId()) {
        // Некоторое имя, которое может находиться в одной из таблиц имен
        auto name = lexValue;
        if(DeclarationActor* pda = f.FindLocalDeclaration(name); pda != nullptr) {
            // Имя является обозначением актора, на который ссылается.
            *ppa = pda->GetActor();
        }
        Ignore();
        goto _end;
    }
    return false;
_end:
    return true;
}

/*
//--------------------------------------------------------------------------
// Определение прототипа функции.
bool Compiler::isProtoDefinition() {

    return false;
}
*/

//--------------------------------------------------------------------------
// Определение типа.
bool Compiler::isTypeDefinition(Declaration** ppdcl) {
    Type* ptv;
//_0: Проверка текущей лексемы на ключевое слово type
    if(isReservedWord("type")) {
        Ignore();
        goto _1;
    }
    return false;
_1:
    // Проверка на базовый именованный тип
    if(isBaseType(&ptv)) {
        Ignore();
        goto _end;
    }
    // Проверка на вектор
    if(isArray(&ptv)) {
        Ignore();
        goto _end;
    }
    // Проверка на структуру
    if(isStruct()) {
        Ignore();
        goto _end;
    }
    // Проверка на кортеж
    if(isTuple(&ptv)) {
        Ignore();
        goto _end;
    }
    // Проверка на рой (толпу)
    if(isSwarm(&ptv)) {
        Ignore();
        goto _end;
    }
    // Именованный тип (идентификатор или квалифицированны ид. на два имени
    // Одно из определений должно быть именованным типом
    //...
    Err("isTypeDefinition: A type definition have been expected");
    return false;    
_end:
    *ppdcl = sm.newDeclarationType(ptv);
    return true;
}

//--------------------------------------------------------------------------
// Определение базового типа.
bool Compiler::isBaseType(Type **pptv) {
//_0: 
    if(isReservedWord("signal")) { // сигнальный тип
        *pptv = sm.GetTypeSignal();
        Ignore();
        goto _end;
    }
    if(isReservedWord("int")) { // целочисленный тип
        *pptv = sm.GetTypeInt();
        Ignore();
        goto _end;
    }
    if(isReservedWord("bool")) { // булевский тип
        *pptv = sm.GetTypeBool();
        Ignore();
        goto _end;
    }
    if(isReservedWord("float")) { // действительный тип
        *pptv = sm.GetTypeFloat();
        Ignore();
        goto _end;
    }
    // Остальные будут дописаны позднее
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение вектора.
// Vector = "(" [Integer] ")" TypeName
// TypeName = BaseType | QualId
bool Compiler::isArray(Type **pptv) {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0: 
    if(isSymbol('(')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isInteger()) { // размерность вектора
        Ignore();
        erFlag = true;
        goto _2;
    }
/*
    if(isSymbol(')')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
*/
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err("isArray: Incorrect size of array");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;    
_2:
    if(isSymbol(')')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
    if(isSymbol(',')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    Err("isArray: ')' have been expected");
    return false;    
_3:
    if(isBaseType(pptv)) {
        Ignore();
        goto _end;
    }
    // Остальные типы будут дописаны позднее
    Err("isArray: vector type must have type of elements");
    return false;    
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение именованной структуры.
bool Compiler::isStruct() {
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0:    
    if(isSymbol('(')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isSymbol('{')) {
        nextSym();
        Ignore();
        erFlag = true;
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _2;
    }
    if(isId()) { // 
        Ignore();
        goto _4;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err("isStruct: Incorrect name list");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;    
_2:
    if(isId()) { // 
        Ignore();
        goto _3;
    }
    Err("isStruct: Expected name of field");
    return false;
_3:    
    if(isSymbol(',')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _2;
    }
    if(isSymbol('}')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _4;
    }
    Err("isStruct: Expected '}' or','");
    return false;
_4:
    if(isSymbol('@')) {
        nextSym();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        erFlag = true;
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err("isStruct: Expected '@'");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;    
_5:
    if(isQualId()) { // 
        Ignore();
        goto _6;
    }
    if(isId()) { // 
        Ignore();
        goto _6;
    }
    Err("isStruct: Expected type name");
    return false;
_6:
    if(isSymbol(',')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    if(isSymbol(')')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _end;
    }
    Err("isStruct: Expected ',' or')'");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение кортежа.
bool Compiler::isTuple(Type **pptv) {
    ////auto fieldCounter = 0;  // Счетчика повторяющихся полей
    // Вектор для накопления внутренних объявлений кортежа
    std::vector<DeclarationType*> tmpVec;
    // Сохранение позиции для возможного отката назад
    auto ipos{pos};
    auto isymbol{symbol};
    auto iline{line};
    auto icolumn{column};
    auto erFlag = false;     // флаг для отката или ошибки
//_0:    
    if(isSymbol('(')) {
        nextSym();
        Ignore();
        goto _1;
    }
    return false;
_1:
    if(isQualId()) { // Фиксирует как обычный ид, так и квалифицированный (по счетчику)
        // Проверка на то, что это объявление типа
        DeclarationType* pdt = sm.FindTypeDeclaration(lexValue);
        if(pdt != nullptr) {
            // Тип добавляется к временному списку типов кортежа
            tmpVec.push_back(pdt);
        }
        Ignore();
        erFlag = true;
        goto _5;
    }
    if(isId()) { // Локальный тип
        Ignore();
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err("isTuple: typeName or expected '('");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;    
_2:
    if(isInteger()) { // несколько однотипных полей
        Ignore();
        goto _3;
    }
    Err("isTuple: Expected integer (number of same fields)");
    return false;
_3:
    if(isSymbol(')')) {
        nextSym();
        Ignore();
        goto _4;
    }
    Err("isTuple: Expected ')'");
    return false;
_4:
    if(isQualId()) { // Импортируемый тип
        Ignore();
        goto _5;
    }
    if(isId()) { // Локальный тип
        Ignore();
        goto _5;
    }
    // Откат или сообщение об ошибке
    if(erFlag) {
        Err("isTuple: Expected type name");
    } else {
        // Если не целое, то возможна структура. Откат назад
        pos = ipos;
        symbol = isymbol;
        line = iline;
        column = icolumn;
    }
    return false;    
_5:
    if(isSymbol(',')) {
        nextSym();
        Ignore();
        goto _1;
    }
    if(isSymbol(')')) {
        nextSym();
        Ignore();
        goto _end;
    }
    Err("isStruct: Expected ',' or')'");
    return false;
_end:
    // Формирование кортежа
    TypeTuple* typeTuple = sm.NewTuple();
    // Заполнение полей кортежа
    for(auto it: tmpVec) {
        typeTuple->AddType(it);
    }
    *pptv = typeTuple;
    return true;
}

//--------------------------------------------------------------------------
// Определение роя.
bool Compiler::isSwarm(Type** pptv) {
//_0: 
    if(isSymbol('[')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _1;
    }
    return false;
_1:
    if(isInteger()) { // размерность роя
        Ignore();
        goto _2;
    }
    Err("isSwarm: A size of swarm type have been expected");
    return false;    
_2:
    if(isSymbol(']')) {
        nextSym();
        Ignore();
        ///fmt.Print("   ->'", string(Symbol), "'\n")
        goto _3;
    }
    Err("isSwarm: swarm type must have symbol ']'");
    return false;    
_3:
    if(isBaseType(pptv)) {
        Ignore();
        goto _end;
    }
    // Остальные типы будут дописаны позднее
    Err("isSwarm: swarm type must have type of elements");
    return false;    
_end:
    return true;
}

//--------------------------------------------------------------------------
// Определение импорта.
bool Compiler::isImportDefinition() {

    return false;
}

//--------------------------------------------------------------------------
// Определение константного выражения.
bool Compiler::isConstExpression(Const** ppcv) {
//_0:
    if(isConstTerm(ppcv)) {
        goto _1;
    }
    return false;
_1:
    if(isSymbol(':')) {
        nextSym();
        Ignore();
        goto _2;
    }
    if(isSymbol('^')) {
        nextSym();
        Ignore();
        goto _3;
    }
    goto _end;
_2:
    if(isConstTerm(ppcv)) {
        goto _1;
    }
    Err("ConstExpression: A constant term have been expected");
    return false;
_3:
    if(isConstExpression(ppcv)) {
        goto _end;
    }
    Err("ConstExpression: A constant expression have been expected");
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Константный терм, содержащий константы и константные операции
bool Compiler::isConstTerm(Const** ppcv) {
    ///fmt.Println("ConstTerm")
//_0:
    if(isAtom(ppcv)) {
        goto _end;
    }
    /// Остальные варианты термов будут доопределяться по ходу разработки
    /// ConstDataList
    /// ConstParList
    /// String
    return false;
_end:
    return true;
}

//--------------------------------------------------------------------------
// Атомарная константа
bool Compiler::isAtom(Const** ppcv) {
    Const* pcv;
    ///fmt.Println("Atom")
    //ok := false
    if(isSymbol('!')) { // Сигнальная константа
        nextSym();
        Ignore();
        pcv = sm.NewSignalValue();
        goto _end;
    }
    if(isBoolean()) {
        //! Формирование значения булевой константы в виде элемента семантической модели
        bool bv;
        if(lexValue == "true") {
            bv = true;
        } else if(lexValue == "false") {
            bv = false;
        }
        // Формирование значения для данной константы
        pcv = sm.NewBoolValue(bv);
        goto _end;
    }
    if(isFloat()) {
        double fv = std::stod(lexValue);
        // Формирование значения для данной константы
        pcv = sm.NewFloatValue(fv);
        // Формирование значения действительной константы в виде элемента семантической модели
        //fv, _ := strconv.ParseFloat(Value, 64)
        // Формирование значения для данной константы
        //floatValue := smodel.NewFloatValue(fv)
        // Тестовый вывод константы как значения
        //fmt.Println("Float const floatValue (isAtom):", floatValue)
        //* = floatValue
        // Тестовый вывод константы как значения
        //fmt.Println("Float const  (isAtom):", *)
        goto _end;
    }
    if(isInteger()) {
        int iv = std::stoi(lexValue);
        // Формирование значения для данной константы
        pcv = sm.NewIntValue(iv);
        // Формирование значения целочисленной константы в виде элемента семантической модели
        //iv, _ := strconv.Atoi(Value)
        // Формирование значения для данной константы
        //intValue := smodel.NewIntValue(iv)
        // Тестовый вывод константы как значения
        //fmt.Println("Int const intValue (isAtom):", intValue)
        //* = intValue
        // Тестовый вывод константы как значения
        //fmt.Println("Int const  (isAtom):", *)
        goto _end;
    }
    /*
    if(isChar()) {
        // Формирование значения целочисленной константы в виде элемента семантической модели
        cv := rune(Value)
        // Формирование значения для данной константы
        charValue := smodel.NewIntValue(iv)
        // Тестовый вывод константы как значения
        fmt.Println("Int const intValue (isAtom):", intValue)
        * = intValue
        // Тестовый вывод константы как значения
        fmt.Println("Int const  (isAtom):", *)
        goto _end;
    }
    //fmt.Println("Symbol ==", string(Symbol))
    //!!!! Возникла мысль о том, что не имеет смысла использовать сигналы как именованные константы,
    //!!!! Так как они не имеют альтернативных значений. Поэтому их можно вынести из константных величин
    if(isSymbol('!')) { // Пустое (сигнальное) значение
        //*v = new ValueSignal()
        nextSym();
        Ignore();
        goto _end;
    }
    */
    /// Остальные варианты атомов будут доопределяться по ходу разработки
    /// SpecSymbol
    /// NamedConst
    /// IntervalConst
    return false;
_end:
    *ppcv = pcv;
    return true;
}

//--------------------------------------------------------------------------
// Константный вектор (список данных)
bool Compiler::isConstDataList() {
    return false;
}

//--------------------------------------------------------------------------
// Константная группа (параллельный список)
bool Compiler::isConstParList() {
    return false;
}

//--------------------------------------------------------------------------
// Строка символов
//! Пока заморожена, так как символы не входят в основные типы данных.
bool Compiler::isString() {
    return false;
}
