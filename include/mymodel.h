#pragma once

#ifndef MYMODEL_H
#define MYMODEL_H

#include "mymesh.h"

class MyModel 
{
	std::vector<MyMesh*> meshes;
	
public:
	MyModel(std::vector<MyMesh*> meshes) {
		this->meshes = meshes;
	}

	~MyModel() {
		/*for (int i = 0; i < meshes.size(); i++) {
			delete meshes[i];
		}*/
	}

	void draw(Shader* shader) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i]->draw(shader);
		}
	}

	void setColor(glm::vec3 color) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i]->setColor(color);
		}
	}
};

#endif // !MYMODEL_H