# sigscan

AOB / byte-pattern signature scanner with wildcards for reverse engineering (C++17).

`sigscan` parses a textual array-of-bytes (AOB) signature such as
`48 8B ?? E8 ?? ?? ?? ??`, where `??` or `?` are wildcards, into a byte
sequence plus a boolean mask, then scans a binary buffer for every match.

## Features

- Header + small library (`sigscan`) with a clean C++17 API.
- Wildcard support (`??` or `?`) per byte.
- `findFirst` and `scanAll` search helpers.
- Command-line tool to scan a file and print matched offsets in hex.
- Portable: no platform-specific headers.

## Library API

```cpp
#include <sigscan/sigscan.hpp>

sigscan::Pattern pat = sigscan::Pattern::parse("48 8B ?? E8 ?? ?? ?? ??");

std::optional<size_t> first = sigscan::findFirst(data, len, pat);
std::vector<size_t>   all   = sigscan::scanAll(data, len, pat);
```

## Build

```sh
cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=g++ -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

## CLI usage

```sh
sigscan <file> "<pattern>"
```

Example:

```sh
sigscan game.bin "48 8B ?? E8 ?? ?? ?? ??"
```

Prints every matched offset in hexadecimal, one per line:

```
0x1f3a
0x204c
```

## License

MIT - see [LICENSE](LICENSE).
