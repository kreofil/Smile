#include <iostream>
//#include <algorithm>

#include "nametable.h"

void NameTable::DebugOut() {
    auto count = 0;
    for(auto it: table) {
        std::cout << ++count << ") ";
        it->DebugOut();
        std::cout << std::endl;
    }

}

// Поиск в таблице объявления с заданным именем
Declaration* NameTable::FindDeclaration(std::string& n) {
    if(auto it = std::find_if(table.begin(), table.end(), 
                            [&n](Declaration* d ) {return d->GetName() == n;});
       it != table.end()) 
    {
        Declaration* pd = *it;
        return pd;
    }
    return nullptr;
}
 
// Сохранение таблицы имен
void NameTable::Store(std::ofstream &ost) {
    for(auto it: table) {
        ost << it->GetName() << " " << it->toString() << "\n";
    }
}

 
