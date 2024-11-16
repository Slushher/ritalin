#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <shader.hpp>
#include <imgui.hpp>
#include <model.hpp>

const float SCR_WIDTH = 1280.f;
const float SCR_HEIGHT = 720.f;

glm::vec3 lightPos(1.f, 25.0f, -3.f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
int fps = 0;
glm::vec3 selectedColor(0.0f, 0.5f, 1.0f);
std::vector<Object> sceneObjects;

bool file_exists(const std::string &filepath)
{
    std::ifstream file(filepath);
    return file.good();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "_floating_ Project", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        return -1;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND); // ENABLE TRANSPARENCY
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initIMGUI(window);
    Shader shader("shader/model.vs", "shader/model.fs");
    shader.use();
    GLuint modelMatrixLocation = glGetUniformLocation(shader.ID, "model");
    GLuint viewMatrixLocation = glGetUniformLocation(shader.ID, "view");
    GLuint projectionMatrixLocation = glGetUniformLocation(shader.ID, "projection");
    if (modelMatrixLocation == -1 || viewMatrixLocation == -1 || projectionMatrixLocation == -1)
    {
        std::cerr << "Failed to find matrix uniforms in the shader program." << std::endl;
    }
    if (!file_exists("example/map1.obj"))
    {
        std::cout << "file example/map1.obj doesnt exist \n";
    }
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f, 2.0f));
    Model model = loadModel("example/map1.obj", scaleMatrix);
    Object object;
    object.vertices = model.vertices;
    object.normals = model.normals;
    object.texCoords = model.texCoords;
    object.indices = model.indices;

    setupVAO(object);
    sceneObjects.push_back(object);
    Terrain terrain;
    terrain.vertices = model.vertices;
    terrain.indices = model.indices;

    while (!glfwWindowShouldClose(window))
    {
        gravityTick(terrain);
        // fps counter
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1 / deltaTime;

        processMovement(window, terrain);
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupIMGUI();
        shader.use();

        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("objectColor", selectedColor);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 256.0f);
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat3 rotation = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
        glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
        // Set the matrices in the shader program
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        shader.setMat3("rotation", rotationMatrix);
        // Render all objects in the scene
        for (auto &obj : sceneObjects)
        {
            glBindVertexArray(obj.vao);
            glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);
        }

        // Render ImGui
        drawIMGUI(fps, selectedColor);

        // Poll events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shutdownIMGUI();

    glfwTerminate();
    return 0;
}
