#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <model.hpp>

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec3>& out_normals);
GLuint compileShader(GLenum type, const char* source);