#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 物体材质
class MyMaterial {
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

	MyMaterial(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shininess = shininess;
	}

	MyMaterial() {
		this->ambient = glm::vec3(0.0f);
		this->diffuse = glm::vec3(0.0f);
		this->specular = glm::vec3(0.0f);
		this->shininess = 0.0f;
	}

	MyMaterial(const MyMaterial& material) {
		this->ambient = material.ambient;
		this->diffuse = material.diffuse;
		this->specular = material.specular;
		this->shininess = material.shininess;
	}
};

// 光源
class Light {
    //vec3 position;
    // 不需要，多光源共用一套设置
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

	Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	Light() {
		this->ambient = glm::vec3(0.0f);
		this->diffuse = glm::vec3(0.0f);
		this->specular = glm::vec3(0.0f);
	}

	Light(const Light& light) {
		this->ambient = light.ambient;
		this->diffuse = light.diffuse;
		this->specular = light.specular;
	}
};
    