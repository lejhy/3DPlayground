#pragma once

// Std
#include <vector>
#include <string>
using namespace std;
// Glm
#include <glm/glm.hpp>
using namespace glm;
// GLEW
#include <GL\glew.h>
// Includes
#include "Shader.h"

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

class Lights {
	public:
		GLuint nrOfDirLights;
		vector <DirLight> dirLights;
		GLuint nrOfPointLights;
		vector <PointLight> pointLights;
		GLuint nrOfSpotLights;
		vector <SpotLight> spotLights;

		Lights() {
			this->nrOfDirLights = 0;
			this->nrOfPointLights = 0;
			this->nrOfSpotLights = 0;
		}

		GLuint addLight(DirLight light) {
			dirLights.push_back(light);
			return nrOfDirLights++;
		}

		GLuint addLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular) {
			DirLight light;
			light.direction = direction;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			dirLights.push_back(light);
			return nrOfDirLights++;
		}

		GLuint addLight(PointLight light) {
			pointLights.push_back(light);
			return nrOfDirLights + nrOfPointLights++;
		}

		GLuint addLight(vec3 position, float constant, float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular) {
			PointLight light;
			light.position = position;
			light.constant = constant;
			light.linear = linear;
			light.quadratic = quadratic;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			pointLights.push_back(light);
			return nrOfDirLights + nrOfPointLights++;
		}

		GLuint addLight(SpotLight light) {
			spotLights.push_back(light);
			return nrOfDirLights + nrOfPointLights + nrOfSpotLights++;
		}

		GLuint addLight(vec3 position, vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, vec3 ambient, vec3 diffuse, vec3 specular) {
			SpotLight light;
			light.position = position;
			light.direction = direction;
			light.cutOff = cutOff;
			light.outerCutOff = outerCutOff;
			light.constant = constant;
			light.linear = linear;
			light.quadratic = quadratic;
			light.ambient = ambient;
			light.diffuse = diffuse;
			light.specular = specular;
			spotLights.push_back(light);
			return nrOfDirLights + nrOfPointLights + nrOfSpotLights++;
		}

		void setPos(GLuint light, vec3 position) {
			if (light < nrOfDirLights) {
				// do nothing
			} else if (light < nrOfDirLights + nrOfPointLights) {
				pointLights[light - nrOfDirLights].position = position;
			} else if (light < nrOfDirLights + nrOfPointLights + nrOfSpotLights) {
				spotLights[light - nrOfDirLights - nrOfPointLights].position = position;
			} else {
				// index out of bounds
			}
		}

		void setDir(GLuint light, vec3 direction) {
			if (light < nrOfDirLights) {
				dirLights[light].direction = direction;
			}
			else if (light < nrOfDirLights + nrOfPointLights) {
				// do nothing
			}
			else if (light < nrOfDirLights + nrOfPointLights + nrOfSpotLights) {
				spotLights[light - nrOfDirLights - nrOfPointLights].direction = direction;
			}
			else {
				// index out of bounds
			}
		}

		void use(Shader shader) {
			for (int i = 0; i < nrOfDirLights; i++) {
				string light = "dirLights[" + to_string(i) + "]";
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".direction").c_str()), dirLights[i].direction.x, dirLights[i].direction.y, dirLights[i].direction.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".ambient").c_str()), dirLights[i].ambient.x, dirLights[i].ambient.y, dirLights[i].ambient.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".diffuse").c_str()), dirLights[i].diffuse.x, dirLights[i].diffuse.y, dirLights[i].diffuse.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".specular").c_str()), dirLights[i].specular.x, dirLights[i].specular.y, dirLights[i].specular.z);
			}

			for (int i = 0; i < nrOfPointLights; i++) {
				string light = "pointLights[" + to_string(i) + "]";
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".position").c_str()), pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".constant").c_str()), pointLights[i].constant);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".linear").c_str()), pointLights[i].linear);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".quadratic").c_str()), pointLights[i].quadratic);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".ambient").c_str()), pointLights[i].ambient.x, pointLights[i].ambient.y, pointLights[i].ambient.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".diffuse").c_str()), pointLights[i].diffuse.x, pointLights[i].diffuse.y, pointLights[i].diffuse.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".specular").c_str()), pointLights[i].specular.x, pointLights[i].specular.y, pointLights[i].specular.z);
			}

			for (int i = 0; i < nrOfSpotLights; i++) {
				string light = "spotLights[" + to_string(i) + "]";
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".position").c_str()), spotLights[i].position.x, spotLights[i].position.y, spotLights[i].position.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".direction").c_str()), spotLights[i].direction.x, spotLights[i].direction.y, spotLights[i].direction.z);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".cutOff").c_str()), spotLights[i].cutOff);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".outerCutOff").c_str()), spotLights[i].outerCutOff);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".constant").c_str()), spotLights[i].constant);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".linear").c_str()), spotLights[i].linear);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".quadratic").c_str()), spotLights[i].quadratic);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".ambient").c_str()), spotLights[i].ambient.x, spotLights[i].ambient.y, spotLights[i].ambient.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".diffuse").c_str()), spotLights[i].diffuse.x, spotLights[i].diffuse.y, spotLights[i].diffuse.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".specular").c_str()), spotLights[i].specular.x, spotLights[i].specular.y, spotLights[i].specular.z);
			}
		}

	private:
		
};