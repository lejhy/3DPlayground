#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture;

out vec3 vertexColor;
out vec2 textureCoordinates;

uniform mat4 transform;

void main () {
	gl_Position = transform * vec4(position, 1.0f);
	vertexColor = color;
	textureCoordinates = texture;
}