#version 410 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Light {
    vec3 position;        // already in view space
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

uniform vec3  objectColor;
uniform float shininess;
uniform sampler2D tex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(light.position - fragPos);
    vec3 V = normalize(-fragPos);
    vec3 H = normalize(L + V);

    // Ambient
    vec3 ambient = light.ambient;

    // Diffuse
    float diff = max(dot(N, L), 0.0);
    vec3 diffuse = diff * light.diffuse;

    // Specular (Blinn-Phong)
    float spec = pow(max(dot(N, H), 0.0), shininess);
    vec3 specular = spec * light.specular;

    vec3 color = (ambient + diffuse + specular) * objectColor;
    if (useTexture == 1)
        color *= texture(tex, texCoord).rgb;

    FragColor = vec4(color, 1.0);
}