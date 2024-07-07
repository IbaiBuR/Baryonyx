#include "tt.hpp"

#include <algorithm>

namespace search::tt {

bool transposition_table::probe(const zobrist_key key, tt_entry& entry) const {
    if (const auto current_entry = m_data[index(key)]; current_entry.key_matches(key)) {
        entry = current_entry;

        return true;
    }

    return false;
}

void transposition_table::clear() { std::ranges::fill(m_data, tt_entry{}); }

void transposition_table::resize(const usize size_mb) {
    constexpr usize bytes_per_mb = 1024 * 1024;
    const usize     entry_count  = (size_mb * bytes_per_mb) / sizeof(tt_entry);

    m_data.resize(entry_count);
    clear();
}

void transposition_table::prefetch(const zobrist_key key) {
    __builtin_prefetch(&m_data[index(key)]);
}

void transposition_table::store(const zobrist_key key, const tt_entry& entry) {
    m_data[index(key)] = entry;
}

u64 transposition_table::index(const zobrist_key key) const {
    return (static_cast<u128>(key) * static_cast<u128>(m_data.size())) >> 64;
}

u16 transposition_table::hashfull() const {
    u16 hashfull{};

    for (int i = 0; i < 1000; ++i) {
        if (m_data[i].flag() != tt_entry::tt_flag::none)
            ++hashfull;
    }

    return hashfull;
}

} // namespace search::tt
