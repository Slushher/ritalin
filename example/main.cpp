#define STB_IMAGE_IMPLEMENTATION

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <texture.hpp>
#include <sprite.hpp>

#include <camera.hpp>
#include <cube.hpp>
#include <shader.hpp>

// Definitions
const float SCR_WIDTH = 1280.f;
const float SCR_HEIGHT = 720.f;

// Debugging
int success;
char infoLog[512];
bool freeCursor = false;
bool debugging = false;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
int fps = 0;

// Lighting
glm::vec3 lightPos(0.f, 6.0f, 0.f);

int image_load(const char image_path[])
{
    unsigned int texture;
    int image_w, image_h, image_channels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Image handling
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(image_path, &image_w, &image_h, &image_channels, 0);
    if (debugging)
    {
        printf("Loaded a image, path:%s, res:%ix%i, channels: %i\n", image_path, image_w, image_h, image_channels);
    }
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_w, image_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "ERROR::TEXTURE::LOADING_FAILED\n";
    }
    stbi_image_free(data);
    return texture;
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, 144);
    GLFWmonitor *primary = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(primary);
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "_floating_ Project", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!freeCursor)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // GLFW_CURSOR_DISABLED
    }
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glEnable(GL_DEPTH_TEST);
    if (debugging)
    {
        // Print OpenGL and GLSL version
        printf("Version GL: %s\n", glGetString(GL_VERSION));
        printf("Version GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    //  Light VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Shader shader("example/shader/cube.vs", "example/shader/cube.fs");
    GLuint texture = LoadTexture("example/tex1.png", 0, 0);

    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0); // Offset is 0
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); // Offset is 3 floats (position)
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); // Offset is 6 floats (position + normal)
    glEnableVertexAttribArray(2);



    while (!glfwWindowShouldClose(window))
    {
        // fps counter
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fps = 1 / deltaTime;

        // input
        processInput(window);
        // render
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // draw

        shader.use();

        // View and projection matrix
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(fov), SCR_WIDTH / SCR_HEIGHT, 0.1f, 256.0f);
        glm::mat4 model = glm::mat4(1.0f);                           // Identity matrix
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f)); // Translate the cube along the Z-axis
        shader.setMat4("model", model);                              // Set the model matrix for the shader
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Light and camera uniforms
        shader.setVec3("lightPos", lightPos);                       // Light position
        shader.setVec3("viewPos", cameraPos);                       // Camera position
        shader.setVec3("lightColor", glm::vec3(1.0f));              // White light
        shader.setVec3("objectColor", glm::vec3(0.8f, 0.2f, 0.3f)); // Cube's color (light red)
        glBindTexture(GL_TEXTURE_2D, texture);
        // Draw cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // edges_shader.use();
        // edges_shader.setMat4("projection", projection);
        // edges_shader.setMat4("view", view);

        ImGui::Begin("ImGUI");
        ImGui::Text("Player XYZ : %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
        ImGui::Text("Player direction : %f, %f, %f", direction.x, direction.y, direction.z);
        ImGui::Text("FPS : %i", fps);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}