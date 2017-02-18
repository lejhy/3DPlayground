#version 330 core

in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main () {
	color = texture(texture0, textureCoordinates) * vec4(vertexColor, 1.0f);
	color = mix(color, texture(texture1, textureCoordinates), texture(texture1, textureCoordinates).a * 0.5);
}