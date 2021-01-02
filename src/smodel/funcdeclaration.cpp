#include <iostream>

#include "funcdeclaration.h"

DeclarationFunc::DeclarationFunc(FuncProto* fv): funcProtoValue{fv}
{}

DeclarationFunc::DeclarationFunc(std::string n, FuncProto* fv):
    Declaration(n), funcProtoValue{fv}
{}

// Формирование объявления как текстовой строки
std::string DeclarationFunc::toString() {
    return funcProtoValue->toString();
}

// Тестовый вывод экспортируемого объявления в консоль
void DeclarationFunc::DebugOut() {
    Declaration::DebugOut();
    funcProtoValue->DebugOut();
//     std::cout << "\n";
}

// Слияние двух объявлений функции для формирования окончательного списка сигнатур
bool DeclarationFunc::MergeFuncDcl(DeclarationFunc* dcl) {
    // Доступ к двум спискам сигнатур
    auto sigList = funcProtoValue->GetSignatures();
    // Доступ к единственному типу только что компилируемой функции
    FuncProto* pfp = dcl->GetFuncProto();
    auto newSigList = pfp->GetSignatures();
    Signature& newSignature = newSigList[0];
    
    for(auto it: sigList) {
        auto compare = it.Cmp(newSignature);
        switch(compare) {
            case 1:
                // Возможны еще варианты. Перебираем
                continue;
            case 0:
                // Сигнатуры совпадают. Слияние не требуется. Менять не нужно
                return true;
            case -1:
                // Несовпадение типов результатов. Некорректная ситуация
                // Слияние недопустимо
                return false;
        }
    }
    // Несовпадени типов аргументов. Можно добавить новую сигнатуру
    //pfp->AddSig(newSignature);
    funcProtoValue->AddSig(newSignature);
    return true;
}
