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

	// Make some geometry to work with
	// Cube
	GLfloat cubeVerts[] = {
		// Position				Normals					Texture
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  	0.0f,  0.0f, -1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,		0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  	0.0f,  0.0f, 1.0f,		0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  	-1.0f,  0.0f,  0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  	-1.0f,  0.0f,  0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  	-1.0f,  0.0f,  0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	1.0f,  0.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,		0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	1.0f,  0.0f,  0.0f,		1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,		1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  	0.0f, -1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f,  0.0f,		0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,		0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,		1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,		1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  	0.0f,  1.0f,  0.0f,		0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  	0.0f,  1.0f,  0.0f,		0.0f, 1.0f
	};
	// Transformations
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Set up textures
	GLuint container, awesomeFace, container2, container2_specular;
	int textureWidth, textureHeight, n;
	unsigned char *image;
	// Texture 0 - Container
	glGenTextures(1, &container);
	glBindTexture(GL_TEXTURE_2D, container);
	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	image = stbi_load("container.jpg", &textureWidth, &textureHeight, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Texture 1 - Awesome Face
	glGenTextures(1, &awesomeFace);
	glBindTexture(GL_TEXTURE_2D, awesomeFace);
	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture, flipping the image on load
	stbi_set_flip_vertically_on_load(1);
	image = stbi_load("awesomeface.png", &textureWidth, &textureHeight, &n, 4);
	stbi_set_flip_vertically_on_load(0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Texture 2 - Container2
	glGenTextures(1, &container2);
	glBindTexture(GL_TEXTURE_2D, container2);
	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	image = stbi_load("container2.png", &textureWidth, &textureHeight, &n, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Texture 3 - Container2 Specular
	glGenTextures(1, &container2_specular);
	glBindTexture(GL_TEXTURE_2D, container2_specular);
	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	image = stbi_load("container2_specular.png", &textureWidth, &textureHeight, &n, 4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Container
	GLuint boxVAO, VBO;
	glGenVertexArrays(1, &boxVAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object
	glBindVertexArray(boxVAO);
	// Copy vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	// Set vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture attributes
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	// Unbind the Vertex Array Object
	glBindVertexArray(0);
	// Create a shader
	Shader boxShader("phong.vs", "phong.fs");
	// Enable depth testing 
	glEnable(GL_DEPTH_TEST);
	// Get vertex shader uniform locations
	GLint boxModelLoc = glGetUniformLocation(boxShader.programID, "model");
	GLint boxViewLoc = glGetUniformLocation(boxShader.programID, "view");
	GLint boxProjectionLoc = glGetUniformLocation(boxShader.programID, "projection");
	// Get fragment shader uniform locations
	GLint boxViewPositionLoc = glGetUniformLocation(boxShader.programID, "viewPos");
	GLint boxMaterialDiffuseLoc = glGetUniformLocation(boxShader.programID, "material.texture_diffuse0");
	GLint boxMaterialSpecularLoc = glGetUniformLocation(boxShader.programID, "material.texture_specular0");
	GLint boxMaterialShineLoc = glGetUniformLocation(boxShader.programID, "material.shininess");
	// Lights
	Lights lights;
	GLuint dirLight1 = lights.addLight(vec3(-0.2f, -1.0f, -0.3f), vec3(0.05f, 0.05f, 0.05f), vec3(0.4f, 0.4f, 0.4f), vec3(0.5f, 0.5f, 0.5f));
	GLuint pointLight1 = lights.addLight(vec3(0.7f, 0.2f, 2.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight2 = lights.addLight(vec3(2.3f, -3.3f, -4.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight3 = lights.addLight(vec3(-4.0f, 2.0f, -12.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight4 = lights.addLight(vec3(0.0f, 0.0f, -3.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint pointLight5 = lights.addLight(vec3(0.0f, 0.0f, -3.0f), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
	GLuint spotLight1 = lights.addLight(vec3(camera.position.x, camera.position.y, camera.position.z), vec3(camera.front.x, camera.front.y, camera.front.z), glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f)), 1.0f, 0.09, 0.032, vec3(0.05f, 0.05f, 0.05f), vec3(0.8f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));

	// Light
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f, 0.2f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f, 2.0f, -12.0f),
		glm::vec3(0.0f, 0.0f, -3.0f)
	};
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	// Create a shader
	Shader lightShader("light.vs", "light.fs");
	// Get vertex shader uniform locations
	GLint lightModelLoc = glGetUniformLocation(lightShader.programID, "model");
	GLint lightViewLoc = glGetUniformLocation(lightShader.programID, "view");
	GLint lightProjectionLoc = glGetUniformLocation(lightShader.programID, "projection");

	// Variables to preserve across loops
	GLfloat gameTime = glfwGetTime();

	// Nanosuit
	Model nanoSuit("Models/NanoSuit/nanosuit.obj");

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

		// Do all the rendering
		// Background
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// View transformations
		glm::mat4 view;
		view = camera.getViewMatrix();
		// Projection transformations
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);
		// Spotlight
		lights.setPos(spotLight1, vec3(camera.position.x, camera.position.y, camera.position.z));
		lights.setDir(spotLight1, vec3(camera.front.x, camera.front.y, camera.front.z));

		// Box
		// Shader
		boxShader.use();
		// View transformation
		glUniformMatrix4fv(boxViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Projection transformation
		glUniformMatrix4fv(boxProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container2);
		glUniform1i(boxMaterialDiffuseLoc, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, container2_specular);
		glUniform1i(boxMaterialSpecularLoc, 1);
		// Material
		glUniform3f(boxMaterialSpecularLoc, 0.5f, 0.5f, 0.5f);
		glUniform1f(boxMaterialShineLoc, 32.0f);
		// Light sources
		lights.use(boxShader);
		// Camera location
		glUniform3f(boxViewPositionLoc, camera.position.x, camera.position.y, camera.position.z);
		// Geometry
		glBindVertexArray(boxVAO);
		for (GLuint i = 0; i < 10; i++) {
			// Model transformation
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angleOffset = PI / 9 * i;
			GLfloat angle = (PI / 4 * gameTime) + angleOffset;
			model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));
			glUniformMatrix4fv(boxModelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// Draw
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Nanosuit
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(-2.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		GLfloat angle = (PI / 4 * gameTime);
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(boxModelLoc, 1, GL_FALSE, glm::value_ptr(model));
		nanoSuit.draw(boxShader);

		// Light
		// Shader
		lightShader.use();
		glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		// Geometry
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++) {
			// Model transformation
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, glm::value_ptr(model));
			// Draw
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glDeleteVertexArrays(1, &boxVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
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