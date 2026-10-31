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
#include <vector>

namespace ecosym {

simulation::simulation(int rows, int columns, randomizer random)
    : map_(rows, columns), random_(random) {}

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
    return map_.rows();
}

int simulation::columns() const {
    return map_.columns();
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
    return map_.in_bounds(location) && lookup(location) == nullptr;
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
    for (position location : map_.neighbors(center, range)) {
        if (empty(location)) {
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
    for (int row = 0; row < map_.rows(); row++) {
        for (int column = 0; column < map_.columns(); column++) {
            const position location{row, column};
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
    if (hunter == nullptr || !owns(hunter)) {
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
    if (owns(who)) {
        return false;
    }

    position destination = random_empty(who->location(), who->range());
    return destination != no_position && move(who, destination);
}

bool simulation::move_towards(organism *who, trophic_t prey_type) {
    if (who == nullptr || !owns(who)) {
        return false;
    }

    organism *prey = find(prey_type, who->location());
    if (prey == nullptr) {
        return false;
    }

    position current = who->location();
    int current_distance = distance(current, prey->location());

    for (position destination : map_.neighbors(current, who->range())) {
        if (empty(destination) &&
            distance(destination, prey->location()) < current_distance) {
            return move(who, destination);
        }
    }
    return false;
}

void simulation::step() {
    turn_++;
    logger_.clear();
    data::list<organism *> snapshot;
    for (organism *current : organisms_) {
        snapshot.push_back(current);
    }

    for (organism *current : snapshot) {
        if (current->dead()) continue;
        current->act(*this);
    }
    remove_dead();
}

void simulation::draw(std::ostream &output) {
    map_.clear();
    for (const organism *current : organisms_) {
        map_.set(current->location(), current->symbol());
    }

    output << "   ";
    for (int c = 0; c < map_.columns(); c++) {
        output << std::setw(2) << c << ' ';
    }
    output << '\n';

    for (int r = 0; r < map_.rows(); r++) {
        output << std::setw(2) << r << ' ';
        for (int c = 0; c < map_.columns(); c++) {
            output << std::setw(2) << map_.at({r, c}) << ' ';
        }
        output << '\n';
    }
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
    for (const organism *current : organisms_) {
        if (current == candidate) {
            return true;
        }
    }
    return false;
}

void simulation::remove_dead() {
    for (auto it = organisms_.begin(); it != organisms_.end();) {
        organism *current = *it;
        if (!current->dead()) {
            ++it;
            continue;
        }

        delete current;
        it = organisms_.erase(it);
    }
}

} // namespace ecosym
