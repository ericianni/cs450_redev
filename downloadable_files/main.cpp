#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>  // Basic include for GLM
#include <SOIL2/SOIL2.h>  // For SOIL2
#include <iostream>

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Test GLM
    glm::vec3 vec(1.0f, 0.0f, 0.0f);
    std::cout << "GLM vector: " << vec.x << std::endl;

    // Test SOIL2 (load a dummy image; replace with a real path for full test)
    int width, height;
    unsigned char* image = SOIL_load_image("C:\\GL\\SOIL2\\bin\\img_mars.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "SOIL2 failed to load image" << std::endl;
    }
    else {
        std::cout << "SOIL2 image loaded: " << width << "x" << height << std::endl;
        SOIL_free_image_data(image);
    }

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}