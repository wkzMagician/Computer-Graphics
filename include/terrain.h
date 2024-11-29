/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Terrain
{
public:
	Terrain(float radius, float minHeight, float maxHeight, float roughness, float terrainTextureSize);

    ~Terrain();

    void setVerticesArray();

    float* getVerticesArray() {
	    return verticesArray;
    }

    int getVerticesArraySize() {
	    return verticesArraySize;
    }


protected:

	std::vector<glm::vec3> vertices;
    // 法线
	std::vector<glm::vec3> normals;
	// 纹理坐标
	std::vector<glm::vec2> texCoords;

	float* verticesArray;
	int verticesArraySize;

    const int DEPTH = 6;
    const int terrainRotateTimes = 360;
     

	float m_radius = 0.0f; // 圆形半径
    
    float m_minHeight = 0.0f; // 最小高度
    float m_maxHeight = 0.0f; // 最大高度

    float roughness = 0.0f;

    float terrainTextureSize = 0.0f;
    

    // Function to generate a random float value within a specified range
    float RandomFloatRange(float min, float max) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float range = max - min;
        return (random * range) + min;
    }

    void SubdivideTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth);
};

#endif
