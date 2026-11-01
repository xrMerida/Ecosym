#pragma once

#include <ostream>
namespace ecosym {
struct position {
    int row = 0;
    int column = 0;

    bool operator==(const position &) const = default;
};

enum class trophic_t {
    plant,
    herbivore,
    carnivore,
};

constexpr position no_position{-1, -1};

inline std::ostream &operator<<(std::ostream &output, position location) {
    return output << '[' << location.column << ", " << location.row << ']';
}

inline std::ostream &operator<<(std::ostream &output, trophic_t type) {
    switch (type) {
    case trophic_t::plant:
        return output << "plant";
    case trophic_t::herbivore:
        return output << "herbivore";
    case trophic_t::carnivore:
        return output << "carnivore";
    }
    return output << "unknown";
}
} // namespace ecosym
