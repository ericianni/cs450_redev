// object_light.hpp
#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#define numVAOs 1
#define numVBOs 3

const GLuint CAMERA_BINDING_POINT = 0;
const GLuint LIGHT_BINDING_POINT  = 1;

struct Light {
    glm::vec3 position {0.0f, 0.0f, 0.0f};
    glm::vec3 ambient  {0.15f, 0.15f, 0.15f};
    glm::vec3 diffuse  {0.8f, 0.8f, 0.8f};
    glm::vec3 specular {1.0f, 1.0f, 1.0f};
	glm::vec3 attenuation{ 0.5f, 0.03f, 0.003f };
};

struct LightUBO {
    glm::vec3 lightPosition;
    float     pad0;
    glm::vec3 lightAmbient;
    float     pad1;
    glm::vec3 lightDiffuse;
    float     pad2;
    glm::vec3 lightSpecular;
    float     pad3;
    glm::vec3 lightAttenuation;
    float     pad4;
};

class Object {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    int numVertices = 0;

    GLuint VAO[numVAOs];
    GLuint VBO[numVBOs];

    GLuint program = 0;
    GLuint textureID = 0;
    glm::mat4 modelMatrix{1.0f};
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    float shininess = 32.0f;

    Object() = default;

    bool init(const std::string& vertPath,
              const std::string& fragPath,
              const std::string& objPath);

    bool loadTexture(const std::string& path);

    void setPosition(const glm::vec3& pos);
    void translate(const glm::vec3& delta);
    void rotate(float degrees, const glm::vec3& axis);
    void scale(const glm::vec3& s);
    void resetTransform();
    void draw(float time) const;
    void draw(const glm::mat4& modelFromStack, float time) const;
    void cleanup();
    void setColor(const glm::vec3& c);
    void setShininess(float s);

private:
    std::string loadShaderSource(const char* filePath);
    GLuint buildShaderProgram(const char* vertPath, const char* fragPath);
    void bindUniformBlocks();
    bool loadOBJ(const std::string& path);
    void setupBuffers();
};