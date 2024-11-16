#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <algorithm>
#include <vector>

extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern glm::vec3 direction;
extern float fov;

// TODO: make a separate file
class Terrain {
public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    // Function to check if the player is colliding with the terrain
    bool checkCollision(const glm::vec3 &playerMin, const glm::vec3 &playerMax) {
        for (size_t i = 0; i < indices.size(); i += 3) {
            // Get the vertices of the triangle
            glm::vec3 v0 = vertices[indices[i]];
            glm::vec3 v1 = vertices[indices[i + 1]];
            glm::vec3 v2 = vertices[indices[i + 2]];

            // Check if the player bounding box intersects the triangle
            if (aabbIntersectsTriangle(playerMin, playerMax, v0, v1, v2)) {
                return true;  // Collision detected
            }
        }
        return false;  // No collision
    }

private:
    bool aabbIntersectsTriangle(const glm::vec3 &min, const glm::vec3 &max, 
                                 const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {
        float minY = std::min({v0.y, v1.y, v2.y});
        if (max.y < minY)
            return false;

        return true;
    }
};

void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void gravityTick(Terrain &terrain);
void processMovement(GLFWwindow *window, Terrain &terrain);