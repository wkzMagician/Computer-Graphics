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
    
// 用于 Cook-Torrance BRDF 的参数
class Material_BRDF {
public:
	glm::vec3 albedo; //基础颜色
	float metallic; //金属度
	float roughness; //粗糙度
	float ao; //环境光遮蔽

	Material_BRDF(glm::vec3 albedo, float metallic, float roughness, float ao) {
		this->albedo = albedo;
		this->metallic = metallic;
		this->roughness = roughness;
		this->ao = ao;
	}

	Material_BRDF() {
		this->albedo = glm::vec3(0.0f);
		this->metallic = 0.0f;
		this->roughness = 0.0f;
		this->ao = 0.0f;
	}

	Material_BRDF(const Material_BRDF& material) {
		this->albedo = material.albedo;
		this->metallic = material.metallic;
		this->roughness = material.roughness;
		this->ao = material.ao;
	}
};