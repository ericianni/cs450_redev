#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout(std140) uniform Camera { // New UBO code
    mat4 uV;                    // Separate View Matrix
    mat4 uP;                    // Separate Projection Matrix
};
uniform mat4 uM;                // Separate Model Matrix

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

void main()
{
    mat4 mv = uV * uM;                      // Create a new Model-View Matrix
    fragPos  = vec3(mv * vec4(aPos, 1.0));  // updated to use the new mv variable
    mat3 n = mat3(transpose(inverse(mv)));  // Calc inv transpose in shader now
    normal = n * aNormal;                   // use the new n variable
    texCoord = aTexCoord;

    gl_Position = uP * mv * vec4(aPos, 1.0); // Updated to use the new mv variable
}