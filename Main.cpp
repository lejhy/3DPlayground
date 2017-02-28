#include <iostream>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265359f
// Image loading
#define STB_IMAGE_IMPLEMENTATION    
#include <stb/stb_image.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Lights.h"
#include "testingCubes.h"

//functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void click_callback(GLFWwindow* window, int button, int action, int mode);
void do_movement();

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;

// Create a camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// Controls
bool keys[1024];
GLfloat mouseX = 0, mouseY = 0;
bool firstMouseMovement = true;
int animationSpeed = 1;

// Timing
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// The MAIN function, from here we start the application and run the game loop
int main() {

	// Create variables for progress and error checking
	GLint success;
	GLchar infoLog[512];

	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3DPlayground", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	// Define the viewport dimensions
	GLint viewportWidth, viewportHeight;
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	glViewport(0, 0, viewportWidth, viewportHeight);
	GLfloat aspectRatio = (GLfloat)viewportWidth / (GLfloat)viewportHeight;

	// Assign the controls to our window
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, click_callback);

	// Create a shader
	Shader shader("phong.vert", "phong.frag");

	// Enable depth testing 
	glEnable(GL_DEPTH_TEST);

	// Lights
	Lights lights;
	GLuint dirLight1 = lights.addLight(vec3(-0.2f, -1.0f, -0.3f), vec3(0.05f, 0.05f, 0.05f), vec3(0.4f, 0.4f, 0.4f), vec3(0.5f, 0.5f, 0.5f));
	GLuint pointLight1 = lights.addLight(vec3(0.7f, 0.2f, 2.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight2 = lights.addLight(vec3(2.3f, -3.3f, -4.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight3 = lights.addLight(vec3(-4.0f, 2.0f, -12.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight4 = lights.addLight(vec3(0.0f, 0.0f, -3.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight5 = lights.addLight(vec3(0.0f, 0.0f, -3.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint spotLight1 = lights.addLight(vec3(camera.position.x, camera.position.y, camera.position.z), vec3(camera.front.x, camera.front.y, camera.front.z), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));

	// Variables to preserve across loops
	GLfloat gameTime = glfwGetTime();

	// Nanosuit
	Model nanoSuit("Models/NanoSuit/nanosuit.obj");

	// testing Cubes
	testingCubesInit();

	// Program loop
	while (!glfwWindowShouldClose(window)) {

		// Update everything
		// Time
		GLfloat time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;
		gameTime += deltaTime * animationSpeed;
		// Events
		glfwPollEvents();
		do_movement();
		// Spotlight
		lights.setPos(spotLight1, vec3(camera.position.x, camera.position.y, camera.position.z));
		lights.setDir(spotLight1, vec3(camera.front.x, camera.front.y, camera.front.z));

		// Do all the rendering
		// Shader
		shader.use();
		// Background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// View transformations
		glm::mat4 view;
		view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// Projection transformations
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 10.0f);
		glUniformMatrix4fv(glGetUniformLocation(shader.programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// Light sources
		lights.use(shader);
		// Camera location
		glUniform3f(glGetUniformLocation(shader.programID, "viewPos"), camera.position.x, camera.position.y, camera.position.z);
		
		// Nanosuit
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-2.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		GLfloat angle = (PI / 4 * gameTime);
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(shader.programID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(shader.programID, "material.shininess"), 32.0f);
		nanoSuit.draw(shader);

		// testing Cubes
		testingCubesDraw(view, projection, camera.position, gameTime, &lights);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scanCode, int action, int mode) {
	// When escape key is pressed, set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// All other key inputs
	if (action == GLFW_PRESS) {
		keys[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// Check for a mouse entering the window
	if (firstMouseMovement) {
		mouseX = xPos;
		mouseY = yPos;
		firstMouseMovement = false;
	}
	// Calculate values
	GLfloat xOffset = xPos - mouseX;
	GLfloat yOffset = mouseY - yPos;
	mouseX = xPos;
	mouseY = yPos;
	// Pass it on to the camera
	camera.processMouseMovement(xOffset, yOffset, true);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
	// Pass it on to the camera
	camera.processMouseScroll(yOffset);
}

void click_callback(GLFWwindow* window, int button, int action, int mode) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (animationSpeed == 1) {
			animationSpeed = 0;
		} else {
			animationSpeed = 1;
		}
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		if (animationSpeed == -1) {
			animationSpeed = 0;
		}
		else {
			animationSpeed = -1;
		}
	}
}

void do_movement() {
	// WASD Movement
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_X])
		camera.processKeyboard(UP, deltaTime);
	if (keys[GLFW_KEY_Z])
		camera.processKeyboard(DOWN, deltaTime);
}