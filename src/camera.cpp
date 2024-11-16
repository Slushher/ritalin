#include <camera.hpp>

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 8.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 direction;
float yaw = -90.0f;
float pitch = 0.f;
float lastX = 400, lastY = 300;
int chunkLast_x, chunkLast_z = 0;
float fov = 90.0f;
bool firstMouse = true;
bool collision = false;
bool SCREEN_LOCK = false;
bool onGround = false;
float cameraSpeed = 0.5;
float gravConstant = 0.05f;
float acceleration = 0.f;

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    cameraSpeed -= (float)yoffset / 10.f;
    if (cameraSpeed < 0.1f)
        cameraSpeed = 0.1f;
    if (cameraSpeed > 1.0f)
        cameraSpeed = 1.0f;
}

glm::vec3 playerMin = glm::vec3(-0.5f, 2.0f, -0.5f);
glm::vec3 playerMax = glm::vec3(0.5f, 4.0f, 0.5f);

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        if (!SCREEN_LOCK)
        {
            SCREEN_LOCK = 1;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            SCREEN_LOCK = 0;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        collision = !collision;
        std::cout << "Collsions " << collision << "\n";
    }
}

void gravityTick(Terrain &terrain)
{
    // Check if collision with the ground has occurred
    if (collision)
    {
        // Apply gravity to the player's Y position
        acceleration -= gravConstant;
        cameraPos[1] -= acceleration;

        // Check for collisions with the terrain
        if (terrain.checkCollision(playerMin, playerMax))
        {
            cameraPos[1] = playerMin.y + 0.1f;
            onGround = true; // The player is on the ground
        }
        else
        {
            onGround = false; // The player is in the air
        }
    }
}

void processMovement(GLFWwindow *window, Terrain &terrain) {
    processInput(window);
    
    if (onGround) {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            cameraPos[1] += 1.0f;
            onGround = false;
        }
    } else {
        gravityTick(terrain);
    }
}