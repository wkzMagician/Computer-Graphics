#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

const float PI = 3.1415926535f;

class Shape {
public:
	glm::vec3 normal;

	Shape(glm::vec3 normal = glm::vec3(0.0f)) {
		this->normal = normal;
	}

	Shape(const Shape& shape) {
		this->normal = shape.normal;
	}

	Shape& operator=(const Shape& shape) {
		this->normal = shape.normal;
		return *this;
	}

	// 统一从 -45度采样到 315度

	int numSamples = 100;

	std::vector<glm::vec3> samples;

	// 采样
	virtual std::vector<glm::vec3> getSamplePoints(int numSamples = 100) {
		this->numSamples = numSamples;
		samples.clear();
		calRawSamplePoints();
		transformSamplePoints();
		return samples;
	}

	// 获得未处理的采样点
	virtual void calRawSamplePoints() = 0;
	
	void transformSamplePoints() {
		// 旋转变换, 将z轴旋转到normal
		glm::vec3 axis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), normal);
		float angle = glm::acos(glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), normal));
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);

		for (int i = 0; i < samples.size(); i++) {
			samples[i] = glm::vec3(rotation * glm::vec4(samples[i], 1.0f));
		}
	}
};

class Rectangle : public Shape {
public:
	// 长宽
	glm::vec2 size;

	Rectangle(glm::vec3 normal = glm::vec3(0.0f), glm::vec2 size = glm::vec2(1.0f)) : Shape(normal) {
		this->size = size;
	}

	Rectangle(const Rectangle& rectangle) : Shape(rectangle) {
		this->size = rectangle.size;
	}

	Rectangle& operator=(const Rectangle& rectangle) {
		Shape::operator=(rectangle);
		this->size = rectangle.size;
		return *this;
	}

	virtual void calRawSamplePoints() {
		int numSamplesPerSide = numSamples / 4;

		float stepX = size.x / numSamplesPerSide;
		float stepY = size.y / numSamplesPerSide;

		// 右边
		for (int i = 0; i < numSamplesPerSide; i++) {
			samples.push_back(glm::vec3(size.x / 2, -size.y / 2 + i * stepY, 0.0f));
		}
		// 上边
		for (int i = 0; i < numSamplesPerSide; i++) {
			samples.push_back(glm::vec3(size.x / 2 - i * stepX, size.y / 2, 0.0f));
		}
		// 左边
		for (int i = 0; i < numSamplesPerSide; i++) {
			samples.push_back(glm::vec3(-size.x / 2, size.y / 2 - i * stepY, 0.0f));
		}
		// 下边
		for (int i = 0; i < numSamplesPerSide; i++) {
			samples.push_back(glm::vec3(-size.x / 2 + i * stepX, -size.y / 2, 0.0f));
		}
	}
};

// 椭圆
class Ellipse : public Shape {
public:
	// 长短轴
	glm::vec2 radius;

	Ellipse(glm::vec3 normal = glm::vec3(0.0f), glm::vec2 radius = glm::vec2(1.0f)) : Shape(normal) {
		this->radius = radius;
	}

	Ellipse(const Ellipse& ellipse) : Shape(ellipse) {
		this->radius = ellipse.radius;
	}

	Ellipse& operator=(const Ellipse& ellipse) {
		Shape::operator=(ellipse);
		this->radius = ellipse.radius;
		return *this;
	}

	virtual void calRawSamplePoints() {
		float deltaAngle = 2 * PI / numSamples;
		for (float angle = -PI / 4; angle < 7 * PI / 4; angle += deltaAngle) {
			samples.push_back(glm::vec3(radius.x * glm::cos(angle), radius.y * glm::sin(angle), 0.0f));
		}
	}
};

// 圆
class Circle : public Shape {
public:
	// 半径
	float radius;

	Circle(glm::vec3 normal = glm::vec3(0.0f), float radius = 1.0f) : Shape(normal) {
		this->radius = radius;
	}

	Circle(const Circle& circle) : Shape(circle) {
		this->radius = circle.radius;
	}

	Circle& operator=(const Circle& circle) {
		Shape::operator=(circle);
		this->radius = circle.radius;
		return *this;
	}

	virtual void calRawSamplePoints() {
		float deltaAngle = 2 * PI / numSamples;
		for (float angle = -PI / 4; angle < 7 * PI / 4; angle += deltaAngle) {
			samples.push_back(glm::vec3(radius * glm::cos(angle), radius * glm::sin(angle), 0.0f));
		}

	}
};
	