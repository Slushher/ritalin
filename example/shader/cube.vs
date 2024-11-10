#version 330 core

layout (location = 0) in vec3 aPos;       // Vertex position (3 floats)
layout (location = 1) in vec3 aNormal;    // Normal vector (3 floats)
layout (location = 2) in vec2 aTexCoord;  // Texture coordinates (2 floats)

out vec3 FragPos;  // Output: Fragment position
out vec3 Normal;   // Output: Normal for lighting calculations
out vec2 TexCoord; // Output: Texture coordinates

uniform mat4 model;       // Model transformation matrix
uniform mat4 view;        // View (camera) transformation matrix
uniform mat4 projection;  // Projection transformation matrix

void main()
{
    // Calculate the position of the vertex in world space
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Transform the normal to world space (taking inverse transpose of model matrix for proper transformation)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass the texture coordinates to the fragment shader
    TexCoord = aTexCoord;
    
    // Calculate the final vertex position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
