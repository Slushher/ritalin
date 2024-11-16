#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <limits>
#include <iostream>
#include <tiny_obj_loader.hpp> 

struct AABB {
    glm::vec3 min;
    glm::vec3 max;

    AABB(glm::vec3 min_, glm::vec3 max_) : min(min_), max(max_) {}
};

struct Model {
    std::string name;
    glm::mat4 transform;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
};

struct Object {
    glm::mat4 transform;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices; // Add this line
    GLuint vao;
    GLuint vbo;
};

AABB computeAABB(const tinyobj::attrib_t& attrib);Model loadModel(const std::string &filename, const glm::mat4 &initialTransform);
void setupVAO(Object& obj);