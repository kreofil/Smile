#include <iostream>
#include <cmath>

#include "parser.h"

//-----------------------------------------------------------------------------
// Список ключевых и зарезервированных слов,
// которые не могут использоваться в качестве идентификатора
//std::vector<std::string> ExportCompiler::keyWords;
std::vector<std::string> ExportCompiler::keyWords{
        "array",
        "const",
        "func",
        "safe",
        "struct",
        "swarm",
        "tuple",
        "type",
        "var",
};

//-----------------------------------------------------------------------------
// Компиляция файла экспорта в другие пакеты
bool ExportCompile(std::vector<std::string> &e, SemanticModel* sm) {
    ExportCompiler ec(*sm);  // Создается компилятор для распознавания экспорта

/*    
    // Тестовый вывод файла экспорта
    std::cout << "***** EXPORT *****" << std::endl;
    for(auto str: e) {
        std::cout << str << std::endl;
    }
*/    
    std::cout << "***** EXPORT COMPILER STARTED *****" << std::endl;

    // Запускается обход строк файла экспорта
    for(size_t i = 0; i < e.size(); i++) {
        ec.setLine(e[i], i);
        if(ec.isExport()) {
            continue;
        } else {
            std::cout << "***** Line " << i+1 << ": EXPORT FAILED *****" << std::endl;
            return false;
        }
    }
    std::cout << "***** EXPORT OK *****" << std::endl;
    ////sm->DebugOut();
    return true;
}

//-----------------------------------------------------------------------------
// Конструктор компилятора экспорта
//ExportCompiler::ExportCompiler(std::vector<std::string> &e): 
ExportCompiler::ExportCompiler(SemanticModel &m):
        line{0}, column{1}, errCount{0}, sm(m)
{}    

//=============================================================================
// Начальный нетерминал
//=============================================================================

//--------------------------------------------------------------------------
// Запуск процесса компиляции файла экспорта.
// Экспортруются различные варианты описаний, которые заносятся
// в таблицу имен экспорта
//! Export = ConstExport | FuncExpoert | TypeExport | ImportExport...
bool ExportCompiler::isExport() {
    Declaration* dcl = nullptr;
    std::string importedName;
    Ignore();   // пропуск пробельных символов и комментариев перед первой лексемой
//_0: // Проверка на имя экспорта
    // В начале идет проверка на обозначение экспорта
    if(isSimpleName()) {
        // Фиксация имени для последующего занесения в таблицу экспорта вместе с контекстом
        importedName = lexValue;
        //smodel.SetExportName(value) // Фиксация имени экспорта
        Ignore(); // пропуск пробельных символов и комментариев
        goto _1;
    }
    // Возможен пустой файл, если отсутствуют откомпилированные описания
    if (isEnd()) {
        return true;
    }
    Err("Export: Incorrect symbols at  the end or it is not name of exporting declaration");
    return false;
_1: // Далее пошли варианты с описанием альтернативных артефактов с постфиксным обозначением
    if(isConstExport(&dcl)) {
        goto _end;
    }
    if(isFuncExport(&dcl)) {
        goto _end;
    }
    if(isTypeExport(&dcl)) {
        goto _end;
    }
    if(isImportExport()) {
        goto _end;
    }
    Err("Export: It is not export body");
    return false;
_end:
    // После распознавания всех описаний идет тестовый вывод построенной таблицы
    //smodel.OutExportNameTab()
    //fmt.Println("Export: OK!")

    /// Тестовый вывод содержимого таблицы имен экспорта
    ///QTextStream cout(stdout)
    ///semModel.DebugInfo(cout)
    dcl->SetName(importedName);
    sm.AddToExportNameTable(dcl);
    return true;
}

//--------------------------------------------------------------------------
// Экспорт константы
bool ExportCompiler::isConstExport(Declaration** dcl) {
    rune r;
//_0: Проверка текущей лексемы на ключевое слово const
    if(isReservedWord("const")) {
        ////std::cout << "--> const keyword = " << lexValue << " OK" << std::endl;
        Ignore();
        goto _1;
    }
    return false;
_1: // Проверяется тип константы
    if(isReservedWord("int")) {
        //fmt.Println("-->const type int OK")
        Ignore();
        goto _2;
    }
    if(isReservedWord("bool")) {
        Ignore();
        goto _3;
    }
    if(isReservedWord("float")) {
        Ignore();
        goto _4;
    }
    if(isReservedWord("signal")) {
        Ignore();
        goto _5;
    }
    if (isReservedWord("char")) {
        Ignore();
        goto _char;
    }
    Err("isConstExport: Type of constant wated");
    return false;
_2: // Проверяется наличие целочисленного значения    
    if(isInteger()) {
        *dcl = sm.newDeclarationConstInt(std::stoi(lexValue));
        goto _end;
    }
_3: // Проверяется наличие булевского значения    
    if(isBoolean()) {
        bool bVal;
        if(lexValue == "false") {
            bVal = false;
        } else if(lexValue == "true") {
            bVal = true;
        } else {
            Err("isConstExport: Incorrect Boolean value");
            return false;
        }
        *dcl = sm.newDeclarationConstBool(bVal);
        goto _end;
    }
_4: // Проверяется наличие действительного значения    
    if(isFloat()) {
        *dcl = sm.newDeclarationConstFloat(std::stod(lexValue));
        goto _end;
    }
    return false;
_5:
    *dcl = sm.newDeclarationConst(sm.NewSignalValue());
    goto _end;
_char:
    if (isRune(r)) {
        *dcl = sm.newDeclarationConst(sm.NewRuneValue(r));
        goto _end;
    }
    return false;
_end:
    //fmt.Println("isConstExport-->")
    
    return true;
}

//--------------------------------------------------------------------------
// Символьная константа
// TODO: This is copy-paste from src/compiler/parser.cpp
// TODO: And therefore severe violation of DRY principle
bool ExportCompiler::isRune(rune& r) {
    storePos();

    if (isSymbol('\'')) {
        nextSym();
    } else {
        goto failure;
    }

    if (isSymbol('\\')) {
        nextSym();

        if (isSymbol('n')) {
            r = static_cast<rune>('\n');
            nextSym();
        } else if (isSymbol('r')) {
            r = static_cast<rune>('\r');
            nextSym();
        } else if (isSymbol('t')) {
            r = static_cast<rune>('\t');
            nextSym();
        } else {
            goto failure;
        }
    } else {
        const auto result = utf8::fetchRune(reinterpret_cast<const uint8_t*>(&currentDcl[column]), currentDcl.size() - static_cast<size_t>(column));
        if (result.success) {
            for (size_t i = 0; i < result.bytesRead; ++i) {
                nextSym();
            }

            r = result.r;
        } else {
            goto failure;
        }
    }

    if (isSymbol('\'')) {
        nextSym();
    } else {
        goto failure;
    }

    return true;

failure:
    restorePos();
    return false;
}

/*
//--------------------------------------------------------------------------
// Определение экспорта функции
bool ExportCompiler::isFuncExport(Declaration** ppdcl) {
    auto argTypeNum = 0;
    std::string argTypeName{""};
    std::string resTypeName{""};
    FuncProto* funcProto = nullptr;
    //n = ""
    //Type *argType = nullptr
    //Type *resType = nullptr
//_0: Проверка текущей лексемы на ключевое слово func
    if(isReservedWord("func")) {
        // Фиксация имени экспорта во временной переменной
        //n = semModel.GetArtefactName()
        funcProto = sm.NewFuncProto();
        Ignore();
        goto _1a;
    }
    return false;
_1a: // Проверка на количество сигнатур
    if(isInteger()) {
        argTypeNum = std::stoi(lexValue);
        if(argTypeNum < 1) { // Должна быть хотя бы одна сигнатура
            Err("isFuncExport: incorrect number of signtures");
            return false;
        }
        Ignore();
        goto _1;
    }
    Err("isFuncExport: number of arguments is absent");
    return false;
_1: // Проверяется наличие типов сигнатур аргументов
    if(isQualId()) {
        argTypeName = lexValue; 
        Ignore();
        goto _2;
    }
    Err("isFuncExport: type of argument is absent");
    return false;
_2: // Проверяется наличие типа результата
    if(isQualId()) {
        resTypeName = lexValue; 
        Ignore();
        // Формирование и фиксация очередной сигнатуры
        DeclarationType* adt = sm.FindTypeDeclaration(argTypeName);
        DeclarationType* rdt = sm.FindTypeDeclaration(resTypeName);
        if((adt != nullptr)&&(rdt != nullptr)) {
            // Выполняется добавление сигнатуры в список сигнатур
            funcProto->AddSig(Signature(adt, rdt));
        } else {
            Err("FuncDefinition: Incorrect type name of argument or result");
            return false;
        }
        --argTypeNum;
        // Далее переход в зависимости от значения счетчика числа сигнатур
        if(argTypeNum > 0) { // Очередная сигнатура
            goto _1;
        } else {
            goto _end;
        }
    }
    Err("isFuncExport: type of result is absent");
    return false;
_end:
    *ppdcl = sm.newDeclarationFuncProto(funcProto);
    return true;
}
*/

//--------------------------------------------------------------------------
// Определение экспорта функции (версия с анализом типов)
bool ExportCompiler::isFuncExport(Declaration** ppdcl) {
    auto argTypeNum = 0;
    //std::string argTypeName{""};
    //std::string resTypeName{""};
    FuncProto* funcProto = nullptr;
    Type* pta = nullptr;  // Указатель на тип аргумента
    Type* ptr = nullptr;  // Указатель на тип результата
    DeclarationType* adt = nullptr; // Указатель на объявление аргумента
    DeclarationType* rdt = nullptr; // Указатель на объявление результата
    //Type *resType = nullptr
//_0: Проверка текущей лексемы на ключевое слово func
    if(isReservedWord("func")) {
        // Фиксация имени экспорта во временной переменной
        //n = semModel.GetArtefactName()
        funcProto = sm.NewFuncProto();
        Ignore();
        goto _1a;
    }
    return false;
_1a: // Проверка на количество сигнатур
    if(isInteger()) {
        argTypeNum = std::stoi(lexValue);
        if(argTypeNum < 1) { // Должна быть хотя бы одна сигнатура
            Err("isFuncExport: incorrect number of signtures");
            return false;
        }
        Ignore();
        goto _1;
    }
    Err("isFuncExport: number of arguments is absent");
    return false;
_1: // Проверяется наличие типов сигнатур аргументов
    if(isQualId()) { // именованный тип
        adt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore();
        goto _2;
    }
    if(isTupleExport(&pta)) { // кортеж
        adt = sm.newDeclarationType(pta); // Создается неименованный кортеж
        Ignore();
        goto _2;
    }
    Err("isFuncExport: type of argument is absent");
    return false;
_2: // Проверяется наличие типа результата
    if(isQualId()) { // именованный тип
        rdt = sm.FindTypeDeclaration(lexValue); // Поиск именованного типа
        Ignore();
        goto _3;
    }
    if(isTupleExport(&ptr)) { // кортеж
        rdt = sm.newDeclarationType(ptr); // Создается неименованный кортеж
        Ignore();
        goto _3;
    }
_3: // Формирование сигнатуры для текущего аргумента и результата
    if((adt != nullptr)&&(rdt != nullptr)) {
        // Выполняется добавление сигнатуры в список сигнатур
        funcProto->AddSig(Signature(adt, rdt));
    } else {
        Err("isFuncExport: Incorrect type name of argument or result");
        return false;
    }
    --argTypeNum;
    // Далее переход в зависимости от значения счетчика числа сигнатур
    if(argTypeNum > 0) { // Очередная сигнатура
        goto _1;
    } else {
        goto _end;
    }
    Err("isFuncExport: type of result is absent");
    return false;
_end:
    *ppdcl = sm.newDeclarationFuncProto(funcProto);
    return true;
}

//--------------------------------------------------------------------------
// Определение типа.
bool ExportCompiler::isTypeExport(Declaration** ppdcl) {
    Type* ptv = nullptr;
//_0: Проверка текущей лексемы на ключевое слово type
    if(isReservedWord("type")) {
        Ignore();
        goto _1;
    }
    return false;
_1: // Анализ вариантов экспортируемого типа
    if(isBaseExport(&ptv)) { // переименованный тип
        goto _end;
    }
    if(isTupleExport(&ptv)) { // переименованный тип
        goto _end;
    }
    if(isStructExport(&ptv)) { // тип-структура
        goto _end;
    }
    Err("isTypeExport: A type definition have been expected");
    return false;
_end:
    *ppdcl = sm.newDeclarationType(ptv);
    return true;
}

//--------------------------------------------------------------------------
// Определение (переименование) базового типа.
bool ExportCompiler::isBaseExport(Type **pptv) {
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
// Объявление кортежа
bool ExportCompiler::isTupleExport(Type **pptv) {
    auto typeNum = 0;
    std::string typeName{""};
    TypeTuple* typeTuple = nullptr;
//_0: 
    if(isReservedWord("tuple")) { // кортежный тип
        typeTuple = sm.NewTuple();
        Ignore();
        goto _1;
    }
    return false;
_1: // Проверка на количество подтипов
    if(isInteger()) {
        typeNum = std::stoi(lexValue);
        if(typeNum < 1) { // Должен быть хотя бы один тип в кортеже
            Err("isTupleExport: incorrect number of types");
            return false;
        }
        Ignore();
        goto _2;
    }
    Err("isTupleExport: the number of arguments is absent");
_2: // Проверка на заданное число типов
    if(isQualId()) {
        typeName = lexValue; 
        Ignore();
        // Формирование и фиксация очередного тип кортежа
        DeclarationType* dt = sm.FindTypeDeclaration(typeName);
        if(dt != nullptr) {
            // Выполняется добавление типа в список типов кортежа
            typeTuple->AddType(dt);
        } else {
            Err("isTupleExport: Incorrect type name of tuple");
            return false;
        }
        --typeNum;
        // Далее переход в зависимости от значения счетчика числа сигнатур
        if(typeNum > 0) { // Очередная сигнатура
            goto _2;
        } else {
            goto _end;
        }
    }
    Err("isTupleExport: type of tuple's element is absent");
    return false;
_end:
    *pptv = typeTuple;
    return true;
}

bool ExportCompiler::isStructExport(Type** pptv) {
    std::string fieldName, fieldTypeName;
    DeclarationType *pdt = nullptr; // Объявление типа поля
    StructType *pst = sm.NewStruct();

    Ignore();

    if (isReservedWord("struct")) {
        Ignore();
    }

    if (isSymbol('(')) {
        nextSym();
        Ignore();
    } else {
        Err("Expected `(` in export file struct definition");
        return false;
    }

    while (true) {
        if (isSymbol(')')) {
            nextSym();
            Ignore();
            break;
        }

        if (isId()) {
            fieldName = lexValue;
            Ignore();
        } else {
            Err("Execpted field name in export file struct defintion");
            return false;
        }

        if (isSymbol('@')) {
            nextSym();
            Ignore();
        } else {
            Err("Expected `@` in export file struct definition");
            return false;
        }

        if (isId()) {
            fieldTypeName = lexValue;
            Ignore();
        } else {
            Err("Expected field type name in export file struct definition");
            return false;
        }

        if (isSymbol(',')) {
            nextSym();
            Ignore();
        } else {
            Err("Expected `,` in export file struct definition");
            return false;
        }

        pdt = sm.FindTypeDeclaration(fieldTypeName);
        if (pdt == nullptr) {
            Err("Failed to find type declaration");
            std::cout << "fieldTypeName = " << fieldTypeName << "\n";
            return false;
        }

        if (!pst->AddField(fieldName, pdt)) {
            Err("Field names in struct are not unique");
            std::cout << "fieldName = " << fieldName << "\n";
            return false;
        }
    }

    *pptv = pst;
    return true;
}

//--------------------------------------------------------------------------
// Определение импорта.
bool ExportCompiler::isImportExport() {
    return false;
}
