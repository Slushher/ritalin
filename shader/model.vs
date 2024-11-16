#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 rotation;

void main()
{
    // Apply rotation to position if needed (or only apply rotation to normal, depending on your requirements)
    vec3 rotatedPos = rotation * aPos; // Apply rotation to position
    FragPos = vec3(model * vec4(rotatedPos, 1.0)); // Apply model transformation to rotated position

    // Apply normal transformation
    Normal = mat3(transpose(inverse(model))) * (rotation * aNormal);

    // Apply projection and view transformations
    gl_Position = projection * view * vec4(FragPos, 1.0);
}