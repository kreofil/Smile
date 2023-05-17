#include "struct.h"

#include <iostream>
#include <sstream>

StructType::StructType() {
}

int StructType::TypeSize() {
    int totalSize = 0;
    for (const auto& [name, pdt]: fields) {
        totalSize += pdt->GetType()->TypeSize();
    }

    return totalSize;
}

std::string StructType::toString() {
    std::ostringstream os;
    os << "struct (";
    for (const auto& [name, pdt]: fields) {
        os << name << "@" << pdt->GetType()->toString() << ",";
    }
    os << ")";
    return os.str();
}

void StructType::DebugOut() {
    std::cout << toString();
}

bool StructType::EqStruct(Type *pt) {
    StructType* pst = dynamic_cast<StructType*>(pt);
    if (pst == nullptr) {
        return false;
    }

    if (fields.size() != pst->fields.size()) {
        return false;
    }

    for (const auto& [name, pdt]: fields) {
        DeclarationType *pdt2 = pst->GetFieldType(name);
        if (pdt2 == nullptr) {
            return false;
        }

        if (!pdt->GetType()->EqStruct(pdt2->GetType())) {
            return false;
        }
    }

    return true;
}

bool StructType::AddField(const std::string& name, DeclarationType* pd) {
    const auto [iter, flag] = fields.insert(std::make_pair(name, pd));
    return flag;
}

DeclarationType* StructType::GetFieldType(const std::string& name) const {
    const auto iter = fields.find(name);
    if (iter == fields.end()) {
        return nullptr;
    } else {
        return iter->second;
    }
}

std::vector<std::string> StructType::GetFieldNames() const {
    std::vector<std::string> names;
    names.reserve(fields.size());
    for (const auto& [name, pdt]: fields) {
        names.push_back(name);
    }

    return names;
}
