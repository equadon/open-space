#include "GLContext.h"

GLContext::GLContext(EntityManager *entityManager, std::string title, uint16_t width, uint16_t height)
        : entityManager(entityManager), title(title), width(width), height(height) {
}

GLContext::~GLContext() {
    glfwTerminate();
}

glm::mat4 GLContext::getView() {
    auto cameraId = getCamera()->id;
    auto cameraComponent = entityManager->getCameraComponent(cameraId);
    auto positionComponent = entityManager->getPositionComponent(cameraId);
    return cameraComponent->getView(positionComponent);
}

glm::mat4 GLContext::getProjection() {
    auto cameraId = getCamera()->id;
    auto cameraComponent = entityManager->getCameraComponent(cameraId);
    return cameraComponent->getProjection(getAspect());
}

void GLContext::init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("failed to initialize GLAD");
    }

    lastTime = glfwGetTime();
    deltaTime = 0;

    setupImgui();
}

void GLContext::setupImgui() {
    const char* glslVersion = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void) io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

bool GLContext::shouldClose() {
    return glfwWindowShouldClose(window);
}

void GLContext::update() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glfwPollEvents();
}

void GLContext::swapBuffers() {
    glfwSwapBuffers(window);
}

void GLContext::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
