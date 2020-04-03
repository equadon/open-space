#include "entities/components/CameraComponent.h"

CameraComponent::CameraComponent(CameraComponent::Mode mode, CameraComponent::Type type, glm::vec3 target, glm::vec3 front, glm::vec3 up, PositionComponent *positionComponent)
        : mode(mode), type(type), target(target), zoom(1.0f), mouseSensitivity(0.1f), movementSpeed(2.5f) {
    // Initialize vectors
    this->worldUp = glm::normalize(up);
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    // Initialize angles
    auto position = positionComponent->position;
    yaw = 180.0f + glm::degrees(glm::atan((position.z - target.z) / (position.x - target.x)));
    if (isnan(yaw)) {
        yaw = 0.0f;
    }

    auto q = glm::vec3(position.x, 0, position.z);
    pitch = -glm::degrees(glm::acos(glm::dot(position, q) / (glm::length(position) * glm::length(q))));
    if (isnan(pitch)) {
        pitch = 0.0f;
    }
}

glm::mat4 CameraComponent::getView(PositionComponent *positionComponent) {
    auto position = positionComponent->position;
    if (mode == Free) {
        return glm::lookAt(position, position + front, up);
    } else {
        throw std::runtime_error("camera mode not implemented!");
    }
}

glm::mat4 CameraComponent::getProjection(float aspectRatio) {
    if (type == Orthographic) {
        auto zoomOut = 1.0f;
        return glm::ortho(-aspectRatio * zoomOut, aspectRatio * zoomOut, -1.0f * zoomOut, 1.0f * zoomOut, -1000.0f, 100.0f);
    } else if (type == Perspective) {
        auto fov = 45.0f / zoom;
        return glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);
    } else {
        throw std::runtime_error("failed to get projection: unknown projection mode");
    }
}

void CameraComponent::processKeyboard(CameraComponent::Direction direction, float deltaTime, PositionComponent *positionComponent) {
    auto position = positionComponent->position;
    float velocity = movementSpeed * deltaTime;
    if (direction == Forward) {
        position += front * velocity;
    } else if (direction == Backward) {
        position -= front * velocity;
    } else if (direction == Left) {
        position -= right * velocity;
    } else if (direction == Right) {
        position += right * velocity;
    }
    positionComponent->position = position;
}

void CameraComponent::processMouseMovement(float offsetX, float offsetY) {
    offsetX *= mouseSensitivity;
    offsetY *= mouseSensitivity;

    yaw += offsetX;
    pitch = glm::clamp(pitch + offsetY, -89.0f, 89.0f);

    updateVectors();
}

void CameraComponent::updateVectors() {
    auto yawRad = glm::radians(yaw);
    auto pitchRad = glm::radians(pitch);

    glm::vec3 direction;
    direction.x = glm::cos(yawRad) * glm::cos(pitchRad);
    direction.y = glm::sin(pitchRad);
    direction.z = glm::sin(yawRad) * glm::cos(pitchRad);

    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
