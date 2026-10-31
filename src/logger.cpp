#include "ecosym/logger.h"
#include <string>

namespace ecosym {

void logger::add(std::string event) {
    if (event.empty()) return;
    history_.push(event);
    current_.push(history_.top());
}

bool logger::empty() const {
    return current_.empty();
}

void logger::clear() {
    current_.clear();
}

void logger::print_logs(std::ostream &output) {
    while (!current_.empty()) {
        output << current_.front() << '\n';
        current_.pop();
    }
}

void logger::print_history(std::ostream &output) {
    clear();
    while (!history_.empty()) {
        output << history_.top() << '\n';
        history_.pop();
    }
}

} // namespace ecosym
