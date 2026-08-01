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

Object room;

Object teapot_low_flat;
Object teapot_low_gouraud;
Object teapot_low_phong;

Object teapot_med_flat;
Object teapot_med_gouraud;
Object teapot_med_phong;

Object teapot_high_flat;
Object teapot_high_gouraud;
Object teapot_high_phong;

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

bool low = false;
bool med = false;
bool high = false;

glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 8.0f);
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
        100.0f                            // far plane
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

void toggleModel(int model) {
    if (model == 1) {
        low = !low;
    } else if (model == 2) {
        med = !med;
    } else if (model == 3) {
        high = !high;
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
        if ( key == GLFW_KEY_1) {
            toggleModel(1);
        }
        if (key == GLFW_KEY_2) {
            toggleModel(2);
        }
        if (key == GLFW_KEY_3) {
            toggleModel(3);
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
    const float cameraSpeed = 2.5f * deltaTime;

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

    // Cube for the room
    if (!room.init("phong.vert", "phong.frag", "room.obj")) {
        std::cerr << "Failed to load room" << std::endl;
	}
    room.setShininess(8.0f);
    
    // Flat shaders
    if (!teapot_low_flat.init("flat.vert", "flat.frag", "teapot_low.obj")) {
        std::cerr << "Failed to load teapot_low_flat" << std::endl;
    }
    teapot_low_flat.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_low_flat.setShininess(100.0f);

    if (!teapot_med_flat.init("flat.vert", "flat.frag", "teapot_med.obj")) {
        std::cerr << "Failed to load teapot_med_flat" << std::endl;
    }

    teapot_med_flat.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_med_flat.setShininess(100.0f);

    if (!teapot_high_flat.init("flat.vert", "flat.frag", "teapot_high.obj")) {
        std::cerr << "Failed to load teapot_high_flat" << std::endl;
	}
	teapot_high_flat.setColor(glm::vec3{ 0.84f, 0.25f, 0.035f });
	teapot_high_flat.setShininess(100.0f);

    // Gouraud shaders
    if (!teapot_low_gouraud.init("gouraud.vert", "gouraud.frag", "teapot_low.obj")) {
        std::cerr << "Failed to load teapot_low_gouraud" << std::endl;
    }
    teapot_low_gouraud.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_low_gouraud.setShininess(100.0f);

    if (!teapot_med_gouraud.init("gouraud.vert", "gouraud.frag", "teapot_med.obj")) {
        std::cerr << "Failed to load teapot_med_gouraud" << std::endl;
    }

    teapot_med_gouraud.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_med_gouraud.setShininess(100.0f);

    if (!teapot_high_gouraud.init("gouraud.vert", "gouraud.frag", "teapot_high.obj")) {
        std::cerr << "Failed to load teapot_high_gouraud" << std::endl;
	}
	teapot_high_gouraud.setColor(glm::vec3{ 0.84f, 0.25f, 0.035f });
	teapot_high_gouraud.setShininess(100.0f);

    // Phong shaders
    if (!teapot_low_phong.init("phong.vert", "phong.frag", "teapot_low.obj")) {
        std::cerr << "Failed to load teapot_low_phong" << std::endl;
    }
    teapot_low_phong.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_low_phong.setShininess(100.0f);

    if (!teapot_med_phong.init("phong.vert", "phong.frag", "teapot_med.obj")) {
        std::cerr << "Failed to load teapot_med_phong" << std::endl;
    }

    teapot_med_phong.setColor(glm::vec3{0.84f, 0.25f, 0.035f});
    teapot_med_phong.setShininess(100.0f);

    if (!teapot_high_phong.init("phong.vert", "phong.frag", "teapot_high.obj")) {
        std::cerr << "Failed to load teapot_high_phong" << std::endl;
	}
	teapot_high_phong.setColor(glm::vec3{ 0.84f, 0.25f, 0.035f });
	teapot_high_phong.setShininess(100.0f);

    proj = glm::perspective(
        glm::radians(45.0f),              // FOV
        aspect,                  // aspect ratio (hard-coded for now)
        0.1f,                             // near plane
        100.0f                            // far plane
    );
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //define light source(s)
    Light light;
    light.position = glm::vec3(5.0f, 5.0f, 5.0f); //world space coords
    light.ambient = glm::vec3(0.05f);
    light.diffuse = glm::vec3(0.6f);
    light.specular = glm::vec3(1.0f);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

	// Draw the cube (room)
    room.resetTransform();
	room.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    room.scale(glm::vec3(10.0f));

	room.draw(view, proj, light);

    // Flat shading
    teapot_low_flat.resetTransform();
    teapot_low_flat.setPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
    teapot_low_flat.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_low_flat.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_med_flat.resetTransform();
    teapot_med_flat.setPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
    teapot_med_flat.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_med_flat.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_high_flat.resetTransform();
    teapot_high_flat.setPosition(glm::vec3(-2.5f, 0.0f, 0.0f));
    teapot_high_flat.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_high_flat.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Gouraud shading
    teapot_low_gouraud.resetTransform();
    teapot_low_gouraud.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    teapot_low_gouraud.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_low_gouraud.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_med_gouraud.resetTransform();
    teapot_med_gouraud.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    teapot_med_gouraud.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_med_gouraud.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_high_gouraud.resetTransform();
    teapot_high_gouraud.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    teapot_high_gouraud.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_high_gouraud.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    // Blinn-Phong shading
    teapot_low_phong.resetTransform();
    teapot_low_phong.setPosition(glm::vec3(2.5f, 0.0f, .0f));
    teapot_low_phong.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_low_phong.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_med_phong.resetTransform();
    teapot_med_phong.setPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    teapot_med_phong.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_med_phong.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    teapot_high_phong.resetTransform();
    teapot_high_phong.setPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    teapot_high_phong.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_high_phong.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));

    if (low) {
        //teapot_low_flat.draw(view, proj, light);
        //teapot_low_gouraud.draw(view, proj, light);
        teapot_low_phong.draw(view, proj, light);

        teapot_low_phong.resetTransform();
    teapot_low_phong.setPosition(glm::vec3(-5.0f, -5.0f, -5.0f));
    teapot_low_phong.scale(glm::vec3(0.5f, 0.5f, 0.5f));
    teapot_low_phong.rotate((float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    teapot_low_phong.draw(view, proj, light);
    }
    if (med) {
        teapot_med_flat.draw(view, proj, light);
        //teapot_med_gouraud.draw(view, proj, light);
        //teapot_med_phong.draw(view, proj, light);
    }
	if (high) {
        teapot_high_flat.draw(view, proj, light);
        //teapot_high_gouraud.draw(view, proj, light);
        //teapot_high_phong.draw(view, proj, light);
    }
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
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Let There Be Light", NULL, NULL);
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
    teapot_low_flat.cleanup();
    teapot_med_flat.cleanup();
    teapot_high_flat.cleanup();
	teapot_low_gouraud.cleanup();
	teapot_med_gouraud.cleanup();
	teapot_high_gouraud.cleanup();
	teapot_low_phong.cleanup();
	teapot_med_phong.cleanup();
	teapot_high_phong.cleanup();
    room.cleanup();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
