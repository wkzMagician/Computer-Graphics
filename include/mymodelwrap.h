#pragma once

#ifndef MYMODELWRAP_H
#define MYMODELWRAP_H

#include "mymodel.h"
#include "abstract_modelwrap.h"

class MyModelWrap : public AbstractModelWrap
{
	MyModel* model;

public:
	MyModelWrap(MyModel* model, Shader* shader, Geometry &geometry)
		: AbstractModelWrap(shader, geometry) {
		this->model = model;
	}

	~MyModelWrap() {
		
	}

	void setColor(glm::vec3 color) {
		((MyModel*)model)->setColor(color);
	}

    // ?是否画边缘
	void draw(Camera camera) {
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        shader->use();

        // view/projection 变换
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        // 设置光照参数
		if (shader->getUniformLocation("lightColor") != -1)
		    shader->setVec3("lightColor", lightColor);
		if (shader->getUniformLocation("lightPos") != -1)
		    shader->setVec3("lightPos", lightPos);
		if (shader->getUniformLocation("viewPos") != -1)
            shader->setVec3("viewPos", camera.Position);

        // 世界坐标变换
		shader->setMat4("model", geometry.getModelMatrix());

		// Shader Color
		shader->setFloat("mixRatio", mixRatio);

		shader->setBool("use2Light", isTreeLit);
		if (isTreeLit) {
			shader->setVec3("pointLight.position", lightPos2);
			shader->setVec3("pointLight.lightColor", lightColor2);
			shader->setFloat("pointLight.constant", constant);
			shader->setFloat("pointLight.linear", linear);
			shader->setFloat("pointLight.quadratic", quadratic);
		}

		// 渲染

		model->draw(shader);
    }

	AbstractModelWrap* clone() {
		return new MyModelWrap(model, shader, geometry);
	}
};

#endif // !MYMODELWRAP_H