#ifndef __nametable__
#define __nametable__

#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "declaration.h"


// Таблица имен, используемая для хранения объявлений различного типа.
// Выделена отдельно для реализации требуемых манипуляций
class NameTable {
    std::vector<Declaration*> table;
public:
    explicit NameTable() {}
    void DebugOut();
    
    // Безусловное добавление нового объявления в таблицу
    void Append(Declaration* pd) {table.push_back(pd);}
    // Поиск в таблице объявления с заданным именем
    Declaration* FindDeclaration(std::string& n);
    // Сохранение таблицы имен
    void Store(std::ofstream &ost);
};

#endif
