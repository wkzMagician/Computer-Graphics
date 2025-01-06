#pragma once

#include "geometry.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "shape.h"
#include "bezier.h"
#include <vector>
#include <array>

#include <algorithm> // ��Ӵ����԰��� std::copy

// ��������
const unsigned int SCR_WIDTH = 1024 * 1.6;
const unsigned int SCR_HEIGHT = 800 * 1.6;

// �����С
float roomWidth = 1.0f;
float roomHeight = 0.6f;
float roomDepth = 1.0f;

// ȫ������
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

// ���������
//Camera camera(glm::vec3(0.0f, 0.05f, 1.0f));
Camera camera(glm::vec3(0.0f, 0.0f, roomDepth));
//Camera camera(glm::vec3(0.0f, 0.1f, 0.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// ��������

// ������ɫ
//glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 lightColor(0.02f, 0.02f, 0.02f);

glm::vec3 lightAmbient = 0.2f * lightColor;
glm::vec3 lightDiffuse = 0.8f * lightColor;
glm::vec3 lightSpecular = 1.0f * lightColor;

//Light light(lightAmbient, lightDiffuse, lightSpecular);

// ���Դ
// �����
float areaLightWidth = 0.2f;
float areaLightHeight = 0.02f;
float areaLightDepth = 0.2f;
// λ��
glm::vec3 areaLightPos(0.0f, roomHeight - areaLightHeight, 0.0f);

// ��������
int sampleNum = 100;
// ����������
std::vector<glm::vec3> samplePoints;

// ��ɫ
glm::vec3 FWallColor(1.0f, 1.0f, 1.0f);
glm::vec3 LWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 RWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 CeilingColor(0.5f, 0.5f, 0.5f);
glm::vec3 FloorColor(0.5f, 0.5f, 0.5f);
glm::vec3 lightCubeColor(1.0f, 1.0f, 1.0f);

// BRDF����
// �����ȣ�Ӱ��߹ⲿ��
float FWallMetallic = 0.2f;
// �ֲڶ�
float FWallRoughness = 0.75f;
// �������ڱ�
float FWallAmbientOcclusion = 0.5f;

Material_BRDF FWallMaterial(FWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF LWallMaterial(LWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF RWallMaterial(RWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF CeilingMaterial(CeilingColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF FloorMaterial(FloorColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);


// ͳһ�����õ���������Ϣ(ÿһ��ǰ��������Ϊ������꣬������Ϊ������)
   // ------------------------------------------------------------------
float vertices[] = {
    // ����
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
    -roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,

    // ǰ��
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
    -roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,

    // ����
    -roomWidth,  roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,

    // ����
     roomWidth,  roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,

     // ����
     -roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
     -roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
     -roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,

     // ����
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
     -roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f
};

float areaLightVertices[] = {
    // ����
    -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
    -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,

    // ǰ��
    -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
    -areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
    -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,

    // ����
    -areaLightWidth,  areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth,  areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth,  areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,

    // ����
     areaLightWidth,  areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,

     // ����
     -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
     -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
     -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,

     // ����
     -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
     -areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
     -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f
};


//��ȡ����ƽ�������
// ------------------------------------------------------------------
const unsigned int CeilingVerticesSize = 36;
const unsigned int FloorVerticesSize = 36;
const unsigned int LWallVerticesSize = 36;
const unsigned int RWallVerticesSize = 36;
const unsigned int FWallVerticesSize = 36;
const unsigned int lightCubeVerticesSize = 216;
const unsigned int areaLightVerticesSize = 216;

float CeilingVertices[CeilingVerticesSize];
float FloorVertices[FloorVerticesSize];
float LWallVertices[LWallVerticesSize];
float RWallVertices[RWallVerticesSize];
float FWallVertices[FWallVerticesSize];

/// ///////////////
Geometry roomGeometry(cubePos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry areaLightGeometry(areaLightPos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry bezierGeometry(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// ����������
// ���� - ��Բ - Բ��
glm::vec2 rectSize(0.2f, 0.2f);
glm::vec2 ellipseRadius(0.2f, 0.1f);
float circleRadius = 0.1f;

Rectangle rectangle(glm::vec3(1.0f, 0.0f, 0.0f), rectSize);
Ellipse ellipse(glm::vec3(1.0f, 0.0f, 0.0f), ellipseRadius);
Circle circle(glm::vec3(1.0f, 0.0f, 0.0f), circleRadius);

// ����������
int shapeSampleNum = 100;
// �����õ��Ķ���
std::vector<glm::vec3> rectSamplePoints;
std::vector<glm::vec3> ellipseSamplePoints;
std::vector<glm::vec3> circleSamplePoints;

// ���ƶ��������
std::vector<glm::vec3> controlPoints = {
	glm::vec3(-roomWidth / 2, roomHeight / 2, -roomDepth / 2),
	glm::vec3(-roomWidth / 4, -roomHeight / 4, -roomDepth / 4),
	glm::vec3(roomWidth / 4, roomHeight / 4, roomDepth / 4),
	glm::vec3(roomWidth / 2, -roomHeight / 2, roomDepth / 2)
};

// ����������
BezierCurve bezierCurve(controlPoints);
int bezierSampleNum = 100;

// ������Ƭ�Ķ���
// ��ά����
class MyVertex {
public:
    glm::vec3 position;
    glm::vec3 normal;

	MyVertex(float x, float y, float z, float nx, float ny, float nz) : position(x, y, z), normal(nx, ny, nz) {}
	MyVertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor) {}
};
using Layer = std::vector<MyVertex>;
std::vector<Layer> bezierVertices;

std::vector<float> bezierVerticesData;

void initData() {
    std::copy(vertices + 180, vertices + 216, CeilingVertices);
    std::copy(vertices + 144, vertices + 180, FloorVertices);
    std::copy(vertices + 72, vertices + 108, LWallVertices);
    std::copy(vertices + 108, vertices + 144, RWallVertices);
    std::copy(vertices + 0, vertices + 36, FWallVertices);

    // ��ʼ��������
	for (int i = 0; i < sampleNum; i++) {
		float ratio = (rand() % 1000) / 500.0f - 1.0f;
		float x = areaLightPos.x + areaLightWidth * ratio;
		ratio = (rand() % 1000) / 500.0f - 1.0f;
		float y = areaLightPos.y + areaLightHeight * ratio;
		ratio = (rand() % 1000) / 500.0f - 1.0f;
		float z = areaLightPos.z + areaLightDepth * ratio;
		samplePoints.push_back(glm::vec3(x, y, z));
	}

	// ��ʼ��������
	rectSamplePoints = rectangle.getSamplePoints(shapeSampleNum);
	ellipseSamplePoints = ellipse.getSamplePoints(shapeSampleNum);
	circleSamplePoints = circle.getSamplePoints(shapeSampleNum);

	// ���㱴���������ϵĵ�
	bezierCurve.calCurvePoints(bezierSampleNum);

	bezierVertices.clear();

    // ��ֵ��������װ������Ƭ
	for (int i = 0; i < bezierSampleNum; i++) {
		bezierVertices.push_back(Layer());

		// ���κ���Բ֮�䣬��Բ��Բ��֮��
        if (i < bezierSampleNum / 2) {
            float ratio = (float)i / (bezierSampleNum / 2);

            for (int j = 0; j < shapeSampleNum; j++) {
                glm::vec3 p1 = rectSamplePoints[j];
                glm::vec3 p2 = ellipseSamplePoints[j];
                glm::vec3 p = p1 + ratio * (p2 - p1);

                glm::vec3 vertexPos = bezierCurve.getPoint(i) + p;

				MyVertex vertex(vertexPos, glm::vec3(0.0f, 0.0f, 0.0f));
                bezierVertices[i].push_back(vertex);
            }
        }
        else {
            float ratio = (float)(i - bezierSampleNum / 2) / (bezierSampleNum / 2);

            for (int j = 0; j < shapeSampleNum; j++) {
                glm::vec3 p1 = ellipseSamplePoints[j];
                glm::vec3 p2 = circleSamplePoints[j];
                glm::vec3 p = p1 + ratio * (p2 - p1);

                glm::vec3 vertexPos = bezierCurve.getPoint(i) + p;

                MyVertex vertex(vertexPos, glm::vec3(0.0f, 0.0f, 0.0f));
                bezierVertices[i].push_back(vertex);
            }
        }
	}

	// ����
	for (int i = 0; i < bezierSampleNum; i++) {
		for (int j = 0; j < shapeSampleNum; j++) {
			MyVertex& vertex1 = bezierVertices[i][j];
			MyVertex& vertex2 = bezierVertices[(i + 1) % bezierSampleNum][j];
			MyVertex& vertex3 = bezierVertices[(i + 1) % bezierSampleNum][(j + 1) % shapeSampleNum];
			MyVertex& vertex4 = bezierVertices[i][(j + 1) % shapeSampleNum];

			// ������1: v1, v2, v3
			// ������2: v1, v3, v4

            // ���㷨����
			glm::vec3 v1(vertex1.position);
			glm::vec3 v2(vertex2.position);
			glm::vec3 v3(vertex3.position);
			glm::vec3 v4(vertex4.position);

			glm::vec3 normal1 = glm::normalize(glm::cross(v2 - v1, v3 - v1));
			glm::vec3 normal2 = glm::normalize(glm::cross(v3 - v1, v4 - v1));

			// vertex�ĺ�����Ԫ�ش洢������
			vertex1.normal += normal1;
			vertex2.normal += normal1;
			vertex3.normal += normal1;

			vertex1.normal += normal2;
			vertex3.normal += normal2;
			vertex4.normal += normal2;
		}
	}

    // ��һ��������
	for (int i = 0; i < bezierSampleNum; i++) {
		for (int j = 0; j < shapeSampleNum; j++) {
			bezierVertices[i][j].normal = glm::normalize(bezierVertices[i][j].normal);
		}
	}

	bezierVerticesData.clear();

	// ���������ݴ洢��������
	for (int i = 0; i < bezierSampleNum; i++) {
		for (int j = 0; j < shapeSampleNum; j++) {
			MyVertex& vertex = bezierVertices[i][j];
			bezierVerticesData.push_back(vertex.position.x);
			bezierVerticesData.push_back(vertex.position.y);
			bezierVerticesData.push_back(vertex.position.z);
			bezierVerticesData.push_back(vertex.normal.x);
			bezierVerticesData.push_back(vertex.normal.y);
			bezierVerticesData.push_back(vertex.normal.z);
		}
	}

	// �ͷ��ڴ�
	bezierVertices.clear();
}

// constant, linear, and quadratic coefficients
float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;