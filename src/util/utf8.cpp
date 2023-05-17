#include "utf8.h"

namespace utf8 {

FetchResult::FetchResult(): success(false) {
}

FetchResult::FetchResult(rune _r, size_t _bytesRead): r(_r), bytesRead(_bytesRead) {
}

FetchResult fetchRune(const uint8_t *buffer, size_t length) {
    if (length == 0) {
        return FetchResult();
    }

    const auto leadingByte = *buffer;
    buffer++;

    if ((leadingByte & 0x80) == 0) { // leadingByte is of form 0xxx xxxx
        return FetchResult(static_cast<rune>(leadingByte), 1);
    }

    if ((leadingByte & 0xC0) == 0xC0) { // leadingByte is of from 110x xxxx
        if (length < 2) {
            return FetchResult();
        }

        const auto secondByte = *buffer;
        buffer++;

        if ((secondByte & 0xC0) != 0x80) {
            return FetchResult();
        }

        const rune r = static_cast<rune>(secondByte - 0x80) | (static_cast<rune>(leadingByte - 0xC0) << 6ULL);
        return FetchResult(r, 2);
    }

    // Bigger codepoints are not yet supported

    return FetchResult();
}

void writeRune(std::ostream& os, rune r) {
    uint8_t buf[4] = {0, 0, 0, 0};
    size_t length = 0;

    if (r < 0x80) {
        buf[0] = static_cast<uint8_t>(r);
        length = 1;
    } else if (r < 0x800) {
        buf[0] = static_cast<uint8_t>(r >> 6ULL) | 0xC0;
        buf[1] = static_cast<uint8_t>(r & 0x3F) | 0x80;
        length = 2;
    }

    // Bigger codepoints are not yet supported

    if (length > 0) {
        os.write(reinterpret_cast<char*>(&buf[0]), length);
    } else {
        os << "<UNK>";
    }
}

}
