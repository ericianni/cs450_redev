#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 mv;
uniform mat4 p;
uniform mat3 n;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    fragPos  = vec3(mv * vec4(aPos, 1.0));
    normal   = n * aNormal;
    texCoord = aTexCoord;

    gl_Position = p * mv * vec4(aPos, 1.0);
}