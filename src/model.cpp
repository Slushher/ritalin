#include <model.hpp>

bool loadOBJ(const char* path, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec3>& out_normals) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    std::vector<unsigned int> vertexIndices, normalIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string prefix;
        lineStream >> prefix;

        if (prefix == "v") {
            // Vertex positions
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (prefix == "vn") {
            // Vertex Normals
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (prefix == "f") {
            // Faces
            unsigned int vertexIndex[3], normalIndex[3];
            for (int i = 0; i < 3; i++) {
                std::string vertex;
                lineStream >> vertex;
                sscanf(vertex.c_str(), "%u//%u", &vertexIndex[i], &normalIndex[i]);
                vertexIndices.push_back(vertexIndex[i] - 1);
                normalIndices.push_back(normalIndex[i] - 1);
            }
        }
    }
    file.close();

    // Organize vertexes and normals
    for (unsigned int i = 0; i < vertexIndices.size(); i++) {
        out_vertices.push_back(temp_vertices[vertexIndices[i]]);
        out_normals.push_back(temp_normals[normalIndices[i]]);
    }

    return true;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}