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

#include "terrain.h"

Terrain::Terrain(float radius, float minHeight, float maxHeight, float roughness, float terrainTextureSize)
	: m_radius(radius), m_minHeight(minHeight), m_maxHeight(maxHeight), roughness(roughness), terrainTextureSize(terrainTextureSize)
{
	//glm::vec3 mid(0.0f, RandomFloatRange(m_minHeight, m_maxHeight), 0.0f);
	glm::vec3 mid(0.0f, 0.0f, 0.0f);
	//glm::vec3 mid(0.0f, (m_minHeight + m_maxHeight) / 2, 0.0f);

    float angle = 360.0f / terrainRotateTimes;

	glm::vec3 v0(m_radius, 0.0f, 0.0f);
	glm::vec3 v1(m_radius, 0.0f, 0.0f);

    for (int i = 0; i < terrainRotateTimes; i++)
    {
        // 旋转，计算两个三角形顶点
		v0 = glm::vec3(mid.x + cos(angle * i) * m_radius, 0.0f, mid.z + sin(angle * i) * m_radius);
		v1 = glm::vec3(mid.x + cos(angle * (i + 1)) * m_radius, 0.0f, mid.z + sin(angle * (i + 1)) * m_radius);

        SubdivideTriangle(mid, v0, v1, 0);
    }
}

Terrain::~Terrain()
{
	
}

void Terrain::SubdivideTriangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, int depth)
{
    if (depth >= DEPTH) {
		vertices.push_back(v0);
		vertices.push_back(v1);
		vertices.push_back(v2);
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
		normals.push_back(normal);
		normals.push_back(normal);
		normals.push_back(normal);
		// 纹理
		texCoords.push_back(glm::vec2(v0.x / terrainTextureSize, v0.z / terrainTextureSize));
		texCoords.push_back(glm::vec2(v1.x / terrainTextureSize, v1.z / terrainTextureSize));
		texCoords.push_back(glm::vec2(v2.x / terrainTextureSize, v2.z / terrainTextureSize));

		return;
    }

	float height = pow(2, -roughness * depth) * (m_maxHeight - m_minHeight) * 0.6f;

	glm::vec3 mid0((v0.x + v1.x) / 2, (v0.y + v1.y) / 2, (v0.z + v1.z) / 2);
	glm::vec3 mid1((v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2);
	glm::vec3 mid2((v2.x + v0.x) / 2, (v2.y + v0.y) / 2, (v2.z + v0.z) / 2);

    float offset = RandomFloatRange(-height, height);
	mid0.y = glm::clamp(mid0.y + offset, m_minHeight, m_maxHeight);
    offset = RandomFloatRange(-height, height);
	mid1.y = glm::clamp(mid1.y + offset, m_minHeight, m_maxHeight);
    offset = RandomFloatRange(-height, height);
	mid2.y = glm::clamp(mid2.y + offset, m_minHeight, m_maxHeight);

    SubdivideTriangle(v0, mid0, mid2, depth + 1);
    SubdivideTriangle(mid0, v1, mid1, depth + 1);
    SubdivideTriangle(mid2, mid1, v2, depth + 1);
    SubdivideTriangle(mid1, mid0, mid2, depth + 1);
}

void Terrain::setVerticesArray()
{
    verticesArraySize = vertices.size() * 8;
	verticesArray = new float[verticesArraySize];

	for (int i = 0; i < vertices.size(); i++)
	{
		verticesArray[i * 8] = vertices[i].x;
		verticesArray[i * 8 + 1] = vertices[i].y;
		verticesArray[i * 8 + 2] = vertices[i].z;
		verticesArray[i * 8 + 3] = normals[i].x;
		verticesArray[i * 8 + 4] = normals[i].y;
		verticesArray[i * 8 + 5] = normals[i].z;
		verticesArray[i * 8 + 6] = texCoords[i].x;
		verticesArray[i * 8 + 7] = texCoords[i].y;
	}
}
