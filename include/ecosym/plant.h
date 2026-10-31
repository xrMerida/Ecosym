#pragma once

#include "ecosym/organism.h"
#include "ecosym/simulation.h"

namespace ecosym {

class plant : public organism {
public:
    explicit plant(char symbol, position location)
        : organism(symbol, location, 0) {}

    void act(simulation &world) override;

    [[nodiscard]] int max_energy() const override { return 4; }
    [[nodiscard]] int food_value() const override { return max_energy(); }
    [[nodiscard]] bool dead() const override { return false; }
    [[nodiscard]] int range() const override { return 1; }
    [[nodiscard]] trophic_t trophic() const override {
        return trophic_t::plant;
    }

private:
    void spread(simulation &world);
};

} // namespace ecosym
