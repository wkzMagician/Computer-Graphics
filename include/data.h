#pragma once

#include "geometry.h"
#include "shader.h"
#include "camera.h"
#include <vector>

#include <algorithm> // ��Ӵ����԰��� std::copy

// ��������
const unsigned int SCR_WIDTH = 1024 * 1.6;
const unsigned int SCR_HEIGHT = 800 * 1.6;

// �����С
float roomWidth = 1.0f;
float roomHeight = 0.6f;
float roomDepth = 1.6f;

// ȫ������
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

// ���������
//Camera camera(glm::vec3(0.0f, 0.05f, 1.0f));
Camera camera(glm::vec3(0.0f, 0.0f, roomDepth / 1.5f));
//Camera camera(glm::vec3(0.0f, 0.1f, 0.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// ��������
glm::vec3 lightPos(0.0f, 0.6f, -0.35f);

// ������ɫ
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 lightAmbient = 0.5f * lightColor;
glm::vec3 lightDiffuse = 0.2f * lightColor;
glm::vec3 lightSpecular = lightColor;

// ��ɫ
glm::vec3 FWallColor(1.0f, 1.0f, 1.0f);
glm::vec3 LWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 RWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 CeilingColor(0.5f, 0.5f, 0.5f);
glm::vec3 FloorColor(0.5f, 0.5f, 0.5f);
glm::vec3 lightCubeColor(1.0f, 1.0f, 1.0f);


// ͳһ�����õ���������Ϣ(ÿһ��ǰ��������Ϊ������꣬������Ϊ������)
   // ------------------------------------------------------------------
float vertices[] = {
    // ����
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
    -roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,

    // ǰ��
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
    -roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,

    // ����
    -roomWidth,  roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,

    // ����
     roomWidth,  roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,

     // ����
     -roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
     -roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
     -roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,

     // ����
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
     -roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f
};

//��ȡ����ƽ�������
// ------------------------------------------------------------------
const unsigned int CeilingVerticesSize = 36;
const unsigned int FloorVerticesSize = 36;
const unsigned int LWallVerticesSize = 36;
const unsigned int RWallVerticesSize = 36;
const unsigned int FWallVerticesSize = 36;
const unsigned int lightCubeVerticesSize = 216;


float CeilingVertices[CeilingVerticesSize];
float FloorVertices[FloorVerticesSize];
float LWallVertices[LWallVerticesSize];
float RWallVertices[RWallVerticesSize];
float FWallVertices[FWallVerticesSize];

/// ///////////////
Geometry roomGeometry(cubePos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry lightCubeGeometry(lightPos, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 0.0f));

void initData() {
    std::copy(vertices + 180, vertices + 216, CeilingVertices);
    std::copy(vertices + 144, vertices + 180, FloorVertices);
    std::copy(vertices + 72, vertices + 108, LWallVertices);
    std::copy(vertices + 108, vertices + 144, RWallVertices);
    std::copy(vertices + 0, vertices + 36, FWallVertices);
}

// constant, linear, and quadratic coefficients
float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;