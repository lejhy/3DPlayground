#version 330 core

in mat4 Projection;
in vec4 FragPos;

out vec4 color;

void main () {
	float a = Projection[2][2];
	float b = Projection[3][2];

	float far = (b/2) * (1 - ((a-1)/(a+1)));
	float value = FragPos.z / far;

    color = vec4(vec3(value), 1.0f);
}  