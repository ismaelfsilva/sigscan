#include "sigscan/sigscan.hpp"

#include <cstdio>
#include <fstream>
#include <iterator>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::fprintf(stderr, "usage: sigscan <file> \"<pattern>\"\n");
        return 2;
    }

    std::ifstream in(argv[1], std::ios::binary);
    if (!in) {
        std::fprintf(stderr, "error: cannot open file: %s\n", argv[1]);
        return 1;
    }
    std::vector<uint8_t> data((std::istreambuf_iterator<char>(in)),
                              std::istreambuf_iterator<char>());

    sigscan::Pattern pat;
    try {
        pat = sigscan::Pattern::parse(argv[2]);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "error: %s\n", e.what());
        return 1;
    }

    for (size_t off : sigscan::scanAll(data.data(), data.size(), pat)) {
        std::printf("0x%zx\n", off);
    }
    return 0;
}
