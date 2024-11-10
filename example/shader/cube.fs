#version 330 core

out vec4 FragColor;   // Final color output for the fragment

in vec3 FragPos;      // Fragment position from vertex shader
in vec3 Normal;       // Normal vector from vertex shader
in vec2 TexCoord;     // Texture coordinates from vertex shader

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D texture1; // Texture sampler

void main()
{
    // Normalize the normal and light direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse shading (Lambert's cosine law)
    float diff = max(dot(norm, lightDir), 0.0);

    // Ambient lighting
    vec3 ambient = 0.1 * lightColor; // Light intensity * ambient factor

    // Diffuse lighting
    vec3 diffuse = diff * lightColor; // Diffuse contribution

    // Combine ambient and diffuse lighting
    vec3 result = ambient + diffuse;

    // Sample the texture
    vec3 texColor = texture(texture1, TexCoord).rgb;

    // Final color is a mix of texture color and lighting
    FragColor = vec4(result * texColor, 1.0); // Apply lighting to the texture color
}
