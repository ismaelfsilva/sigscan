#include "sigscan/sigscan.hpp"

#include <cctype>
#include <sstream>
#include <stdexcept>

namespace sigscan {

namespace {

bool isWildcard(const std::string& tok) {
    return tok == "?" || tok == "??";
}

int hexVal(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

uint8_t parseByte(const std::string& tok) {
    if (tok.size() != 2) throw std::invalid_argument("invalid byte token: " + tok);
    int hi = hexVal(tok[0]);
    int lo = hexVal(tok[1]);
    if (hi < 0 || lo < 0) throw std::invalid_argument("invalid hex token: " + tok);
    return static_cast<uint8_t>((hi << 4) | lo);
}

} // namespace

Pattern Pattern::parse(const std::string& text) {
    Pattern pat;
    std::istringstream iss(text);
    std::string tok;
    while (iss >> tok) {
        if (isWildcard(tok)) {
            pat.bytes.push_back(0);
            pat.mask.push_back(false);
        } else {
            pat.bytes.push_back(parseByte(tok));
            pat.mask.push_back(true);
        }
    }
    if (pat.bytes.empty()) throw std::invalid_argument("empty pattern");
    return pat;
}

std::vector<size_t> scanAll(const uint8_t* data, size_t len, const Pattern& pat) {
    std::vector<size_t> hits;
    const size_t n = pat.size();
    if (n == 0 || len < n) return hits;
    for (size_t i = 0; i + n <= len; ++i) {
        bool ok = true;
        for (size_t j = 0; j < n; ++j) {
            if (pat.mask[j] && data[i + j] != pat.bytes[j]) {
                ok = false;
                break;
            }
        }
        if (ok) hits.push_back(i);
    }
    return hits;
}

std::optional<size_t> findFirst(const uint8_t* data, size_t len, const Pattern& pat) {
    const size_t n = pat.size();
    if (n == 0 || len < n) return std::nullopt;
    for (size_t i = 0; i + n <= len; ++i) {
        bool ok = true;
        for (size_t j = 0; j < n; ++j) {
            if (pat.mask[j] && data[i + j] != pat.bytes[j]) {
                ok = false;
                break;
            }
        }
        if (ok) return i;
    }
    return std::nullopt;
}

} // namespace sigscan
