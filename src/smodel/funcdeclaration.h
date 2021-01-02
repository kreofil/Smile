#ifndef __funcdeclaration__
#define __funcdeclaration__

//#include "declaration.h"
#include "func.h"

// Класс, фиксирующий прототип функции, оставляя для объявления 
// только поля, необходимые для ее экспорта.
class DeclarationFunc: public Declaration {
    FuncProto* funcProtoValue;
public:
    explicit DeclarationFunc(FuncProto* fv);
    explicit DeclarationFunc(std::string n, FuncProto* fv);

    virtual std::string toString();
    virtual void DebugOut();
    
    // Слияние двух объявлений функции для формирования
    // окончательного списка сигнатур
    bool MergeFuncDcl(DeclarationFunc* dcl);
    
    FuncProto* GetFuncProto() {return funcProtoValue;}
};


#endif
