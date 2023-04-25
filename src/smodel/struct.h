#ifndef __struct__
#define __struct__
// Описание структуры

#include "declaration.h"

#include <map>

class StructType: public Type {
    std::map<std::string, DeclarationType*> fields;

public:
    explicit StructType();
    std::string toString() override;
    void DebugOut() override;
    int TypeSize() override;
    // Сравнение типов на структурную эквивалентность
    bool EqStruct(Type *pt) override;

    bool AddField(const std::string& name, DeclarationType* dt);
    DeclarationType* GetFieldType(const std::string& name) const;
    std::vector<std::string> GetFieldNames() const;
};

#endif
