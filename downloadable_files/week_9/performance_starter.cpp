#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>  // For SOIL2
#include <iostream>
#include <string>
#include <fstream>

#include "object_light.hpp"

// Declare objects here

int windowWidth = 800;
int windowHeight = 600;
float aspect = (float)windowWidth / (float)windowHeight;

float yaw = -90.f;
float pitch = 0.0f;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;

bool firstMouse = true;
const float mouseSensitivity = 0.1f;

bool mouseLookEnabled = false;

bool paused = false;
double pauseStartTime = 0.0f;
double totalPausedTime = 0.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

glm::mat4 proj;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Avoid a divide-by-zero error if the window is minimized
    if (height == 0) {
        return;
    }

    // Update all our window size globals
    windowWidth = width;
    windowHeight = height;
    aspect = (float)width / (float)height;

    // Tell OpenGL the new dimensions
    glViewport(0, 0, width, height);

    //Calculate our new projection matrix
    proj = glm::perspective(
        glm::radians(45.0f),              // FOV
        aspect,                           // aspect ratio (hard-coded for now)
        0.1f,                             // near plane
        1000.0f                            // far plane
    );
}

void togglePause() {
    if (!paused) {
        paused = true;
        std::cout << "PAUSED" << std::endl;
		pauseStartTime = glfwGetTime();
    }
    else {
        paused = false;
        std::cout << "UNPAUSED" << std::endl;
		totalPausedTime += glfwGetTime() - pauseStartTime;
    }
}

void updateCameraDirection()
{
    cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection.y = sin(glm::radians(pitch));
    cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraDirection = glm::normalize(cameraDirection);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_P) {
            togglePause();
        }
        if (key == GLFW_KEY_M) {
            mouseLookEnabled = !mouseLookEnabled;

            if (mouseLookEnabled) {
                std::cout << "Mouse-look: ENABLED" << std::endl;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                firstMouse = true;   // prevent jump when re-enabling
            }
            else {
				std::cout << "Mouse-look: DISABLED" << std::endl;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
    }
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	if (!mouseLookEnabled) {
        return;
    }
    
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;   // reversed
    lastX = xpos;
    lastY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Constrain pitch
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateCameraDirection();
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 15.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraDirection;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraDirection;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraDirection, cameraUp)) * cameraSpeed;
}

void init(GLFWwindow* window) {

    // Load Objects here
    // Load Textures here

    proj = glm::perspective(
        glm::radians(45.0f),              // FOV
        aspect,                  // aspect ratio (hard-coded for now)
        0.1f,                             // near plane
        1000.0f                            // far plane
    );
	glEnable(GL_DEPTH_TEST);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //define light source(s)
    Light light;
    light.position = glm::vec3(0.0f, 5.0f, 0.0f); //world space coords
    light.ambient = glm::vec3(0.6f);
    light.diffuse = glm::vec3(1.0f);
    light.specular = glm::vec3(1.0f);
    light.attenuation = glm::vec3(1.0f, 0.0f, 0.0f);
    
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

    // Position, transform, draw objects here

}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Needed for MacOS
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Matrix Stacking", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glewExperimental = GL_TRUE;  // Ensures all extensions are loaded including Core Profiles
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glfwSwapInterval(1);  // enables Vsync3

    init(window);

    double animationTime = 0.0f;
    while (!glfwWindowShouldClose(window)) {
		if (!paused) {
            animationTime = glfwGetTime() - totalPausedTime;
        }
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        processInput(window);
        display(window, animationTime);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Add object cleanup here

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
