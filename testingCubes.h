#pragma once

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
// Others
#include "Shader.h"
#include "Lights.h"

// Vertex data
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
// Container Transformations
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
// Light Transformations
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f, 0.2f, 2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f, 2.0f, -12.0f),
	glm::vec3(0.0f, 0.0f, -3.0f)
};

// Textures
GLuint container, awesomeFace, container2, container2_specular;
int textureWidth, textureHeight, n;
unsigned char *image;

// Buffers
GLuint boxVAO, VBO, lightVAO;

void testingCubesInit() {
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
	// Enable depth testing 
	glEnable(GL_DEPTH_TEST);
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void testingCubesDraw(glm::mat4 view, glm::mat4 projection, vec3 cameraPosition, GLfloat gameTime, Lights *lights) {
	// Shaders
	Shader boxShader("phong.vs", "phong.fs");
	Shader lightShader("light.vs", "light.fs");
	// Get vertex shader uniform locations
	GLint boxModelLoc = glGetUniformLocation(boxShader.programID, "model");
	GLint boxViewLoc = glGetUniformLocation(boxShader.programID, "view");
	GLint boxProjectionLoc = glGetUniformLocation(boxShader.programID, "projection");
	// Get fragment shader uniform locations
	GLint boxViewPositionLoc = glGetUniformLocation(boxShader.programID, "viewPos");
	GLint boxMaterialDiffuseLoc = glGetUniformLocation(boxShader.programID, "material.texture_diffuse0");
	GLint boxMaterialSpecularLoc = glGetUniformLocation(boxShader.programID, "material.texture_specular0");
	GLint boxMaterialShineLoc = glGetUniformLocation(boxShader.programID, "material.shininess");

	GLint lightModelLoc = glGetUniformLocation(lightShader.programID, "model");
	GLint lightViewLoc = glGetUniformLocation(lightShader.programID, "view");
	GLint lightProjectionLoc = glGetUniformLocation(lightShader.programID, "projection");

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
	// Camera location
	glUniform3f(boxViewPositionLoc, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	// Lights
	lights->use(boxShader);
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
}