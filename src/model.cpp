#include <model.hpp>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.hpp>

// Computing AABB for tinyobj header loader models
AABB computeAABB(const tinyobj::attrib_t &attrib)
{
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());

    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        glm::vec3 vertex(
            attrib.vertices[i],
            attrib.vertices[i + 1],
            attrib.vertices[i + 2]);
        min = glm::min(min, vertex);
        max = glm::max(max, vertex);
    }
    return AABB(min, max);
}

Model loadModel(const std::string &filename, const glm::mat4 &initialTransform = glm::mat4(1.0f))
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());
    if (!err.empty())
    {
        std::cerr << err << std::endl;
    }
    if (!ret)
    {
        exit(1);
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;

    // Process positions, normals, and texture coordinates
    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        vertices.emplace_back(attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    for (size_t i = 0; i < attrib.normals.size(); i += 3)
    {
        normals.emplace_back(attrib.normals[i], attrib.normals[i + 1], attrib.normals[i + 2]);
    }

    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
    {
        texCoords.emplace_back(attrib.texcoords[i], attrib.texcoords[i + 1]);
    }

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            unsigned int vIdx = index.vertex_index; // Position index
            if (vIdx < vertices.size())
            {
                indices.push_back(vIdx);
            }
        }
    }
    computeAABB(attrib);
    // Return the Model with updated indices
    return {filename, initialTransform, vertices, normals, texCoords, indices};
}

void setupVAO(Object &obj)
{
    glGenVertexArrays(1, &obj.vao);
    glGenBuffers(1, &obj.vbo);
    GLuint ebo;

    glBindVertexArray(obj.vao);

    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    std::vector<Vertex> interleavedVertices;
    for (size_t i = 0; i < obj.vertices.size(); i++)
    {
        interleavedVertices.push_back({obj.vertices[i],
                                       obj.normals[i],
                                       obj.texCoords[i]});
    }

    glBindBuffer(GL_ARRAY_BUFFER, obj.vbo);
    glBufferData(GL_ARRAY_BUFFER, interleavedVertices.size() * sizeof(Vertex), interleavedVertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), obj.indices.data(), GL_STATIC_DRAW);

    const int stride = sizeof(Vertex);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
}