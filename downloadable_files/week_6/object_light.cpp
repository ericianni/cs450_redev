// object.cpp
#include "object_light.hpp"

#include <fstream>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2/SOIL2.h"

#define TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_DISABLE_FAST_FLOAT
#include "tiny_obj_loader.h"

// ------------------------------------------------------------
// public
// ------------------------------------------------------------

bool Object::init(const std::string& vertPath,
                 const std::string& fragPath,
                 const std::string& objPath)
{
    program = buildShaderProgram(vertPath.c_str(), fragPath.c_str());
    if (program == 0)
        return false;

    if (!loadOBJ(objPath))
        return false;

    setupBuffers();
    return true;
}

bool Object::loadTexture(const std::string& path)
{
    textureID = SOIL_load_OGL_texture(
        path.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);

    return textureID != 0;
}

void Object::setColor(const glm::vec3& c) {
    color = c;
}

void Object::setShininess(float s) {
    shininess = s;
}

void Object::setPosition(const glm::vec3& pos)
{
    modelMatrix[3] = glm::vec4(pos, 1.0f);
}

void Object::translate(const glm::vec3& delta)
{
    modelMatrix = glm::translate(modelMatrix, delta);
}

void Object::rotate(float degrees, const glm::vec3& axis)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(degrees), axis);
}

void Object::scale(const glm::vec3& s)
{
    modelMatrix = glm::scale(modelMatrix, s);
}

void Object::resetTransform()
{
    modelMatrix = glm::mat4(1.0f);
}

void Object::draw(const glm::mat4& view,
                  const glm::mat4& projection,
                  const Light& light) const
{
    glUseProgram(program);

    glm::mat4 mv = view * modelMatrix;

    GLint mvLoc = glGetUniformLocation(program, "uMV");
    if (mvLoc >= 0)
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
    else
        std::cout << "Failed to get uMV location" << std::endl;

    GLint pLoc = glGetUniformLocation(program, "uP");
    if (pLoc >= 0)
        glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat3 n = glm::mat3(glm::transpose(glm::inverse(mv)));
    GLint nLoc = glGetUniformLocation(program, "uN");
    
    if (nLoc >= 0)
        glUniformMatrix3fv(nLoc, 1, GL_FALSE, glm::value_ptr(n));
    
    // We must transform the light position into view space
    glm::vec3 lightPosView = glm::vec3(view * glm::vec4(light.position, 1.0f));
	
    GLint lightPosLoc = glGetUniformLocation(program, "uLight.position");
    if (lightPosLoc >= 0)
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPosView));

    GLint lightAmbLoc = glGetUniformLocation(program, "uLight.ambient");
    if (lightAmbLoc >= 0)
        glUniform3fv(lightAmbLoc, 1, glm::value_ptr(light.ambient));

    GLint lightDiffLoc = glGetUniformLocation(program, "uLight.diffuse");
    if (lightDiffLoc >= 0)
        glUniform3fv(lightDiffLoc, 1, glm::value_ptr(light.diffuse));

    GLint lightSpecLoc = glGetUniformLocation(program, "uLight.specular");
    if (lightSpecLoc >= 0)
        glUniform3fv(lightSpecLoc, 1, glm::value_ptr(light.specular));

    GLint colorLoc = glGetUniformLocation(program, "uObjectColor");
    if (colorLoc >= 0)
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    GLint shininessLoc = glGetUniformLocation(program, "uShininess");
    if (shininessLoc >= 0)
        glUniform1f(shininessLoc, shininess);
    
    GLint useTexLoc = glGetUniformLocation(program, "useTexture");
    if (useTexLoc >= 0)
        glUniform1i(useTexLoc, (textureID != 0) ? 1 : 0);
    
    if (textureID != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        GLint texLoc = glGetUniformLocation(program, "uTex");
        if (texLoc >= 0)
            glUniform1i(texLoc, 0);
    }

    glBindVertexArray(VAO[0]);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    glBindVertexArray(0);
}

void Object::cleanup()
{
    glDeleteBuffers(numVBOs, VBO);
    glDeleteVertexArrays(numVAOs, VAO);

    if (program)   { glDeleteProgram(program);        program = 0; }
    if (textureID) { glDeleteTextures(1, &textureID); textureID = 0; }
}

// ------------------------------------------------------------
// private – shader helpers
// ------------------------------------------------------------

std::string Object::loadShaderSource(const char* filePath)
{
    std::string source;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    }
    else {
        std::cout << "Loading shader: " << filePath << std::endl;
    }

    std::string line;
    while (std::getline(fileStream, line)) {
        source.append(line + "\n");
    }

    fileStream.close();
    return source;
}

GLuint Object::buildShaderProgram(const char* vertPath, const char* fragPath)
{
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = loadShaderSource(vertPath);
    std::string fragShaderStr = loadShaderSource(fragPath);

    if (vertShaderStr.empty() || fragShaderStr.empty()) {
        glDeleteShader(vShader);
        glDeleteShader(fShader);
        return 0;
    }

    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    glCompileShader(fShader);

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return vfProgram;
}

// ------------------------------------------------------------
// private – geometry
// ------------------------------------------------------------

bool Object::loadOBJ(const std::string& path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());
    if (!warn.empty())
        std::cout << "WARN: " << warn << "\n";
    if (!err.empty())
        std::cerr << "ERR: " << err << "\n";
    if (!ok)
        return false;

    vertices.clear();
    normals.clear();
    texCoords.clear();

    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            int fv = shape.mesh.num_face_vertices[f];

            for (size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                if (idx.normal_index >= 0) {
                    normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                    normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                    normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
                } else {
                    normals.push_back(0.0f);
                    normals.push_back(0.0f);
                    normals.push_back(1.0f);
                }

                if (idx.texcoord_index >= 0) {
                    texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
                    texCoords.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
                } else {
                    texCoords.push_back(0.0f);
                    texCoords.push_back(0.0f);
                }
            }
            index_offset += fv;
        }
    }

    numVertices = static_cast<int>(vertices.size() / 3);
    return true;
}

void Object::setupBuffers()
{
    glGenVertexArrays(numVAOs, VAO);
    glGenBuffers(numVBOs, VBO);

    glBindVertexArray(VAO[0]);

    // Position → location 0
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal → location 1
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(float),
                 normals.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    // TexCoord → location 2
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER,
                 texCoords.size() * sizeof(float),
                 texCoords.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}