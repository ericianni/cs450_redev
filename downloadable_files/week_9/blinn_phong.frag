#version 410 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

layout(std140) uniform LightBlock {  // replace the struct and uLight uniform
    vec3  lightPosition;
    float pad0;
    vec3  lightAmbient;
    float pad1;
    vec3  lightDiffuse;
    float pad2;
    vec3  lightSpecular;
    float pad3;
    vec3  lightAttenuation;
    float pad4;
};

float constant  = lightAttenuation.x;   // Update variable name
float linear    = lightAttenuation.y;   // Update variable name
float quadratic = lightAttenuation.z;   // Update variable name

uniform float uTime;

uniform vec3  uObjectColor;
uniform float uShininess;
uniform sampler2D uTex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(lightPosition - fragPos);   // Update variable name
    vec3 V = normalize(-fragPos);
    vec3 H = normalize(L + V);

    // Distance attenuation (point light)
    float distance = length(lightPosition - fragPos);  // Update variable name
    float attenuation = 1.0f / (constant + linear * distance + quadratic * distance * distance);

    vec3 ambient = lightAmbient;  // Update variable name
    vec3 diffuse = max(dot(N, L), 0.0) * lightDiffuse * attenuation; // Update variable name
    vec3 specular = pow(max(dot(N, H), 0.0), uShininess) * lightSpecular * attenuation;  // Update variable name

    vec3 lightingColor = (ambient + diffuse + specular) * uObjectColor;
    if (useTexture == 1)
        lightingColor *= texture(uTex, texCoord).rgb;

    FragColor = vec4(lightingColor, 1.0);
}