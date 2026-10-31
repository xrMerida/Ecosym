#include "ecosym/map.h"
#include <stdexcept>

namespace ecosym {

map::map(int rows, int columns, char empty) : cells_(), empty_(empty) {
    if (rows <= 0 || columns <= 0) {
        throw std::invalid_argument("map dimensions must be positive");
    }
    cells_.assign(rows, std::string(columns, empty_));
}

int map::rows() const {
    return static_cast<int>(cells_.size());
}

int map::columns() const {
    return static_cast<int>(cells_[0].size());
}

bool map::in_bounds(position location) const {
    return location.row >= 0 && location.row < rows() && location.column >= 0 &&
           location.column < columns();
}

std::vector<position> map::neighbors(position center, int range) const {
    if (range < 0) {
        throw std::invalid_argument("neighbor range cannot be negative");
    }

    std::vector<position> result;
    for (int row = center.row - range; row <= center.row + range; row++) {
        for (int column = center.column - range;
             column <= center.column + range; column++) {
            position location{row, column};
            if (location != center && in_bounds(location)) {
                result.push_back(location);
            }
        }
    }
    return result;
}

char map::at(position location) const {
    check_location(location);
    return cells_[location.row][location.column];
}

void map::set(position location, char symbol) {
    check_location(location);
    cells_[location.row][location.column] = symbol;
}

void map::clear() {
    for (std::string &row : cells_) {
        row.assign(columns(), empty_);
    }
}

void map::check_location(position location) const {
    if (!in_bounds(location)) {
        throw std::out_of_range("map location is outside the map");
    }
}

} // namespace ecosym
