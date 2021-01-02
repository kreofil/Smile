#include <iostream>
#include <sstream>

#include "func.h"

//==============================================================================
// Методы класса, описывающего семантическую модель сигнатуры функции
//==============================================================================

//------------------------------------------------------------------
Signature::Signature(DeclarationType* adt, DeclarationType* rdt):
            argDeclarationType{adt}, resDeclarationType{rdt}
{}

std::string Signature::toString() {
    return argDeclarationType->GetType()->toString() 
            + " " 
            + resDeclarationType->GetType()->toString();
}

void Signature::DebugOut() {
    argDeclarationType->GetType()->DebugOut();
    std::cout << " -> ";
    resDeclarationType->GetType()->DebugOut();
}

int Signature::Cmp(Signature& s) {
    if(!argDeclarationType->EqStruct(s.GetArgDeclarationType())) {
        // Сигнатуры разные
        return 1;
    } else if(resDeclarationType->EqStruct(s.GetResDeclarationType())) {
        // Сигнатуры одинаковы
        return 0;
    }
    // Типы аргументов не должны быть одинаковыми при разных
    // типах результатов. Ситуация недопустима
    return -1;
/*
    if( (argDeclarationType != s.GetArgDeclarationType())) {
        // Сигнатуры разные
        return 1;
    } else if(resDeclarationType == s.GetResDeclarationType()) {
        // Сигнатуры одинаковы
        return 0;
    }
    // Типы аргументов не должны быть одинаковыми при разных
    // типах результатов. Ситуация недопустима
    return -1;
*/
}

//==============================================================================
// Методы класса, описывающего семантическую модель прототипа функции
//==============================================================================

//------------------------------------------------------------------
FuncProto::FuncProto()
{}

std::string FuncProto::toString() {
    std::string s{"func "};
    auto sigListSize = sigList.size();
    std::ostringstream os;
    os <<  sigListSize;
    s += os.str();
    for(auto it:sigList) {
        s += " " + it.toString();
    }
    return s;
}

void FuncProto::DebugOut() {
    std::cout << "func ";
    for(auto it:sigList) {
        it.DebugOut();
        std::cout << "; ";
    }
}

//==============================================================================
// Методы класса, описывающего семантическую модель функции
//==============================================================================

//------------------------------------------------------------------
Func::Func() {
/*
    // Формирование обязательной начальной вершины РИГ, фиксирующей объявление аргумента
    // Всегда располагается в нулевой позиции вектора вершин
    AddActor(new ActorArg{0});
    // Добавление обязательной вершины возврата из функции.
    // На начальном этапе она следует за начальной вершиной, но впоследствии возможна
    // реализация перемещения в конец после изменения порядка следования вершин в соответствии
    // с информационными зависимостями между ними.
    AddActor(new ActorRet{});
*/
}

std::string Func::toDclString() {
    std::string s{"func "};
    s += argType->toString() + " " + resType->toString();
    return s;
}

//------------------------------------------------------------------
void Func::DebugOut() {
    ////std::cout << "-------- DEFINITION OF FUNCTION --------\n";
    std::cout << "func " << argTypeName << "@";
    argType->DebugOut();
    std::cout << " -> ";
    resType->DebugOut();
    std::cout << std::endl;
    
    // Вывод отладочной информации о вершинах РИГ и связях между ними
    std::cout << "--- FUNC BODY ---" << std::endl;
    for(auto it: body) {
        it->DebugOut();
        std::cout << std::endl;
    }
    std::cout << "Number of actors = " << ActorNumber() << std::endl;
}

//------------------------------------------------------------------
// Добавление в таблицу имен функции очередного объявления
void Func::AddToLocalNameTable(Declaration* pd) {
    localNameTable.Append(pd);
}

//------------------------------------------------------------------
// Добавление в таблицу имен функции очередного объявления
void Func::AddToImportNameTable(Declaration* pd) {
    importNameTable.push_back(pd);
}

// Поиск объявления актора в локальной таблице имен
DeclarationActor* Func::FindLocalDeclaration(std::string& n) {
    Declaration* pd = localNameTable.FindDeclaration(n);
    if(pd != nullptr) {
        DeclarationActor* pda = dynamic_cast<DeclarationActor*>(pd);
        if(pda != nullptr) {
            return pda;
        }
    }
    return nullptr;
}
