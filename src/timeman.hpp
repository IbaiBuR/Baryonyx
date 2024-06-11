#pragma once

#include "utils.hpp"

class TimeManager {
    public:
        TimeManager() :
            m_startTime(0),
            m_baseTime(UINT64_MAX),
            m_increment(UINT16_MAX) {}

        TimeManager(const u64 startTime, const u64 baseTime, const u16 increment) {
            this->m_startTime = startTime;
            this->m_baseTime  = baseTime;
            this->m_increment = increment;
        }

        void setStartTime(u64 startTime);

        [[nodiscard]] u64  elapsed() const;
        [[nodiscard]] u64  optimumTime() const;

    private:
        u64 m_startTime;
        u64 m_baseTime;
        u16 m_increment;
};