#include <iostream>

// Mathematics
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
#include "shader.h"

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;

//functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

	// Assign the key callback to our window
	glfwSetKeyCallback(window, key_callback);

	// Make some geometry to work with
	// Square
	GLfloat squareVerts[] = {
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
	};
	// Initialize variables
	GLuint squareVAO, squareVBO, squareEBO;
	glGenVertexArrays(1, &squareVAO);
	glGenBuffers(1, &squareVBO);
	glGenBuffers(1, &squareEBO);
	// Bind the Vertex Array Object
	glBindVertexArray(squareVAO);
		// Copy vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(squareVerts), squareVerts, GL_STATIC_DRAW);
		// Set vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// Set texture attributes pointers
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	// Unbind the Vertex Array Object
	glBindVertexArray(0);

	// Create a shader
	Shader shader("vertexShader.vs", "fragmentShader.fs");
	// Enable depth testing 
	glEnable(GL_DEPTH_TEST);

	// Set textures
	GLuint container, awesomeFace;
	int textureWidth, textureHeight, n;
	unsigned char *image;
	// Container
	glGenTextures(1, &container);
	glBindTexture(GL_TEXTURE_2D, container);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load and generate the texture
	image = stbi_load("container.jpg", &textureWidth, &textureHeight, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Awesome Face
	glGenTextures(1, &awesomeFace);
	glBindTexture(GL_TEXTURE_2D, awesomeFace);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	// Program loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Do all the rendering

		// Time
		GLfloat time = glfwGetTime();
		
		// Background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Shader
		shader.use();

		// Transformations
		// Model
		glm::mat4 model;
		model = glm::rotate(model, PI/4*time, glm::vec3(0.5f, 1.0f, 0.0f));
		GLint modelLoc = glGetUniformLocation(shader.Program, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// View
		glm::mat4 view;
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		GLint viewLoc = glGetUniformLocation(shader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// Projection
		glm::mat4 projection;
		projection = glm::perspective(PI/4, aspectRatio, 0.1f, 100.0f);
		GLint projectionLoc = glGetUniformLocation(shader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, container);
		glUniform1i(glGetUniformLocation(shader.Program, "texture0"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, awesomeFace);
		glUniform1i(glGetUniformLocation(shader.Program, "texture1"), 1);

		// Square
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(squareVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When escape key is pressed, set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}