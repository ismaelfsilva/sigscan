#ifndef SIGSCAN_SIGSCAN_HPP
#define SIGSCAN_SIGSCAN_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sigscan {

// An AOB signature: bytes plus a per-byte wildcard mask.
// mask[i] == true means bytes[i] is significant; false means wildcard.
struct Pattern {
    std::vector<uint8_t> bytes;
    std::vector<bool> mask;

    // Parse a space-separated pattern such as "48 8B ?? E8 ?? ?? ?? ??".
    // Each token is a two-hex-digit byte or a wildcard ("?" or "??").
    // Throws std::invalid_argument on malformed input.
    static Pattern parse(const std::string& text);

    size_t size() const { return bytes.size(); }
};

// Return every offset in [0, len) where the pattern matches.
std::vector<size_t> scanAll(const uint8_t* data, size_t len, const Pattern& pat);

// Return the first matching offset, or std::nullopt if none.
std::optional<size_t> findFirst(const uint8_t* data, size_t len, const Pattern& pat);

} // namespace sigscan

#endif // SIGSCAN_SIGSCAN_HPP
