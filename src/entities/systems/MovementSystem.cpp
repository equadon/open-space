#include "cg/entities/systems/MovementSystem.h"
#include "cg/entities/components/VelocityComponent.h"

MovementSystem::MovementSystem(EntityManager *entityManager, GLContext *context) : System(entityManager), context(context) {
}

MovementSystem::~MovementSystem() {
}

void MovementSystem::init() {
}

void MovementSystem::update() {
    for (auto& pair : entityManager->getComponents<VelocityComponent>()) {
        auto entity = pair.first;
        auto velocity = reinterpret_cast<VelocityComponent *>(pair.second);
        auto transform = entityManager->getComponent<TransformComponent>(entity);
        auto mass = entityManager->getComponent<MassComponent>(entity);

//add mass component and acceleration
        if (velocity && transform) {
            if(entity->id == 0) {
                printf("a = %s, %v = %s, p = $s\n",
                        glm::to_string(velocity->acceleration).c_str(), glm::to_string(velocity->position).c_str(), glm::to_string(transform->position).c_str());
            }
            velocity->position += velocity->acceleration * static_cast<float>(glm::pow(context->getDeltaTime(), 2));
            auto translation = velocity->position;
            translation -= velocity->gravity;
            transform->move(translation * static_cast<float>(context->getDeltaTime()));

            auto rotation = velocity->rotation * static_cast<float>(context->getDeltaTime());
            transform->rotate(rotation);

            if (glm::length(velocity->scaling) > 0) {
                auto scaling = velocity->scaling * static_cast<float>(context->getDeltaTime());
                transform->scale(glm::normalize(scaling));
            }

            if (velocity->customUpdate.has_value()) {
                velocity->customUpdate.value()(entityManager, entity);
            }
        }
    }
}
