#pragma once

#include <cstdint>
#include <ostream>

using rune = uint32_t;

namespace utf8 {

struct FetchResult {
    rune r;
    bool success;
    size_t bytesRead;

    explicit FetchResult();
    explicit FetchResult(rune _r, size_t _bytesRead);
};

void writeRune(std::ostream& os, rune r);
FetchResult fetchRune(const uint8_t *buffer, size_t length);

};
