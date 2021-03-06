#pragma once

#include <cg/GLHeader.h>
#include <cg/skybox/Skybox.h>
#include <cg/entities/EntityManager.h>
#include <cg/entities/systems/RenderSystem.h>
#include <cg/entities/systems/InputSystem.h>
#include <cg/entities/systems/MovementSystem.h>
#include <cg/entities/systems/GravitySystem.h>
#include <cg/entities/systems/OrbitSystem.h>
#include <cg/entities/systems/CollisionSystem.h>
#include <cg/procedural/Universe.h>
#include <cg/entities/UniverseEntityFactory.h>

class Application
{
public:
    const uint16_t MAX_LIGHTS = 2;

    Application(EntityManager &, GLContext &, UniverseEntityFactory &, RenderSystem &, MovementSystem &,
                OrbitSystem &, InputSystem &, GravitySystem &, CollisionSystem &);

    void init(std::string title, uint16_t width, uint16_t height);
    void run();

private:
    EntityManager &entityManager;
    UniverseEntityFactory &universeEntityFactory;
    GLContext &context;

    Universe universe;

    Skybox *sky = nullptr;

    // Systems
    RenderSystem &renderSystem;
    InputSystem &inputSystem;
    MovementSystem &movementSystem;
    GravitySystem &gravitySystem;
    OrbitSystem &orbitSystem;
    CollisionSystem &collisionSystem;

    std::vector<glm::vec3> instanceTransformations;

    void createCameras();
    Entity *createGrid(int width, int height, bool showYAxis);

    void setLightUniforms(ShaderProgram *meshTextureShaderProgram, std::vector<std::pair<LightComponent *, TransformComponent *>> lights);
};
