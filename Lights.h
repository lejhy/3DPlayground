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
		GLint nrOfDirLights;
		vector <DirLight> dirLights;
		GLint nrOfPointLights;
		vector <PointLight> pointLights;
		GLint nrOfSpotLights;
		vector <SpotLight> spotLights;

		Lights() {
			this->nrOfDirLights = 0;
			this->nrOfPointLights = 0;
			this->nrOfSpotLights = 0;
		}

		int addLight(DirLight light) {
			dirLights.push_back(light);
			nrOfDirLights++;
		}

		int addLight(PointLight light) {
			pointLights.push_back(light);
			nrOfPointLights++;
		}

		int addLight(SpotLight light) {
			spotLights.push_back(light);
			nrOfSpotLights++;
		}

		int use(Shader shader) {
			for (int i = 0; i < nrOfDirLights; i++) {
				string light = "dirLights" + to_string(i);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".direction").c_str()), dirLights[i].direction.x, dirLights[i].direction.y, dirLights[i].direction.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".ambient").c_str()), dirLights[i].ambient.x, dirLights[i].ambient.y, dirLights[i].ambient.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".diffuse").c_str()), dirLights[i].diffuse.x, dirLights[i].diffuse.y, dirLights[i].diffuse.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".specular").c_str()), dirLights[i].specular.x, dirLights[i].specular.y, dirLights[i].specular.z);
			}

			for (int i = 0; i < nrOfPointLights; i++) {
				string light = "pointLights" + to_string(i);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".position").c_str()), pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".constant").c_str()), pointLights[i].constant);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".linear").c_str()), pointLights[i].linear);
				glUniform1f(glGetUniformLocation(shader.programID, (light + ".quadratic").c_str()), pointLights[i].quadratic);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".ambient").c_str()), pointLights[i].ambient.x, pointLights[i].ambient.y, pointLights[i].ambient.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".diffuse").c_str()), pointLights[i].diffuse.x, pointLights[i].diffuse.y, pointLights[i].diffuse.z);
				glUniform3f(glGetUniformLocation(shader.programID, (light + ".specular").c_str()), pointLights[i].specular.x, pointLights[i].specular.y, pointLights[i].specular.z);
			}

			for (int i = 0; i < nrOfSpotLights; i++) {
				string light = "spotLights" + to_string(i);
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