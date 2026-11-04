#include "organism.h"
#include "animal.h"
#include "plant.h"
#include "simulation.h"

void ecosym::animal::act(simulation &world) {
    consume_energy(upkeep());
    if (dead()) return;

    if (world.kill(this, prey_type())) return;
    if (world.stalk(this, prey_type())) return;
    world.move_random(this);
}

void ecosym::plant::act(simulation &world) {
    add_energy(2);
    if (energy() < max_energy()) return;

    spread(world);
}

void ecosym::plant::spread(simulation &world) {
    position seed_location = world.random_empty(location(), range());
    if (seed_location == no_position) return;

    world.spawn(trophic(), symbol(), seed_location);
    consume_energy(max_energy());
}
