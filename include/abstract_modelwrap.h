#pragma once

#ifndef ABSTRACT_MODELWRAP_H
#define ABSTRACT_MODELWRAP_H

#include "geometry.h"
#include "shader.h"

class AbstractModelWrap {
protected:
    Shader* shader;
    Geometry geometry;
	Geometry initGeometry;

	bool isLight = false;
	float mixRatio = 0.0f;

public:
	AbstractModelWrap( Shader* shader, Geometry& geometry) {
		this->shader = shader;
		this->geometry = geometry;
		this->initGeometry = geometry;
		this->isLight = false;
		this->mixRatio = 0.0f;
	}

	AbstractModelWrap() {

	}

	AbstractModelWrap(AbstractModelWrap& modelWrap) {
		this->shader = modelWrap.shader;
		this->geometry = modelWrap.geometry;
		this->initGeometry = modelWrap.initGeometry;
		this->isLight = modelWrap.isLight;
		this->mixRatio = modelWrap.mixRatio;
	}

	~AbstractModelWrap() {

	}

	// 设置新的模型Geometry
	void setGeometry(Geometry& geometry) {
		this->geometry = geometry;
	}

	// 旋转
	void rotate(float angle, glm::vec3 axis) {
		geometry.setModelMatrix(geometry.position, geometry.scale, geometry.rotation + angle * axis);
	}

	void scale(float scale) {
		geometry.setModelMatrix(geometry.position, initGeometry.scale * scale, geometry.rotation);
	}

	void setPosition(glm::vec3 position) {
		geometry.setModelMatrix(position, geometry.scale, geometry.rotation);
	}

	void setScale(glm::vec3 scale) {
		geometry.setModelMatrix(geometry.position, scale, geometry.rotation);
	}

	void setMixRatio(float mixRatio) {
		this->mixRatio = mixRatio;
	}

	glm::vec3 getPosition() {
		return geometry.position;
	}

	glm::vec3 getScale() {
		return geometry.scale;
	}

	glm::vec3 getRotation() {
		return geometry.rotation;
	}

	virtual void draw(Camera camera) = 0;

	virtual AbstractModelWrap* clone() = 0;

	void setLight(bool isLight) {
		this->isLight = isLight;
	}
};

#endif // !ABSTRACT_MODELWRAP_H