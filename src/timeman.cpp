#include "timeman.hpp"

#include "utils/time.hpp"

void TimeManager::set_start_time(const u64 startTime) { m_startTime = startTime; }

u64 TimeManager::elapsed() const { return utils::time::get_time_ms() - m_startTime; }

u64 TimeManager::optimum_time() const { return m_baseTime / 20 + m_increment / 2; }
