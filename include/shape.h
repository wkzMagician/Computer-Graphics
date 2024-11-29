#pragma once

#ifndef SHAPE_H
#define SHAPE_H

#include "geometry.h"

enum class ShapeType
{
	POINT,
	// 球
	SPHERE,
	// 立方体
	CUBE,
	// 圆柱
	CYLINDER,
	// 圆锥
	CONE,
	// 平面
	PLANE,
	// 圆形
	CIRCLE,
};

class Shape
{
public:
	ShapeType type;

	Shape() {}
	~Shape() {}

	// clone
	virtual Shape* clone() = 0;

	virtual glm::vec3 getRandomPoint() = 0;
	virtual bool isPointInside(glm::vec3 point) = 0;
	virtual bool isIntersect(Shape* other) = 0;
	virtual void transform(glm::vec3 translate = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) = 0;
};

class Point : public Shape
{
public:
	glm::vec3 position;

	Point(glm::vec3 position)
	{
		this->position = position;
		type = ShapeType::POINT;
	}

	glm::vec3 getRandomPoint() override
	{
		return position;
	}

	virtual Shape* clone() override
	{
		return new Point(position);
	}

	bool isPointInside(glm::vec3 point) override
	{
		return point == position;
	}

	bool isIntersect(Shape* other) override
	{
		//std::cout << "Point isIntersect" << std::endl;
		//std::cout << "position: " << position.x << " " << position.y << " " << position.z << std::endl;
		return other->isPointInside(position);
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
	}
};

class Plane : public Shape
{
public:
	glm::vec3 normal;
	glm::vec3 position;

	Plane(glm::vec3 normal, float d)
	{
		this->normal = normal;
		this->position = normal * d;
		type = ShapeType::PLANE;
	}

	glm::vec3 getRandomPoint() override
	{
		return position;
	}

	virtual Shape* clone() override
	{
		return new Plane(normal, glm::dot(normal, position));
	}

	bool isPointInside(glm::vec3 point) override
	{
		return false;
	}

	bool isIntersect(Shape* other) override
	{
		return false;
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
		Geometry geometry;
		geometry.setModelMatrix(position, glm::vec3(1.0f), rotation);
		this->normal = glm::vec3(geometry.getModelMatrix() * glm::vec4(normal, 0.0f));
	}
};

class Circle : public Plane
{
public:
	float radius;

	Circle(glm::vec3 normal, float d, float radius) : Plane(normal, d)
	{
		this->radius = radius;
		type = ShapeType::CIRCLE;
	}

	glm::vec3 getRandomPoint() override
	{
		// 获得圆面上的随机点
		float r = static_cast<float>(rand()) / RAND_MAX * radius;
		float theta = static_cast<float>(rand()) / RAND_MAX * 2 * 3.1415926f;
		glm::vec3 point = glm::vec3(r * cos(theta), r * sin(theta), 0.0f);
		
		// 旋转到平面
		glm::vec3 axis = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
		float angle = glm::acos(glm::dot(normal, glm::vec3(0.0f, 0.0f, 1.0f)));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, axis);
		point = glm::vec3(rotate * glm::vec4(point, 1.0f));

		return point + position;
	}

	virtual Shape* clone() override
	{
		return new Circle(normal, glm::dot(normal, position), radius);
	}

	bool isPointInside(glm::vec3 point) override
	{
		return false;
	}

	bool isIntersect(Shape* other) override
	{
		return false;
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
		this->radius *= glm::length(rotation);
		// 不处理旋转！！
	}
};

class Cube : public Shape
{
public:
	glm::vec3 position;
	glm::vec3 size;

	Cube(glm::vec3 position, glm::vec3 size)
	{
		this->position = position;
		this->size = size;
		type = ShapeType::CUBE;
	}

	glm::vec3 getRandomPoint() override
	{
		return glm::vec3(
			position.x + static_cast<float>(rand()) / RAND_MAX * size.x,
			position.y + static_cast<float>(rand()) / RAND_MAX * size.y,
			position.z + static_cast<float>(rand()) / RAND_MAX * size.z
		);
	}

	virtual Shape* clone() override
	{
		return new Cube(position, size);
	}

	bool isPointInside(glm::vec3 point) override
	{
		return point.x >= position.x && point.x <= position.x + size.x &&
			point.y >= position.y && point.y <= position.y + size.y &&
			point.z >= position.z && point.z <= position.z + size.z;
	}

	bool isIntersect(Shape* other) override
	{
		if (other->type == ShapeType::POINT)
		{
			return isPointInside(static_cast<Point*>(other)->position);
		}

		return false;
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
		// 不处理旋转！！
	}
};

class Cone : public Shape
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	float radius;
	float height;

	Cone(glm::vec3 position, glm::vec3 normal, float radius, float height)
	{
		this->position = position;
		this->normal = normal;
		this->radius = radius;
		this->height = height;
		type = ShapeType::CONE;
	}

	glm::vec3 getRandomPoint() override
	{
		// 获得圆面上的随机点
		float r = static_cast<float>(rand()) / RAND_MAX * radius;
		float theta = static_cast<float>(rand()) / RAND_MAX * 2 * 3.1415926f;
		glm::vec3 point = glm::vec3(r * cos(theta), r * sin(theta), 0.0f);

		// 旋转到平面
		glm::vec3 axis = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
		float angle = glm::acos(glm::dot(normal, glm::vec3(0.0f, 0.0f, 1.0f)));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, axis);
		point = glm::vec3(rotate * glm::vec4(point, 1.0f));

		// 获得圆锥上的随机点
		float h = static_cast<float>(rand()) / RAND_MAX * height;
		return point + position + glm::vec3(0.0f, 0.0f, h);
	}

	virtual Shape* clone() override
	{
		return new Cone(position, normal, radius, height);
	}

	bool isPointInside(glm::vec3 point) override
	{
		// 计算斜边与高的夹角
		float angle = glm::acos(height / glm::sqrt(height * height + radius * radius));
		// 计算点与顶点的连线与高的夹角
		glm::vec3 v = point - position - normal * height;
		float angle2_cos = glm::abs(glm::dot(v, -normal) / glm::length(v));
		float angle2 = glm::acos(angle2_cos);

		if (angle2 <= angle && angle2_cos * glm::length(v) <= height){
			return true;
		}

		return false;
	}

	bool isIntersect(Shape* other) override
	{
		if (other->type == ShapeType::POINT)
		{
			return isPointInside(static_cast<Point*>(other)->position);
		}

		return false;
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
		// 不处理旋转！！
	}
};

class Cylinder : public Shape
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	float radius;
	float height;

	Cylinder(glm::vec3 position, glm::vec3 normal, float radius, float height)
	{
		this->position = position;
		this->normal = normal;
		this->radius = radius;
		this->height = height;
		type = ShapeType::CYLINDER;
	}

	glm::vec3 getRandomPoint() override
	{
		// 获得圆面上的随机点
		float r = static_cast<float>(rand()) / RAND_MAX * radius;
		float theta = static_cast<float>(rand()) / RAND_MAX * 2 * 3.1415926f;
		glm::vec3 point = glm::vec3(r * cos(theta), r * sin(theta), 0.0f);

		// 旋转到平面
		glm::vec3 axis = glm::cross(normal, glm::vec3(0.0f, 0.0f, 1.0f));
		float angle = glm::acos(glm::dot(normal, glm::vec3(0.0f, 0.0f, 1.0f)));
		glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, axis);
		point = glm::vec3(rotate * glm::vec4(point, 1.0f));

		// 获得圆柱上的随机点
		float h = static_cast<float>(rand()) / RAND_MAX * height;
		return point + position + glm::vec3(0.0f, 0.0f, h);
	}

	virtual Shape* clone() override
	{
		return new Cylinder(position, normal, radius, height);
	}

	bool isPointInside(glm::vec3 point) override
	{
		glm::vec3 v = point - position;
		float v_len = glm::length(v);
		float h = glm::dot(v, normal);
		float r = glm::sqrt(v_len * v_len - h * h);

		if (h >= 0 && h <= height && r <= radius) {
			return true;
		}

		return false;
	}

	bool isIntersect(Shape* other) override
	{
		if (other->type == ShapeType::POINT)
		{
			return isPointInside(static_cast<Point*>(other)->position);
		}

		return false;
	}

	void transform(glm::vec3 position = glm::vec3(0.0f), glm::vec3 rotation = glm::vec3(0.0f)) override
	{
		this->position = position;
		// 不处理旋转！！
	}

};


#endif // !SHAPE_H