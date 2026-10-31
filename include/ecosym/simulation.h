#pragma once

#include "data/list.h"
#include "ecosym/logger.h"
#include "ecosym/map.h"
#include "ecosym/organism.h"
#include "types.h"
#include <ostream>

namespace ecosym {

class simulation {
public:
    using randomizer = int (*)(int);

    simulation(const simulation &) = delete;
    simulation(simulation &&) = delete;
    simulation &operator=(const simulation &) = delete;
    simulation &operator=(simulation &&) = delete;

    simulation(int rows, int columns, randomizer random);
    ~simulation();

    [[nodiscard]] size_t organism_count() const;
    [[nodiscard]] int rows() const;
    [[nodiscard]] int columns() const;
    [[nodiscard]] size_t count(trophic_t type) const;
    [[nodiscard]] logger &logs();
    [[nodiscard]] int turn() const;
    [[nodiscard]] const organism *lookup(position location) const;
    [[nodiscard]] bool empty(position location) const;
    [[nodiscard]] position random_empty(position center, int range) const;
    [[nodiscard]] position random_empty() const;
    void spawn(trophic_t type, char symbol, position location);
    bool kill(organism *hunter, trophic_t prey_type);
    bool move_random(organism *who);
    bool move_towards(organism *who, trophic_t prey_type);
    void step();
    void draw(std::ostream &output);

private:
    map map_;
    data::list<organism *> organisms_;
    logger logger_;
    randomizer random_;
    int turn_ = 0;

    static int distance(position first, position second);
    bool move(organism *who, position destination);
    void remove(organism *target);
    [[nodiscard]] organism *find(trophic_t type, position center, int range);
    [[nodiscard]] organism *find(trophic_t type, position center);
    [[nodiscard]] bool owns(const organism *candidate) const;

    void remove_dead();
};

} // namespace ecosym
