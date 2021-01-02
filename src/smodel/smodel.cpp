#include <iostream>
#include <algorithm>

#include "smodel.h"

// Статическое локальное пространство для хранения единичных экземпляров
namespace {
    TypeSignal          typeSignal;
    TypeInt             typeInt;
    TypeBool            typeBool;
    TypeFloat           typeFloat;
    
    DeclarationType     dclSignal("signal", &typeSignal);
    DeclarationType     dclInt("int", &typeInt);
    DeclarationType     dclBool("bool", &typeBool);
    DeclarationType     dclFloat("float", &typeFloat);
}

// Получение доступа к семантической модели, реализованной как синглетон
SemanticModel* GetSemanticModel() {
    static SemanticModel sm;
    std::cout << "SemanticModel created as singleton\n";
    //std::cout << "Creating of SemanticModel\n";
    //return new SemanticModel;
    return &sm;
}

// Удаление семантической модели
void DeleteSemanticModel(SemanticModel*) {
    std::cout << "Deleting of SemanticModel\n";
}

// Конструктор, формирующий каркас семантической модели
SemanticModel::SemanticModel(): 
    artefactName{""}, compiledFunction{nullptr}
{
    // Добавление предопределенных типов во внутреннюю таблицу имен
    internalNameTable.Append(&dclSignal);
    internalNameTable.Append(&dclInt);
    internalNameTable.Append(&dclBool);
    internalNameTable.Append(&dclFloat);
    
    // Формирование и добавление в таблицу имен предопределенных функций (операций)
    AddPreFuncProtoToNameTable();
}

// Тестовый вывод содержимого семантической модели в консоль
void SemanticModel::DebugOut() {
    std::cout << "Semantic Model. Test Out!!!\n";
    
    // Вывод содержимого таблицы предопределенного импорта
    std::cout << "--- Internal Table ---" << std::endl;
    internalNameTable.DebugOut();
    // Вывод содержимого таблицы экспорта
    std::cout << "--- Export Table ---" << std::endl;
    exportNameTable.DebugOut();
    if(compiledFunction != nullptr) {
        std::cout << "--- Definition of function ---" << std::endl;
        std::cout << artefactName << "<< ";
        compiledFunction->DebugOut();
    }
}

//------------------------------------------------------------------
// Формирование указателя на сигнальную константу
Const* SemanticModel::NewSignalValue() {
    static ConstSignal s;
    return &s;
}

// Создание целочисленной константы
Const* SemanticModel::NewIntValue(int iv) {
    return new ConstInt(iv);
}

// Создание булевской константы
Const* SemanticModel::NewBoolValue(bool bv) {
    return new ConstBool(bv);
}

// Создание действительной константы
Const* SemanticModel::NewFloatValue(double fv) {
    return new ConstFloat(fv);
}

//------------------------------------------------------------------
// Формирование объявление полученной обобщенной константы
Declaration* SemanticModel::newDeclarationConst(Const* c) {
    return new DeclarationConst(c);
}

// Формирование объявления целочисленной константы
Declaration* SemanticModel::newDeclarationConstInt(int v) {
    return new DeclarationConst(NewIntValue(v));
}

// Формирование объявления булевской константы
Declaration* SemanticModel::newDeclarationConstBool(bool v) {
    return new DeclarationConst(NewBoolValue(v));
}

// Формирование объявления действительной константы
Declaration* SemanticModel::newDeclarationConstFloat(double v) {
    return new DeclarationConst(NewFloatValue(v));
}

//------------------------------------------------------------------
// Получение указателя на сигнальный тип
Type* SemanticModel::GetTypeSignal() {
    return &typeSignal;
}

//------------------------------------------------------------------
// Получение указателя на целочисленный тип
Type* SemanticModel::GetTypeInt() {
    return &typeInt;
}

//------------------------------------------------------------------
// Получение указателя на булевский тип
Type* SemanticModel::GetTypeBool() {
    return &typeBool;
}

//------------------------------------------------------------------
// Получение указателя на действительный тип
Type* SemanticModel::GetTypeFloat() {
    return &typeFloat;
}

//------------------------------------------------------------------
// Формирование объявления для сформированного типа
DeclarationType* SemanticModel::newDeclarationType(Type* t) {
    return new DeclarationType(t);
}

//------------------------------------------------------------------
// Создание прототипа функции
FuncProto* SemanticModel::NewFuncProto() {
    return new FuncProto;
}

//------------------------------------------------------------------
// Создание функции
Func* SemanticModel::NewFunc() {
    return new Func;
}

//------------------------------------------------------------------
// Создание кортежа
TypeTuple* SemanticModel::NewTuple() {
    return new TypeTuple;
}

//------------------------------------------------------------------
// Формирование объявления для функции
DeclarationFunc* SemanticModel::newDeclarationFuncProto(FuncProto* f) {
    return new DeclarationFunc(f);
}
    
//------------------------------------------------------------------
// Добавление в таблицу экспорта очередного объявления
// Предполагается, что невозможно заместить объявление одного типа
// объявлением другого типа. Для такого замещения нужно предварительно
// из таблицы экспорта явно удалить замещаемое объявление
bool SemanticModel::AddToExportNameTable(Declaration* dcl) {
    // Выделение имени сформрованного нового объявления
    std::string& newName = dcl->GetName();
    // Поиск объявления с этим именем в таблице экспорта
    Declaration* pd = exportNameTable.FindDeclaration(newName);
    if(pd != nullptr) {
        // При совпадении имен проводится анализ на то, что оба описания являются функциями
        // Тогда нужно разобраться с сигнатурами.
        if(auto newDcl = dynamic_cast<DeclarationFunc*>(dcl); newDcl != nullptr) {
            // Новое описание является функцией
            std::cout << "Test out: The func " << newName << " was compiled\n";
            // Прошла проверка, что новое объявление - функция
            if(auto oldDcl = dynamic_cast<DeclarationFunc*>(pd); oldDcl != nullptr) {
                std::cout << "The old name is func too\n";
                // Прошла проверка, что старое объявление - функция
                // Необходимо слить два объявления одой функции путем добавления
                // нового к старому с различными вариациями
                auto ok = oldDcl->MergeFuncDcl(newDcl);
                return ok;
            } else { // Новое описание не является фунцией
                std::cout << "The old name isn't func\n";
                // Два противоречивых объявления с одним именем. 
                // Недопустма замена без предварительного удаления старого описания
                // соответствующей дополнительной утилитой.
                return false;
            }
        } else {
            // В других случаях при одноименных объявлениях идет замена старого объявления на новое
            // Этот фрагмент в дальнейшем может быть реализован с более строгим сценарием замены,
            // Что обуславливается возможным влиянием на внешние зависимости
            pd = dcl;
            return true;
        }
    }
    // В том случае, если имя нового объявления не совпадает ни с одним из уже объявленных
    // осуществляется безусловное добавление нового объявления в таблицу экспорта
    exportNameTable.Append(dcl);
    return true;
}

// Поиск объявления типа по его имени среди существующих таблиц имен
DeclarationType* SemanticModel::FindTypeDeclaration(std::string& n) {
    Declaration* pd = internalNameTable.FindDeclaration(n);
    if(auto typeDcl = dynamic_cast<DeclarationType*>(pd); typeDcl != nullptr) {
        return typeDcl;
    }

    pd = exportNameTable.FindDeclaration(n);
    if(auto typeDcl = dynamic_cast<DeclarationType*>(pd); typeDcl != nullptr) {
        return typeDcl;
    }

    return nullptr;
}

// Поиск объявления в таблице экспорта по имени
Declaration* SemanticModel::FindExportedDeclaration(std::string& n) {
    return exportNameTable.FindDeclaration(n);
}

// Второе статическое локальное пространство компонент для прототипов базовых функций
namespace {
    // Набор сигнатур (возможно, повторно использующихся
    Signature sigBoolToBool(&dclBool, &dclBool);
    Signature sigIntToInt(&dclInt, &dclInt);
    Signature sigFloatToFloat(&dclFloat, &dclFloat);

    TypeTuple ttIntInt;
    DeclarationType dclIntPair("", &ttIntInt);
    Signature sigInt2ToInt(&dclIntPair, &dclInt);
    
    FuncProto inc;
    DeclarationFunc dclInc("++", &inc);
    
    FuncProto dec;
    DeclarationFunc dclDec("--", &dec);

    FuncProto add;
    DeclarationFunc dclAdd("+", &add);

    FuncProto subMin;
    DeclarationFunc dclSubMin("-", &subMin);
}


// Формирование и добавление в таблицу имен предопределенных функций (операций)
// Заполняются выше описанные артефакты
void SemanticModel::AddPreFuncProtoToNameTable() {
    ttIntInt.AddType(&dclInt);
    ttIntInt.AddType(&dclInt);
    
    // Добавление функции прибавления единицы к целому
    inc.AddSig(sigIntToInt);
    internalNameTable.Append(&dclInc);
    
    // Добавление функции вычитания единицы из целого
    dec.AddSig(sigIntToInt);
    internalNameTable.Append(&dclDec);

    // Операция сложения
    add.AddSig(sigInt2ToInt);
    internalNameTable.Append(&dclAdd);
    
    // Функция смены знака на противоположный или инверсии булевской величины
    // Операция вычитания
    subMin.AddSig(sigBoolToBool);
    subMin.AddSig(sigIntToInt);
    subMin.AddSig(sigFloatToFloat);
    subMin.AddSig(sigInt2ToInt);
    internalNameTable.Append(&dclSubMin);
}

// Сохранение таблицы экспорта после компиляции артефакта
void SemanticModel::Store(std::ofstream &ost) {
    exportNameTable.Store(ost);
}

//------------------------------------------------------------------
// Получение указателей на предопределенные базовые функции
// и другие предопределенные артефакты

// Получение объявления функции ++
DeclarationFunc* SemanticModel::GetDeclarationInc() {
    return &dclInc;
}

// Получение объявления функции +
DeclarationFunc* SemanticModel::GetDeclarationAdd() {
    return &dclAdd;
}

// Получение объявления функции --
DeclarationFunc* SemanticModel::GetDeclarationDec() {
    return &dclDec;
}

// Получение объявления функции -
DeclarationFunc* SemanticModel::GetDeclarationSubMin() {
    return &dclSubMin;
}


