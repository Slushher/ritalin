#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <texture.hpp>
#include <camera.hpp>
#include <cube.hpp>
#include <shader.hpp>
#include <imgui.hpp>
#include <model.hpp>

const float SCR_WIDTH = 1280.f;
const float SCR_HEIGHT = 720.f;

glm::vec3 lightPos(1.f, 3.0f, -3.f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
int fps = 0;

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint ModelVS = compileShader(GL_VERTEX_SHADER, "shader/model.vs");
    GLuint ModelFS = compileShader(GL_FRAGMENT_SHADER, "shader/model.fs");
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, ModelVS);
    glAttachShader(shaderProgram, ModelFS);
    glLinkProgram(shaderProgram);

    std::vector<glm::vec3> vertices, normals;
    if (!loadOBJ("example/snowman.obj", vertices, normals))
    {
        std::cerr << "Failed to load OBJ" << std::endl;
        return -1;
    }
    unsigned int objVAO, objVBO;
    glGenVertexArrays(1, &objVAO);
    glGenBuffers(1, &objVBO);

    glBindVertexArray(objVAO);
    glBindBuffer(GL_ARRAY_BUFFER, objVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) * 2, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), normals.size() * sizeof(glm::vec3), &normals[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(vertices.size() * sizeof(glm::vec3)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    initIMGUI(window);
    Shader shader("example/shader/cube.vs", "example/shader/cube.fs");
    GLuint texture = LoadTexture("example/tex.png", 0, 0);

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    while (!glfwWindowShouldClose(window))
    {
        // fps counter
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1 / deltaTime;

        processInput(window);
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setupIMGUI();
        shader.use();
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 256.0f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
        shader.setMat4("model", model);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("objectColor", glm::vec3(0.8f, 0.2f, 0.3f));

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 objModel = glm::mat4(1.0f);
        glm::mat3 rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f)));
        objModel = glm::rotate(objModel, glm::radians(-90.f), glm::vec3(1.0f, 0.0f, 0.0f));
        objModel = glm::scale(objModel, glm::vec3(0.5f));
        objModel = glm::translate(objModel, glm::vec3(0.0f, 0.0f, -2.0f));

        shader.setMat4("model", objModel);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        shader.setVec3("lightPos", lightPos);
        shader.setVec3("viewPos", cameraPos);
        shader.setVec3("lightColor", glm::vec3(1.0f));
        shader.setVec3("objectColor", glm::vec3(0.8f, 0.2f, 0.3f));
        shader.setMat3("rotation", rotationMatrix);

        glBindVertexArray(objVAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        drawIMGUI(fps);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    shutdownIMGUI();

    glfwTerminate();
    return 0;
}
