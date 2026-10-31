#pragma once

#include "data/queue.h"
#include "data/stack.h"
#include <ostream>
#include <string>
#include <string_view>

namespace ecosym {

class logger {
  public:
    void add(std::string event);
    [[nodiscard]] bool empty() const;
    void clear();
    void print_logs(std::ostream &output);
    void print_history(std::ostream &output);

  private:
    data::queue<std::string_view> current_;
    data::stack<std::string> history_;
};

} // namespace ecosym
