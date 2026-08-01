#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>  // Basic include for GLM
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>  // For SOIL2
#include <iostream>
#include <string>
#include <fstream>

#define numVAOs 1
#define numVBOs 2

float vertices[] = {
    // Front face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // Back face
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // Left face
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // Right face
     0.5f,  0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,

     // Bottom face
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f, -0.5f,

     // Top face
     -0.5f,  0.5f, -0.5f,
     -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
     -0.5f,  0.5f, -0.5f
};

float colors[] = {
    // Front face (red)
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // Back face (green)
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // Left face (blue)
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // Right face (yellow)
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    // Bottom face (cyan)
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    // Top face (magenta)
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f
};

GLuint renderingProgram;

GLuint vao[numVAOs];
GLuint vbo[numVBOs];

GLint mvLoc = -1;
GLint pLoc  = -1;

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

std::string loadShaderSource(const char* filePath) {
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

GLuint buildShaderProgram() {
    // We need create our shader programs and save the ID number for each
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = loadShaderSource("shader.vert");
    std::string fragShaderStr = loadShaderSource("shader.frag");

    // We need to convert our strings into c-strings
    const char* vertShaderSrc = vertShaderStr.c_str();
    const char* fragShaderSrc = fragShaderStr.c_str();

    // We need to load the shader source code into the shader program itself
    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    // We need to compile the shader source code
    glCompileShader(vShader);
    glCompileShader(fShader);

    // We need to create our program and attach our shader
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);

    // We need to link the compiled shaders into a single program
    glLinkProgram(vfProgram);

    return vfProgram;
}

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
    renderingProgram = buildShaderProgram();

    //Get uniform location
    mvLoc = glGetUniformLocation(renderingProgram, "mv");
	pLoc = glGetUniformLocation(renderingProgram, "p");

    // Generate VAOs and VBOs
    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    // Bind VAO and Position VBO
    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

    // Fill Position VBO with vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Bind Color VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

    // Fill Color VBO with vertex data
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    // Set Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(1);

    // Unbind VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    proj = glm::perspective(
        glm::radians(45.0f),              // FOV
        aspect,                  // aspect ratio (hard-coded for now)
        0.1f,                             // near plane
        100.0f                            // far plane
    );
	glEnable(GL_DEPTH_TEST);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraDirection, cameraUp);

    glUseProgram(renderingProgram);

    // This is not a true mvp matrix,
    // we will learn how to do it correctly soon
    glm::mat4 model = glm::mat4(1.0f); // Always start with the identiy matrix
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
    model = glm::rotate(model, glm::radians((float)currentTime * 45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 mv = view * model;

    // Set Uniforms
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mv));
    glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(proj));

    // Bind VAO
    glBindVertexArray(vao[0]);

    // Draw triangle

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Unbind VAO
    glBindVertexArray(0);
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
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "LookAt Me Now", NULL, NULL);
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

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
