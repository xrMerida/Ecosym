#include "ecosym/simulation.h"
#include "data/list.h"
#include "ecosym/carnivore.h"
#include "ecosym/herbivore.h"
#include "ecosym/organism.h"
#include "ecosym/plant.h"
#include "logger.h"
#include <climits>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace ecosym {

simulation::simulation(int rows, int columns, randomizer random)
    : map_(), random_(random) {
    if (rows <= 0 || columns <= 0) {
        throw std::invalid_argument("map dimensions must be positive");
    }
    map_.assign(rows, std::string(columns, '\''));
}

simulation::~simulation() {
    for (organism *current : organisms_) {
        delete current;
    }
}

void simulation::spawn(trophic_t type, char symbol, position location) {
    if (!empty(location)) {
        throw std::invalid_argument("location already occupied");
    }

    organism *new_organism;
    switch (type) {
    case trophic_t::plant:
        new_organism = new plant{symbol, location};
        break;
    case trophic_t::herbivore:
        new_organism = new herbivore{symbol, location};
        break;
    case trophic_t::carnivore:
        new_organism = new carnivore{symbol, location};
        break;
    default:
        throw std::invalid_argument("unknown trophic type");
    }

    std::ostringstream event;
    event << new_organism->symbol() << " was born at " << location;
    logger_.add(event.str());
    organisms_.push_back(new_organism);
}

size_t simulation::organism_count() const {
    return organisms_.size();
}

int simulation::rows() const {
    return static_cast<int>(map_.size());
}

int simulation::columns() const {
    return static_cast<int>(map_[0].size());
}

size_t simulation::count(trophic_t type) const {
    size_t result = 0;
    for (const organism *current : organisms_) {
        if (current->trophic() == type) {
            result++;
        }
    }
    return result;
}

logger &simulation::logs() {
    return logger_;
}

int simulation::turn() const {
    return turn_;
}

const organism *simulation::lookup(position location) const {
    for (const organism *current : organisms_) {
        if (current->location() != location) continue;

        return current;
    }
    return nullptr;
}

bool simulation::empty(position location) const {
    return in_bounds(location) && lookup(location) == nullptr;
}

bool simulation::in_bounds(position location) const {
    return location.row >= 0 && location.row < rows() && location.column >= 0 &&
           location.column < columns();
}

bool simulation::move(organism *who, position destination) {
    if (!owns(who) || !empty(destination)) {
        return false;
    }

    std::ostringstream event;
    event << who->symbol() << " move from " << who->location() << " to "
          << destination;
    logger_.add(event.str());
    who->set_location(destination);
    return true;
}

organism *simulation::find(trophic_t type, position center, int range) {
    organism *nearest = nullptr;
    int nearest_distance = INT_MAX;

    for (organism *current : organisms_) {
        if (current->trophic() != type) {
            continue;
        }

        int current_distance = distance(center, current->location());
        if (current_distance > range) {
            continue;
        }

        if (current_distance < nearest_distance) {
            nearest = current;
            nearest_distance = current_distance;
        }
    }
    return nearest;
}

organism *simulation::find(trophic_t type, position center) {
    organism *nearest = nullptr;
    int nearest_distance = INT_MAX;

    for (organism *current : organisms_) {
        if (current->trophic() != type) {
            continue;
        }

        int current_distance = distance(center, current->location());
        if (current_distance < nearest_distance) {
            nearest = current;
            nearest_distance = current_distance;
        }
    }
    return nearest;
}

position simulation::random_empty(position center, int range) const {
    std::vector<position> locations;
    for (int r = center.row - range; r <= center.row + range; r++) {
        for (int c = center.column - range; c <= center.column + range; c++) {
            position location{r, c};
            if (location == center) continue;
            if (!in_bounds(location)) continue;
            if (!empty(location)) continue;

            locations.push_back(location);
        }
    }

    if (locations.empty()) {
        return no_position;
    }
    return locations[random_(static_cast<int>(locations.size()))];
}

position simulation::random_empty() const {
    std::vector<position> locations;

    for (int r = 0; r < rows(); r++) {
        for (int c = 0; c < columns(); c++) {
            const position location{r, c};
            if (empty(location)) {
                locations.push_back(location);
            }
        }
    }

    if (locations.empty()) {
        return no_position;
    }

    return locations[random_(static_cast<int>(locations.size()))];
}

bool simulation::kill(organism *hunter, trophic_t prey_type) {
    if (!owns(hunter)) {
        return false;
    }

    organism *prey = find(prey_type, hunter->location(), hunter->range());
    if (prey == nullptr) {
        return false;
    }

    std::ostringstream event;
    event << hunter->symbol() << " hunted " << prey->symbol() << " at "
          << prey->location();
    logger_.add(event.str());
    position destination = prey->location();
    hunter->add_energy(prey->food_value());
    remove(prey);
    move(hunter, destination);
    return true;
}

bool simulation::move_random(organism *who) {
    if (!owns(who)) {
        return false;
    }

    position destination = random_empty(who->location(), who->range());
    return destination != no_position && move(who, destination);
}

bool simulation::stalk(organism *hunter, trophic_t prey_type) {
    if (!owns(hunter)) {
        return false;
    }

    organism *prey = find(prey_type, hunter->location());
    if (prey == nullptr) {
        return false;
    }

    position current = hunter->location();
    int current_distance = distance(current, prey->location());

    const int r_initial = current.row - hunter->range();
    const int r_final = current.row + hunter->range();
    const int c_initial = current.column - hunter->range();
    const int c_final = current.column + hunter->range();
    for (int r = r_initial; r <= r_final; r++) {
        for (int c = c_initial; c <= c_final; c++) {
            position destination{r, c};
            if (current == destination) continue;
            if (!in_bounds(destination)) continue;
            if (!empty(destination)) continue;
            if (distance(current, prey->location()) >= current_distance)
                continue;

            return move(hunter, destination);
        }
    }
    return false;
}

void simulation::step() {
    turn_++;
    logger_.clear();

    // FIX: Inefficient solution
    // Possibly use a static iterator on global scope
    // A pointer to an iterator, changed when erase()
    // is called
    // Previous solution is fragile for reverse iterator
    // but those are not used here. Snapshot is left as-is
    data::list<organism *> snapshot;
    for (organism *current : organisms_) {
        snapshot.push_back(current);
    }

    for (organism *current : snapshot) {
        if (!owns(current)) continue;
        if (current->dead()) continue;
        current->act(*this);
    }
    remove_dead();
}

void simulation::draw(std::ostream &output) {
    for (std::string &row : map_) {
        row.assign(columns(), '\'');
    }
    for (const organism *current : organisms_) {
        map_[current->location().row][current->location().column] =
            current->symbol();
    }

    for (int r = rows() - 1; r >= 0; r--) {
        output << std::setw(2) << r << ' ';
        for (int c = 0; c < columns(); c++) {
            char symbol = map_[r][c];
            output << std::setw(2) << symbol << ' ';
        }
        output << '\n';
    }
    output << "   ";
    for (int c = 0; c < columns(); c++) {
        output << std::setw(2) << c << ' ';
    }
    output << '\n';
}

int simulation::distance(position first, position second) {
    return std::max(std::abs(first.row - second.row),
                    std::abs(first.column - second.column));
}

void simulation::remove(organism *target) {
    for (auto it = organisms_.begin(); it != organisms_.end(); ++it) {
        if (*it != target) continue;

        std::ostringstream event;
        event << target->symbol() << " died at " << target->location();
        logger_.add(event.str());
        delete target;
        organisms_.erase(it);
        return;
    }
}

bool simulation::owns(const organism *candidate) const {
    if (!candidate) return false;
    for (const organism *current : organisms_) {
        if (current == candidate) {
            return true;
        }
    }
    return false;
}

void simulation::remove_dead() {
    auto it = organisms_.begin();
    while (it != organisms_.end()) {
        organism *&current = *it;
        if (!current->dead()) {
            ++it;
            continue;
        }

        std::ostringstream event;
        event << current->symbol() << " died at " << current->location();
        logs().add(event.str());
        delete current;
        it = organisms_.erase(it);
    }
}

} // namespace ecosym
