#include "Planet.h"

Planet::Planet(std::vector<SeedType> seed, Star &star, SeedType index) : seed(seed), index(index) {
    seed.emplace_back(PLANET_SEED);
    seed.emplace_back(index);

    auto seedSequence = std::seed_seq(seed.begin(), seed.end());
    random = std::mt19937(seedSequence);

    auto distance = 5.0 * star.radius * index;

    position = glm::dvec3(distance, star.position.y, star.position.z);
    radius = star.radius * 0.1;

    auto r = limit(random(), 0.2, 1.0);
    auto g = limit(random(), 0.2, 1.0);
    auto b = limit(random(), 0.2, 1.0);
    color = glm::vec3(r, g, b);

//    printf("Planet %d: %s\n", index, glm::to_string(position).c_str());

    // Orbit details
    semiMajorAxis = distance;
    semiMinorAxis = distance;
    orbitSpeed = limit(random(), 0.001, 0.01);
    orbitAngle = limit(random(), 0, 2 * PI);

    // TODO: Less moons for planets close to their star
    auto numMoons = glm::round(gaussian(random(), 0, MAX_MOONS, AVERAGE_MOONS, MOONS_STANDARD_DEVIATION));
    for (int i = 0; i < numMoons; i++) {
        moons.emplace_back(getMoon(i));
    }
}

Moon Planet::getMoon(int index) {
    return Moon(seed, index);
}
