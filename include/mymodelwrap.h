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

	MyModelWrap(MyModel* model, Shader* shader) {
		Geometry g;
		this->geometry = g;
		this->shader = shader;
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

        // 变换
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
		shader->setMat4("model", geometry.getModelMatrix());

        // 设置光照参数
		shader->setVec3("lightColor", lightColor);
		// 更复杂的光照设置
		/*shader->setVec3("light.ambient", light.ambient);
		shader->setVec3("light.diffuse", light.diffuse);
		shader->setVec3("light.specular", light.specular);*/

        shader->setVec3("viewPos", camera.Position);

		// 渲染
		model->draw(shader);
    }

	AbstractModelWrap* clone() {
		return new MyModelWrap(model, shader, geometry);
	}
};

#endif // !MYMODELWRAP_H