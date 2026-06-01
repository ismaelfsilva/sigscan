#include "sigscan/sigscan.hpp"

#include <cassert>
#include <cstdint>
#include <vector>

int main() {
    // offsets: 0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17
    const std::vector<uint8_t> buf = {
        0x48, 0x8B, 0x05, 0xE8, 0x11, 0x22, 0x33, 0x44,
        0x90, 0x90,
        0x48, 0x8B, 0xC0, 0xE8, 0xAA, 0xBB, 0xCC, 0xDD,
    };
    const uint8_t* d = buf.data();
    const size_t n = buf.size();

    // Pattern with "??" wildcards matches at offsets 0 and 10.
    sigscan::Pattern pat = sigscan::Pattern::parse("48 8B ?? E8 ?? ?? ?? ??");
    auto first = sigscan::findFirst(d, n, pat);
    assert(first.has_value());
    assert(first.value() == 0);

    auto all = sigscan::scanAll(d, n, pat);
    assert(all.size() == 2);
    assert(all[0] == 0);
    assert(all[1] == 10);

    // No-match pattern returns empty / no value.
    sigscan::Pattern miss = sigscan::Pattern::parse("FF EE DD");
    assert(sigscan::scanAll(d, n, miss).empty());
    assert(!sigscan::findFirst(d, n, miss).has_value());

    // Single "?" wildcard form, multiple matches.
    sigscan::Pattern single = sigscan::Pattern::parse("48 ? ?? E8");
    auto multi = sigscan::scanAll(d, n, single);
    assert(multi.size() == 2);
    assert(multi[0] == 0);
    assert(multi[1] == 10);

    return 0;
}
