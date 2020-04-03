#ifndef CG1_PROJECT_GLCONTEXT_H
#define CG1_PROJECT_GLCONTEXT_H

#include <cg/GLHeader.h>
#include <cg/entities/Entity.h>
#include <cg/entities/EntityManager.h>
#include <cg/shaders/Shader.h>

class GLContext {
public:
    GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height);
    ~GLContext();

    float getAspect() { return (float) width / (float) height; }
    uint16_t getWidth() { return width; }
    uint16_t getHeight() { return height; }

    GLFWwindow* getWindow() { return window; }

    Entity* getCamera() { return activeCamera; }
    void setActiveCamera(Entity *entity) { activeCamera = entity; }

    glm::mat4 getProjection();
    glm::mat4 getView();

    double getDeltaTime() { return deltaTime; }

    void init();

    bool shouldClose();
    void update();

    void swapBuffers();

    bool debug = false;
    bool displayGui = false;
    bool displayGrid = true;

    Entity *perspectiveCamera = nullptr;
    Entity *topCamera = nullptr;
    Entity *sideCamera = nullptr;

private:
    EntityManager *entityManager;
    const std::string title;
    uint16_t width;
    uint16_t height;

    GLFWwindow *window;

    Entity *activeCamera = nullptr;

    double lastTime;
    double deltaTime;

    void setupImgui();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
};


#endif //CG1_PROJECT_GLCONTEXT_H
