#include <stb_image.h>
#include <cg/entities/EntityBuilder.h>
#include <cg/skybox/SkyboxStar.h>
#include <stb_image_write.h>
#include "cg/skybox/Skybox.h"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

Skybox::Skybox(glm::vec3 size, ShaderProgram *shaderProgram) : size(size) {
    indexed = false;
    mode = GL_TRIANGLES;
    indexed = false;
    instances = 1;
    this->shaderProgram = shaderProgram;

    createMesh();
}

Skybox::Skybox(glm::vec3 size, std::string textureFilename, ShaderProgram *shaderProgram) : Skybox(size, shaderProgram) {
    createTexture(textureFilename);

    setupBuffers();
}

Skybox::Skybox(glm::vec3 size, glm::vec3 position, ShaderProgram *shaderProgram) : Skybox(size, shaderProgram) {
    createTexture(position);

    setupBuffers();
}

void Skybox::createMesh() {
    float x = size.x / 2.0f;
    float y = size.y / 2.0f;
    float z = size.z / 2.0f;

    auto a =  glm::vec3(-x, y, z);
    auto b =  glm::vec3(x, y, z);
    auto c =  glm::vec3(-x, -y, z);
    auto d =  glm::vec3(x, -y, z);
    auto e =  glm::vec3(-x, y, -z);
    auto f =  glm::vec3(x, y, -z);
    auto g =  glm::vec3(-x, -y, -z);
    auto h =  glm::vec3(x, -y, -z);

    // Front
    createTriangle(a, b, c);
    createTriangle(b, d, c);

    // Top
    createTriangle(a, e, b);
    createTriangle(b, e, f);

    // Left side
    createTriangle(a, g, e);
    createTriangle(a, c, g);

    // Right side
    createTriangle(b, h, d);
    createTriangle(b, f, h);

    // Back side
    createTriangle(f, e, g);
    createTriangle(f, g, h);

    // Under side
    createTriangle(c, h, g);
    createTriangle(c, d, h);
}

void Skybox::createTexture(glm::vec3 position) {
    throw std::runtime_error("Not implemented!");
}

void Skybox::createTexture(std::string filename) {
    std::vector<std::string> faces = {
            "./assets/textures/skybox1/right.png",
            "./assets/textures/skybox1/left.png",
            "./assets/textures/skybox1/top.png",
            "./assets/textures/skybox1/bottom.png",
            "./assets/textures/skybox1/front.png",
            "./assets/textures/skybox1/back.png",
    };

    Texture texture {};
    texture.isCubeMap = true;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (GLuint i = 0; i < faces.size(); i++) {
        int width, height, numChannels;
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &numChannels, 0);
        auto format = numChannels == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    textures.emplace_back(texture);
}

void Skybox::createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
    auto normal = glm::normalize(glm::cross(b - a, c - a));

    vertices.emplace_back(Vertex { a, normal });
    vertices.emplace_back(Vertex { b, normal });
    vertices.emplace_back(Vertex { c, normal });
}

/**
 * Generate a random skybox texture based on the seed.
 */
void Skybox::generate(uint64_t seed) {
}

void Skybox::render(RenderSystem *renderSystem, EntityManager *entityManager, CameraComponent *camera) {
    std::vector<glm::vec4> clearColors = {
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
            glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
            glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
    };

//    std::vector<std::vector<glm::vec3>> directions = {
//            // Front, right, up
//            { glm::vec3(1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0) },
//            { glm::vec3(-1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0) },
//            { glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) },
//            { glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1) },
//            { glm::vec3(0, 0, 1), glm::vec3(-1, 0, 0), glm::vec3(0, 1, 0) },
//            { glm::vec3(0, 0, -1), glm::vec3(1, 0, 0), glm::vec3(0, 1, 0) },
//    };
    std::vector<std::vector<glm::vec3>> directions = {
            // Front, right, up
            // X - pos / neg
            { glm::vec3(1, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0) },
            { glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0) },
            // Y - pos / neg
            { glm::vec3(0, 1, 0), glm::vec3(1, 0, 0), glm::vec3(0, 0, 1) },
            { glm::vec3(0, -1, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1) },
            // Z - pos / neg
            { glm::vec3(0, 0, 1), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0) },
            { glm::vec3(0, 0, -1), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0) },
    };

    uint32_t framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glViewport(0, 0, resolution, resolution);

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    auto starShaderProgram = new ShaderProgram();
    starShaderProgram->attachShader("./assets/shaders/skyboxStar.vert", ShaderType::VertexShader);
    starShaderProgram->attachShader("./assets/shaders/skyboxStar.frag", ShaderType::FragmentShader);
    starShaderProgram->link();

    createEntities(entityManager, starShaderProgram);

    glEnable(GL_DEPTH);
    for (uint16_t i = 0; i < 6; i++) {
        // Rotate camera
        camera->front = directions[i][0];
        camera->right = directions[i][1];
        camera->up = directions[i][2];

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, resolution, resolution, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, texture, 0);

//        auto clearColor = clearColors[i];
        float diff = 5.0 / 255.0;
//        auto clearColor = glm::vec4(i * diff, i * diff, i * diff, 1.0);
        auto clearColor = glm::vec4(0, 0, 0, 1.0);
        glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderEntities(renderSystem, entityManager, starShaderProgram);

//        std::string filename = "cubemap_" + std::to_string(i) + ".png";
//        auto data = new uint8_t[resolution][resolution][3];
//        glReadPixels(0, 0, resolution, resolution, GL_RGB, GL_UNSIGNED_BYTE, data);
//        stbi_write_png(filename.c_str(), resolution, resolution, 3, data, 0);
//        delete[] data;
    }

    textures[0].id = texture;

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Failed to generate skybox!\n");
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &framebuffer);
}

glm::vec3 getStarRotation(glm::vec3 position) {
    auto pos = glm::normalize(position);
    auto forward = glm::vec3(0, 0, -1);
    auto angle = glm::acos(glm::dot(glm::normalize(pos), forward));

    auto up = glm::normalize(glm::cross(forward, pos));
    return up * angle;
}

void Skybox::createEntities(EntityManager *entityManager, ShaderProgram *shaderProgram) {
    float radius = 900;
    int count = 128;
    float rad = 0.0f;
    float diff = static_cast<float>(2 * PI) / static_cast<float>(count);
    for (int i = 0; i < count; i++) {
        float x1 = radius * glm::cos(rad);
        float z1 = radius * glm::sin(rad);
        auto position1 = glm::vec3(x1, 0, z1);
        auto rotation1 = getStarRotation(position1);
        auto size = 1.0;
        auto transform = new TransformComponent(position1);
        transform->rotate(rotation1);
        EntityBuilder::create()
                ->withTransform(transform)
                ->withMesh(new SkyboxStar(size, shaderProgram))
                ->build(entityManager);
        float z2 = radius * glm::cos(rad);
        float y2 = radius * glm::sin(rad);
        auto position2 = glm::vec3(0, y2, z2);
        auto rotation2 = getStarRotation(position2);
        transform = new TransformComponent(position2);
        transform->rotate(rotation2);
        EntityBuilder::create()
                ->withTransform(transform)
                ->withMesh(new SkyboxStar(size, shaderProgram))
                ->build(entityManager);
        rad += diff;
    }
//    for (int i = -100; i < 101; i += 10) {
//        EntityBuilder::create()
//                ->withTransform(i, 0, 100)
//                ->withMesh(new SkyboxStar(1, shaderProgram))
//                ->build(entityManager);
//        EntityBuilder::create()
//                ->withTransform(0, i, 100)
//                ->withMesh(new SkyboxStar(1, shaderProgram))
//                ->build(entityManager);
//    }

    // X
//    auto transform = new TransformComponent(glm::vec3(50, 0, 0));
//    transform->rotate(glm::vec3(0, glm::radians(90.0f), 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(100, 0, 10));
//    transform->rotate(glm::vec3(0, -90, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(-100, 0, 0));
//    transform->rotate(glm::vec3(0, 90, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(-100, 0, 10));
//    transform->rotate(glm::vec3(0, 90, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
//
//    // Y
//    transform = new TransformComponent(glm::vec3(0, 100, 0));
//    transform->rotate(glm::vec3(90, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(10, 100, 0));
//    transform->rotate(glm::vec3(90, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(0, -100, 0));
//    transform->rotate(glm::vec3(-90, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(10, -100, 0));
//    transform->rotate(glm::vec3(-90, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
//
//    // Z
//    EntityBuilder::create()
//            ->withTransform(0, 0, -100)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    EntityBuilder::create()
//            ->withTransform(10, 0, -100)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(0, 0, 100));
//    transform->rotate(glm::vec3(180, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(1, shaderProgram))
//            ->build(entityManager);
//    transform = new TransformComponent(glm::vec3(10, 0, 100));
//    transform->rotate(glm::vec3(180, 0, 0));
//    EntityBuilder::create()
//            ->withTransform(transform)
//            ->withMesh(new SkyboxStar(2, shaderProgram))
//            ->build(entityManager);
}

void Skybox::renderEntities(RenderSystem *renderSystem, EntityManager *entityManager, ShaderProgram *shaderProgram) {
    for (auto& pair : entityManager->getTransformComponents()) {
        auto entityId = pair.first;
        auto transform = pair.second;
        auto meshes = entityManager->getMeshComponents(entityId);

        for (auto it = meshes.first; it != meshes.second; it++) {
            auto mesh = it->second;
            renderSystem->renderMesh(mesh, shaderProgram, transform->getModel());
        }
    }
}
