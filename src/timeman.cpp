#include "timeman.hpp"

#include "utils/time.hpp"

void TimeManager::set_start_time(const u64 start_time) { m_start_time = start_time; }

u64 TimeManager::elapsed() const { return utils::time::get_time_ms() - m_start_time; }

u64 TimeManager::optimum_time() const { return m_base_time / 20 + m_increment / 2; }
