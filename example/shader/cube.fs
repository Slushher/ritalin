#version 330 core
out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform sampler2D texture1;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 result = ambient + diffuse;
    vec4 texColor = texture(texture1, TexCoord);

    FragColor = vec4(result * texColor.rgb, texColor.a);
}
