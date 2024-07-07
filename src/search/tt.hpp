#pragma once

#include <vector>

#include "../moves/move.hpp"

namespace search::tt {

/// @class tt_entry
/// @brief Represents and entry of the transposition table
/// @note For efficiency and in order to maximize the number of entries that the tranposition table
/// can store, keys and scores are packed into 16 bits
class tt_entry {
    public:
        enum class tt_flag : u8 {
            none,
            exact,
            upper_bound,
            lower_bound
        };

        tt_entry() :
            m_key(0),
            m_move(moves::move::null()),
            m_score(constants::score_none),
            m_depth(0),
            m_flag(tt_flag::none) {}

        tt_entry(
            const zobrist_key k, const moves::move m, const score s, const u8 d, const tt_flag f) :
            m_key(static_cast<tt_key>(k)),
            m_move(m),
            m_score(static_cast<i16>(s)),
            m_depth(d),
            m_flag(f) {}

        [[nodiscard]] tt_key key() const { return m_key; }

        [[nodiscard]] moves::move move() const { return m_move; }

        [[nodiscard]] i16 value() const { return m_score; }

        [[nodiscard]] u8 depth() const { return m_depth; }

        [[nodiscard]] tt_flag flag() const { return m_flag; }

        [[nodiscard]] bool key_matches(const zobrist_key key) const {
            return m_key == static_cast<tt_key>(key);
        }

        /// @brief Determines if the stored score in the entry can be used for search
        /// @param alpha The lower bound of the search window
        /// @param beta The upper bound of the search window
        /// @returns true if the score is exact or within the search window bounds
        [[nodiscard]] bool can_use_score(const score alpha, const score beta) const {
            return m_flag == tt_flag::exact || (m_flag == tt_flag::upper_bound && m_score <= alpha)
                || (m_flag == tt_flag::lower_bound && m_score >= beta);
        }

    private:
        tt_key      m_key;
        moves::move m_move;
        i16         m_score;
        u8          m_depth;
        tt_flag     m_flag;
};

static_assert(sizeof(tt_entry) == 8);

class transposition_table {
    public:
        transposition_table() :
            transposition_table(default_tt_size) {}

        explicit transposition_table(const usize size) { resize(size); }

        /// @brief Checks if the position has been saved already in the tranposition table
        /// @param key Zobrist key
        /// @param entry Entry to fill if position is found
        /// @returns true if the position's key and the tranposition table key match
        bool probe(zobrist_key key, tt_entry& entry) const;

        /// @brief Clears the transposition table by filling it with default-initialized entries
        void clear();

        /// @brief Sets the size of the transposition table accordingly
        /// @param size_mb Memory to allocate, in MB
        void resize(usize size_mb);

        /// @brief Prefetches and entry of the tranposition table
        /// @param key Zobrist key
        void prefetch(zobrist_key key);

        /// @brief Stores an entry in the transposition table
        /// @param key Zobrist key
        /// @param entry Entry to store
        void store(zobrist_key key, const tt_entry& entry);

        /// @brief Gives an estimate of how much entries are filled in the transposition table
        /// @returns The number of filled entries, in the range [0, 1000]
        [[nodiscard]] u16 hashfull() const;

    private:
        /// @brief Default size for the tranposition table, in MB
        static constexpr usize default_tt_size = 16;

        /// @brief Creates an index to map the tranposition table using the "fast range" trick
        /// @param key Zobrist key
        /// @note See https://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
        /// @returns The computed 64-bit index
        [[nodiscard]] u64 index(zobrist_key key) const;

        std::vector<tt_entry> m_data;
};

/// @brief Adjusts the score before storing it in the transposition table
/// @param s Score
/// @param ply Ply
/// @returns The adjusted score
inline score score_to_tt(score s, const int ply) {
    if (s > constants::score_win)
        s += ply;
    else if (s < -constants::score_win)
        s -= ply;

    return s;
}

/// @brief Adjusts the score before retrieving it from the transposition table
/// @param s Score
/// @param ply Ply
/// @returns The adjusted score
inline score score_from_tt(score s, const int ply) {
    if (s > constants::score_win)
        s -= ply;
    else if (s < -constants::score_win)
        s += ply;

    return s;
}

/// @brief Shared tranposition table
inline transposition_table global_tt;

} // namespace search::tt