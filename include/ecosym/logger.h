#pragma once

#include "data/queue.h"
#include "data/stack.h"
#include <ostream>
#include <string>
#include <string_view>

namespace ecosym {

/// @brief Stores current-turn events and the complete event history.
class logger {
  public:
    /// @brief Logs an event.
    /// @param event The event description.
    /// @events are stored in a LIFO history and a FIFO current-turn log.
    void add(std::string event);
    /// @brief Checks whether the current-turn log is empty.
    [[nodiscard]] bool empty() const;
    /// @brief Clears the current-turn log.
    void clear();
    /// @brief Writes and clears the current-turn events.
    /// @param output The stream receiving the events.
    void print_logs(std::ostream &output);
    /// @brief Writes the complete event history.
    /// @param output The stream receiving the history.
    void print_history(std::ostream &output);

  private:
    data::queue<std::string_view> current_;
    data::stack<std::string> history_;
};

} // namespace ecosym
