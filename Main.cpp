#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 1920, HEIGHT = 1080;

// Shaders
const GLchar *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"out vec4 vertexColor;\n"
"void main () {\n"
"	gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"	vertexColor = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar *fragmentShaderSource =
"#version 330 core\n"
"out vec4 color;\n"
"in vec4 vertexColor;\n"
"void main () {\n"
"	color = vertexColor;\n"
"}\0";

//functions
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The MAIN function, from here we start the application and run the game loop
int main() {
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
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Assign the key callback to our window
	glfwSetKeyCallback(window, key_callback);

	// Create variables for progress and error checking
	GLint success;
	GLchar infoLog[512];

	// Compile a vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Compile a fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Set up a shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Make some geometry to work with
	// Triangle
	GLfloat triangleVerts[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.8f, 0.0f
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	// Unbind the Vertex Array Object
	glBindVertexArray(0);

	// Square
	GLfloat squareVerts[] = {
		0.5f,  0.5f, 0.0f,	// Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,	// Bottom Left
		-0.5f,  0.5f, 0.0f	// Top Left 
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	// Unbind the Vertex Array Object
	glBindVertexArray(0);

	// Program loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Do all the rendering
		
		// Background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Shader
		glUseProgram(shaderProgram);

		// Square
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(squareVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Triangle
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(triangleVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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