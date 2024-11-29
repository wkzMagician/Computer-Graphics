#pragma once

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 描述平移, 缩放，旋转的类

class Geometry
{
public:
	// 位置
	glm::vec3 position;
	// 缩放
	glm::vec3 scale;
	// 旋转
	glm::vec3 rotation;

	// model
	glm::mat4 model;

	// 构造函数
	Geometry(glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1.0f), glm::vec3 rotation = glm::vec3(0.0f)){
		setModelMatrix(position, scale, rotation);
	}

	void setModelMatrix(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
	{
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;

		model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		this->model = model;
	}

	glm::mat4 getModelMatrix() const
	{
		return model;
	}
};

#endif // !GEOMETRY_H