#pragma once

#include "ecosym/organism.h"
#include "ecosym/simulation.h"

namespace ecosym {

class animal : public organism {
public:
    animal(char symbol, position location, int energy)
        : organism(symbol, location, energy) {}

    void act(simulation &world) override;

protected:
    virtual trophic_t prey_type() const = 0;
    virtual int upkeep() const = 0;
};

} // namespace ecosym
