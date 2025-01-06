#pragma once

#ifndef MYMESH_H
#define MYMESH_H

#include "model.h"
#include "data.h"

class MyMesh 
{
	// 顶点数组
	float* vertices;
	// VAO
	unsigned int VAO;
	// VBO
	unsigned int VBO;

    // 顶点数组的大小
	unsigned int verticesSize;

    // don't use EBOs

    // color
	glm::vec3 color;

	// 材质
	MyMaterial material;

	Material_BRDF materialBRDF;

	// Texture Path
	vector<string> texturePaths;

	bool useTexture = false;

	const float LINE_WIDTH = 7.0f;
	const glm::vec3 lineColor = glm::vec3(0.0f, 0.0f, 0.0f);

	bool drawEdge = false;

public:
	void setDrawEdge(bool drawEdge) {
		this->drawEdge = drawEdge;
	}

	// Texture
	vector<Texture> textures;

	unsigned int vertexAttrSize = 6;


	MyMesh(float* vertices, unsigned int N, glm::vec3 color) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->color = color;

		setupMesh();
    }

	MyMesh(float* vertices, unsigned int N, MyMaterial material) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->material = material;

		setupMesh();
	}

	MyMesh(float* vertices, unsigned int N, Material_BRDF material) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->materialBRDF = material;

		setupMesh();
	}

	MyMesh(std::vector<float> vertices, Material_BRDF material) {
		verticesSize = vertices.size();
		this->vertices = new float[verticesSize];
		memcpy(this->vertices, vertices.data(), verticesSize * sizeof(float));
		this->materialBRDF = material;

		setupMesh();
	}

	MyMesh(float* vertices, unsigned int N, glm::vec3 color, vector<string> texturePaths, bool useTexture) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->color = color;
		this->texturePaths = texturePaths;
		this->useTexture = useTexture;

		vertexAttrSize = 8;

		setupMesh();
	}

	~MyMesh() {
		// 释放VAO和VBO
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		// 释放顶点数组
		delete[] vertices;

		// 释放纹理
		for (unsigned int i = 0; i < textures.size(); i++) {
			glDeleteTextures(1, &textures[i].id);
		}
	}

	bool loadTexture(const char* path, string typeName) {
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);

		if (data) {
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);

			// 设置纹理环绕方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// 设置纹理过滤方式
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (nrChannels == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			else if (nrChannels == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else {
				cout << "Texture format not supported" << endl;
				return false;
			}

			glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);

			Texture textureObj;
			textureObj.id = texture;
			textureObj.type = typeName;
			textureObj.path = path;

			textures.push_back(textureObj);

			return true;
		}
		else {
			cout << "Failed to load texture" << endl;
			stbi_image_free(data);
			return false;
		}
	}
    
    void setupMesh() {
        // 生成顶点数组对象和顶点缓冲对象
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // 绑定VBO并传输顶点数据
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesSize, vertices, GL_STATIC_DRAW);

        // 绑定VAO
        glBindVertexArray(VAO);

		if (!useTexture) {
			// 设置顶点属性指针
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
			glEnableVertexAttribArray(0); // 启动顶点属性

			// 设置法向量属性指针
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1); // 启动法向量属性

			return;
		}
        
		// 设置顶点属性指针
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0); // 启动顶点属性

		// 设置法向量属性指针
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // 启动法向量属性


		// 设置纹理属性指针
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2); // 启动纹理属性

		// 加载纹理
		for (unsigned int i = 0; i < texturePaths.size(); i++) {
			if (!loadTexture(texturePaths[i].c_str(), "gTextureHeight")) {
				cout << "Failed to load texture" << endl;
			}
		}

    }

	void draw(Shader* shader) {
		if (drawEdge) {
			shader->setVec3("objectColor", lineColor);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(LINE_WIDTH);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, verticesSize / vertexAttrSize);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		shader->setVec3("objectColor", color);

		// ！！！设置材质
		/*shader->setVec3("material.ambient", material.ambient);
		shader->setVec3("material.diffuse", material.diffuse);
		shader->setVec3("material.specular", material.specular);
		shader->setFloat("material.shininess", material.shininess);*/

		// 设置 BRDF 材质
		shader->setVec3("material.albedo", materialBRDF.albedo);
		shader->setFloat("material.ao", materialBRDF.ao);
		shader->setFloat("material.metallic", materialBRDF.metallic);
		shader->setFloat("material.roughness", materialBRDF.roughness);

		/*if (useTexture) {
			shader->setBool("useTex", true);
		}
		else {
			shader->setBool("useTex", false);
		}*/

		/*shader->setVec4("Ka", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		shader->setVec4("Kd", glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
		shader->setVec4("Ks", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		shader->setFloat("Ns", 32.0f);*/

		// 渲染
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, verticesSize / vertexAttrSize);

		// 解绑VAO
		glBindVertexArray(0);

		// 解绑纹理
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void setColor(glm::vec3 color) {
		this->color = color;
	}
};

#endif // !MYMESH_H