#pragma once

#include "ecosym/animal.h"

namespace ecosym {

class carnivore : public animal {
public:
    explicit carnivore(char symbol, position location)
        : animal(symbol, location, 10) {}

    [[nodiscard]] int max_energy() const override { return 15; }
    [[nodiscard]] int range() const override { return 3; }
    [[nodiscard]] trophic_t trophic() const override {
        return trophic_t::carnivore;
    }

private:
    trophic_t prey_type() const override { return trophic_t::herbivore; }
    int upkeep() const override { return 3; }
};

} // namespace ecosym
