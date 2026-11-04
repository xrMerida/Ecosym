#pragma once

#include <ostream>
namespace ecosym {
/// @brief A row and column location in the simulation map.
struct position {
    int row = 0;
    int column = 0;

    bool operator==(const position &) const = default;
};

/// @brief The trophic level of an organism.
enum class trophic_t {
    plant,
    herbivore,
    carnivore,
};

/// @brief An invalid non-null position.
constexpr position no_position{-1, -1};

/// @brief Writes a position as a column-row coordinate pair.
/// @param output The stream receiving the formatted position.
/// @param location The position to write.
/// @return The supplied output stream.
inline std::ostream &operator<<(std::ostream &output, position location) {
    return output << '[' << location.column << ", " << location.row << ']';
}

/// @brief Writes a trophic type.
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
