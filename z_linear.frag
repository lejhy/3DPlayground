#version 330 core

in mat4 Projection;

out vec4 color;

void main () {
	float a = Projection[2][2];
	float b = Projection[3][2];

	float far = (b/2) * (1 - ((a-1)/(a+1)));
	float near = (b/2) * (((a+1)/(a-1)) - 1);
	float z = gl_FragCoord.z;

	float value = (1 + far / (z*(far-near) - far)) * near / (near - far);

    color = vec4(vec3(value), 1.0f);
}  