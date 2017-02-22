#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec2 texture;

out vec3 fragmentPosition;
out vec3 normalVector;
out vec3 vertexColor;
out vec2 textureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	fragmentPosition = vec3(model * vec4(position, 1.0f));
	normalVector = mat3(transpose(inverse(model))) * normal;
	vertexColor = color;
	textureCoordinates = texture;
}