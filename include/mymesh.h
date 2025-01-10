#pragma once

#ifndef MYMESH_H
#define MYMESH_H

#include "model.h"
#include "data.h"

class MyMesh 
{
public:
	// 材质类型
	// 比如说，CUBE_MAP, 2D_TEXTURE
	enum TextureType {
		CUBE_MAP,
		TEXTURE_2D
	};

private:
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

	TextureType textureType;
	// textureID
	unsigned int textureID;
	
	const float LINE_WIDTH = 7.0f;
	const glm::vec3 lineColor = glm::vec3(0.0f, 0.0f, 0.0f);

	bool drawEdge = false;
	bool drawLine = false;
	bool drawLineLoop = false;

public:
	void setDrawEdge(bool drawEdge) {
		this->drawEdge = drawEdge;
	}

	// Texture
	vector<Texture> textures;

	unsigned int vertexAttrSize = 6;


	MyMesh(float* vertices, unsigned int N, glm::vec3 color, int vertexAttrSize = 6) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->color = color;

		setupMesh();
    }

	MyMesh(float* vertices, unsigned int N, MyMaterial material, int vertexAttrSize = 6) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->material = material;

		setupMesh();
	}

	MyMesh(float* vertices, unsigned int N, Material_BRDF material, int vertexAttrSize = 6) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->materialBRDF = material;

		setupMesh();
	}

	MyMesh(std::vector<float> vertices, Material_BRDF material, int vertexAttrSize = 6) {
		verticesSize = vertices.size();
		this->vertices = new float[verticesSize];
		memcpy(this->vertices, vertices.data(), verticesSize * sizeof(float));
		this->materialBRDF = material;

		setupMesh();
	}

	MyMesh(float* vertices, unsigned int N, vector<string> texturePaths, TextureType textureType, int vertexAttrSize = 3) {
		verticesSize = N;
		this->vertices = new float[N];
		memcpy(this->vertices, vertices, N * sizeof(float));
		this->color = color;
		this->texturePaths = texturePaths;
		this->useTexture = true;
		this->textureType = textureType;

		this->vertexAttrSize = vertexAttrSize;

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

	bool loadCubemap() {

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		int width, height, nrChannels;
		for (unsigned int i = 0; i < texturePaths.size(); i++)
		{
			unsigned char* data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << texturePaths[i] << std::endl;
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return true;
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

		if (textureType == CUBE_MAP) {
			// 设置顶点属性指针
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, this->vertexAttrSize * sizeof(float), (void*)(0 * sizeof(float)));
			glEnableVertexAttribArray(0); // 启动顶点属性

			loadCubemap();
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

		// 设置材质
		/*shader->setVec3("material.ambient", material.ambient);
		shader->setVec3("material.diffuse", material.diffuse);
		shader->setVec3("material.specular", material.specular);
		shader->setFloat("material.shininess", material.shininess);*/

		// 设置 BRDF 材质
		shader->setVec3("material.albedo", materialBRDF.albedo);
		shader->setFloat("material.ao", materialBRDF.ao);
		shader->setFloat("material.metallic", materialBRDF.metallic);
		shader->setFloat("material.roughness", materialBRDF.roughness);

		if (useTexture) {
			if (textureType == CUBE_MAP) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
			}
			else {
				for (unsigned int i = 0; i < textures.size(); i++) {
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, textures[i].id);
				}
			}
		}

		// 渲染
		glBindVertexArray(VAO);

		if (drawLine) {
			if (drawLineLoop) glDrawArrays(GL_LINE_LOOP, 0, verticesSize / vertexAttrSize);
			else glDrawArrays(GL_LINE_STRIP, 0, verticesSize / vertexAttrSize);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, verticesSize / vertexAttrSize);
		}

		// 解绑VAO
		glBindVertexArray(0);

		// 解绑纹理
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void setColor(glm::vec3 color) {
		this->color = color;
	}

	void setDrawLine(bool drawLine, bool drawLineLoop) {
		this->drawLine = drawLine;
		this->drawLineLoop = drawLineLoop;
	}

	void setMaterial(MyMaterial material) {
		this->material = material;
	}

	void setMaterial(Material_BRDF material) {
		this->materialBRDF = material;
	}

	void setMaterial(glm::vec3 color, float metallic, float roughness, float ambientOcclusion) {
		this->materialBRDF.albedo = color;
		this->materialBRDF.metallic = metallic;
		this->materialBRDF.roughness = roughness;
		this->materialBRDF.ao = ambientOcclusion;
	}
};

#endif // !MYMESH_H