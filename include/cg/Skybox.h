#ifndef CG1_PROJECT_SKYBOX_H
#define CG1_PROJECT_SKYBOX_H

#include <cg/GLHeader.h>
#include <cg/entities/components/MeshComponent.h>

class Skybox : public MeshComponent {
public:
    Skybox(glm::vec3 size, ShaderProgram *shaderProgram);
    Skybox(glm::vec3 size, std::string textureFilename, ShaderProgram *shaderProgram);
    Skybox(glm::vec3 size, glm::vec3 position, ShaderProgram *shaderProgram);

private:
    void createMesh();

    void createTexture(glm::vec3 position);
    void createTexture(std::string filename);

    void createTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);

    glm::vec3 size;
};

#endif //CG1_PROJECT_SKYBOX_H
