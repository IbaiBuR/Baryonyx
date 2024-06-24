#pragma once

#include "types.hpp"

class time_manager {
    public:
        time_manager() :
            m_start_time(0),
            m_base_time(UINT64_MAX),
            m_increment(UINT16_MAX) {}

        time_manager(const u64 start_time, const u64 base_time, const u16 increment) {
            this->m_start_time = start_time;
            this->m_base_time  = base_time;
            this->m_increment  = increment;
        }

        void set_start_time(u64 start_time);

        [[nodiscard]] u64 elapsed() const;
        [[nodiscard]] u64 optimum_time() const;

    private:
        u64 m_start_time;
        u64 m_base_time;
        u16 m_increment;
};