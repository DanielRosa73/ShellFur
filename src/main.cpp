#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include <camera.h>
#include <graphics/shader.h>

#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1440

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& cam, float deltaTime);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

#include <vector>

std::vector<float> generateCubeVertices() {
    // Vertices for a complete cube
    std::vector<float> vertices = {
        -1.0f, -1.0f, -1.0f, // 0. Left-Bottom-Back
         1.0f, -1.0f, -1.0f, // 1. Right-Bottom-Back
        -1.0f,  1.0f, -1.0f, // 2. Left-Top-Back
         1.0f,  1.0f, -1.0f, // 3. Right-Top-Back
        -1.0f, -1.0f,  1.0f, // 4. Left-Bottom-Front
         1.0f, -1.0f,  1.0f, // 5. Right-Bottom-Front
        -1.0f,  1.0f,  1.0f, // 6. Left-Top-Front
         1.0f,  1.0f,  1.0f  // 7. Right-Top-Front
    };

    return vertices;
}

std::vector<unsigned int> generateCubeIndices() {
    // Indices for drawing cube triangles
    std::vector<unsigned int> indices = {
        // Back face
        0, 1, 2, // First triangle
        2, 1, 3, // Second triangle
        // Front face
        4, 5, 6,
        6, 5, 7,
        // Left face
        4, 0, 6,
        6, 0, 2,
        // Right face
        1, 5, 3,
        3, 5, 7,
        // Bottom face
        0, 1, 4,
        4, 1, 5,
        // Top face
        2, 3, 6,
        6, 3, 7
    };

    return indices;
}

int main() {
    // GLFW initialization and window setup...
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Fur Rendering", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::vector<float> cubeVertices = generateCubeVertices();
    std::vector<unsigned int> cubeIndices = generateCubeIndices();

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(unsigned int), cubeIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load the fur shaders
    Shader furShader("./shaders/vertex.glsl", "./shaders/fragment.glsl"); //, "./shaders/geometry.glsl");

    Camera cam(WINDOW_HEIGHT, WINDOW_WIDTH);
    glfwSetWindowUserPointer(window, &cam);

    double lastTime = glfwGetTime();
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        processInput(window, cam, deltaTime);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        furShader.use();
        furShader.setMat4("view", cam.getViewMatrix());
        furShader.setMat4("projection", cam.getProjectionMatrix());
        // Set any additional uniforms for the fur shader

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}



float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
bool xKeyPressed = false;
bool wireframeMode = false;

// Fonction pour gérer les entrées clavier
void processInput(GLFWwindow* window, Camera& cam, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glm::vec3 initialCamPosition = cam.getPosition();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.moveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.moveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.moveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.moveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cam.moveDown(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.moveUp(deltaTime);
    glm::vec3 camPosition = cam.getPosition();

    bool xKeyDown = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
	if (xKeyDown && !xKeyPressed) {
		wireframeMode = !wireframeMode;
		if (wireframeMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	xKeyPressed = xKeyDown;
}

// Fonction de rappel appelée à chaque fois que la fenêtre est redimensionnée
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Inversé puisque y-coordonnées vont de bas en haut
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    Camera* camera = reinterpret_cast<Camera*>(glfwGetWindowUserPointer(window));
    camera->rotate(xoffset, yoffset);
}