#version 410 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Light {
    vec3 position;        // already in view space
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};
uniform Light uLight;

float constant  = uLight.attenuation.x;
float linear    = uLight.attenuation.y;
float quadratic = uLight.attenuation.z;

uniform vec3  uObjectColor;
uniform float uShininess;
uniform sampler2D uTex;
uniform int useTexture;

out vec4 FragColor;

void main()
{
    vec3 N = normalize(normal);
    vec3 L = normalize(uLight.position - fragPos);
    vec3 V = normalize(-fragPos);
    vec3 H = normalize(L + V);

    // Distance attenuation (point light)
	float distance	  =	length(uLight.position - fragPos);
	float attenuation =	1.0f / (constant	+ linear * distance	+ quadratic	* distance * distance);

    vec3 ambient = uLight.ambient;
    vec3 diffuse = max(dot(N, L), 0.0) * uLight.diffuse * attenuation;
    vec3 specular = pow(max(dot(N, H), 0.0), uShininess) * uLight.specular * attenuation;

    vec3 lightingColor = (ambient + diffuse + specular) * uObjectColor;
    if (useTexture == 1)
        lightingColor *= texture(uTex, texCoord).rgb;

    FragColor = vec4(lightingColor, 1.0);
}