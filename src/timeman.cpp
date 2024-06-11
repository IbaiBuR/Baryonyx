#include "timeman.hpp"

void TimeManager::setStartTime(const u64 startTime) { m_startTime = startTime; }

u64 TimeManager::elapsed() const { return Utils::getTimeMs() - m_startTime; }

u64 TimeManager::optimumTime() const { return m_baseTime / 20 + m_increment / 2; }
