#pragma once

#ifndef MODELWRAP_H
#define MODELWRAP_H

#include "model.h"
#include "data.h"
#include "abstract_modelwrap.h"

class ModelWrap : public AbstractModelWrap {
    Model* model;

public:
    ModelWrap(Model* model, Shader* shader, Geometry& geometry) :
		AbstractModelWrap(shader, geometry) {
		this->model = model;
	}

	~ModelWrap() {
		
	}

	void draw(Camera camera) {
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader->use();

        // view/projection 变换
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

		shader->setVec3("lightAmbient", lightAmbient);
        shader->setVec3("lightDiffuse", lightDiffuse);
        shader->setVec3("lightSpecular", lightSpecular);
		shader->setVec3("lightPos", lightPos);
        shader->setVec3("viewPos", camera.Position);

        // 世界坐标变换
        shader->setMat4("model", geometry.getModelMatrix());

		if (isLight) {
			shader->setBool("isLight", true);
		}
		else {
			shader->setBool("isLight", false);
		}

		// Shader Color
		shader->setFloat("mixRatio", mixRatio);

        // 渲染
        model->Draw(*shader);
    }

	AbstractModelWrap* clone() {
		return new ModelWrap(model, shader, geometry);
	}

};

#endif // !MODELWRAP_H