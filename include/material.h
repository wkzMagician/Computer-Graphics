#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// �������
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

// ��Դ
class Light {
    //vec3 position;
    // ����Ҫ�����Դ����һ������
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
    