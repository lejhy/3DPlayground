#include <iostream>

// Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define PI 3.14159265359
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
	int viewportWidth, viewportHeight;
	glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
	glViewport(0, 0, viewportWidth, viewportHeight);

	// Assign the key callback to our window
	glfwSetKeyCallback(window, key_callback);

	// Make some geometry to work with
	// Triangle
	GLfloat triangleVerts[] = {
		// Positions		// Colors
		-0.5f, -0.5f, 0.0f, 0.8f, 0.0f, 0.0f,	// Bottom right
		0.5f, -0.5f, 0.0f,	0.0f, 0.8f, 0.0f,	// Bottom left
		0.0f,  0.8f, 0.0f,	0.0f, 0.0f, 0.8f	// Top
	};
	// Initialize variables
	GLuint triangleVAO, triangleVBO;
	glGenVertexArrays(1, &triangleVAO);
	glGenBuffers(1, &triangleVBO);
	// Bind the Vertex Array Object
	glBindVertexArray(triangleVAO);
		// Copy vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVerts), triangleVerts, GL_STATIC_DRAW);
		// Set vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Set color attributes pointers
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	// Unbind the Vertex Array Object
	glBindVertexArray(0);

	// Square
	GLfloat squareVerts[] = {
		0.5f,  0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,   // Top Right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,   // Bottom Right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,   // Bottom Left
		-0.5f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f    // Top Left 
	};
	GLuint squareIndices[] = {
		0, 1, 3,	// First Triangle
		1, 2, 3		// Second Triangle
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
		// Bind the Element Buffer Object
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
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
		glClear(GL_COLOR_BUFFER_BIT);

		// Shader
		shader.use();

		// Transformations
		glm::mat4 transform;
		double timeScale = (sin(time) / 2) + 0.5;
		transform = glm::rotate(transform, (float)(time * PI / 3), glm::vec3(0.0, 0.0, 1.0));
		transform = glm::scale(transform, glm::vec3(timeScale, timeScale, timeScale));
		GLuint transformLoc = glGetUniformLocation(shader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		//// Uniform
		//GLfloat colorIntensity = (sin(time) / 2) + 0.5;
		//GLint colorIntensityLocation = glGetUniformLocation(shader, "colorIntensity");
		//glUniform1f(colorIntensityLocation, colorIntensity);

		//// Triangle
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glBindVertexArray(triangleVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glBindVertexArray(0);

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
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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