#pragma once

#include "types.h"
#include <string>
#include <vector>

namespace ecosym {

class map {
public:
    map(const map &) = delete;
    map(map &&) = delete;
    map &operator=(const map &) = delete;
    map &operator=(map &&) = delete;

    map(int rows, int columns);
    [[nodiscard]] int rows() const;
    [[nodiscard]] int columns() const;
    [[nodiscard]] bool in_bounds(position location) const;
    [[nodiscard]] std::vector<position> neighbors(position center,
                                                  int range) const;
    [[nodiscard]] char at(position location) const;
    void set(position location, char symbol);
    void clear();

private:
    void check_location(position location) const;

    std::vector<std::string> cells_;
    const char empty_ = '\'';
};

} // namespace ecosym
