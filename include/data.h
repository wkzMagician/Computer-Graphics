#pragma once

#include "geometry.h"
#include "shader.h"
#include "camera.h"
#include "shape.h"
#include <vector>

#include <algorithm> // 添加此行以包含 std::copy

bool emitSnow = false;
bool isTreeLit = false;

// 窗口设置
const unsigned int SCR_WIDTH = 1024 * 1.6;
const unsigned int SCR_HEIGHT = 800 * 1.6;

// 房间大小
float roomWidth = 1.0f;
float roomHeight = 0.6f;
float roomDepth = 1.6f;

// 黑板大小设置
float blackboardWidth = 0.60f;
float blackboardHeight = 0.36f;
// 黑板边框大小
float blackboardBorderSize = 0.03f;

// 是否显示风车
bool showWindmill = false;
// 风车边的长度
float windmillBladeLength = 0.12f;
// 风车颜色
glm::vec3 windmillColor = glm::vec3(1.0f, 1.0f, 1.0f); // 初始颜色为白色
// 风车是否旋转
bool windmillRotate = false;
// 风车每秒转动的角度,用弧度表示
float windmillRotateSpeed = 90.0f;
// 风车已经转过的角度
float windmillRotatedAngle = 0.0f;


// 全局坐标
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

// 摄像机设置
//Camera camera(glm::vec3(0.0f, 0.05f, 1.0f));
Camera camera(glm::vec3(0.0f, 0.0f, roomDepth / 1.5f));
//Camera camera(glm::vec3(0.0f, 0.1f, 0.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;


// 房间中各个物体的位置

glm::vec3 floorMidPos = cubePos + glm::vec3(0.0f, -roomHeight, 0.0f);
glm::vec3 blackboardPos = cubePos + glm::vec3(0.0f, 0.0f, -roomDepth + 0.002f);
glm::vec3 windmillPos = cubePos + glm::vec3(0.0f, 0.0f, -roomDepth + 0.003f);
glm::vec3 tablePos = floorMidPos + glm::vec3(-0.2f, 0.0f, 0.42f);


// 地形设置
// 地形MinHeight
float terrainMinHeight = 0.0f;
// 地形MaxHeight
float terrainMaxHeight = 0.05f;
// 地形大小
float terrainRadius = 0.15f;
// 地形粗糙度
float terrainRoughness = 2.0f;
// 地形纹理块大小
float terrainTextureSize = 0.1f;


// 圆柱
float terrainBaseHeight = 0.02f;
// 渲染圆柱共旋转多少次？
const int terrainRotateTimes = 60;
// 顶点数组的大小
const unsigned int terrainBaseVerticesSize = 6 * 6 * terrainRotateTimes;


float treeScale = 0.05f;
//float tableScale = 0.01f;
float tableScale = 0.012f;


// 地形位置
glm::vec3 terrainBasePos = tablePos + glm::vec3(0.2f, 0.36f, -0.15f);
glm::vec3 terrainPos = terrainBasePos + glm::vec3(0.0f, terrainBaseHeight, 0.0f);
glm::vec3 treePos = terrainPos + glm::vec3(0.0f, 0.01f, 0.0f);

// 光照设置
glm::vec3 lightPos(0.0f, 0.6f, -0.35f);

// 光照颜色
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

glm::vec3 lightAmbient = 0.5f * lightColor;
glm::vec3 lightDiffuse = 0.2f * lightColor;
glm::vec3 lightSpecular = lightColor;

// 颜色
glm::vec3 FWallColor(1.0f, 1.0f, 1.0f);
glm::vec3 LWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 RWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 CeilingColor(0.5f, 0.5f, 0.5f);
glm::vec3 FloorColor(0.5f, 0.5f, 0.5f);
glm::vec3 blackboardColor(0.0f, 0.0f, 0.0f);
glm::vec3 blackboardBorderColor(1.0f, 1.0f, 0.0f);
glm::vec3 lightCubeColor(1.0f, 1.0f, 1.0f);
glm::vec3 terrainBaseColor(160.0f / 255.0f/4, 82.0f / 255.0f/4, 45.0f / 255.0f/4);


// 统一设置用到的坐标信息(每一行前三个数字为点的坐标，后三个为法向量)
   // ------------------------------------------------------------------
float vertices[] = {
    // 后面
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
    -roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, -1.0f,

    // 前面
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
    -roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  1.0f,

    // 左面
    -roomWidth,  roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight,  roomDepth, -1.0f,  0.0f,  0.0f,

    // 右面
     roomWidth,  roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight,  roomDepth,  1.0f,  0.0f,  0.0f,

     // 底面
     -roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
     -roomWidth, -roomHeight,  roomDepth,  0.0f, -1.0f,  0.0f,
     -roomWidth, -roomHeight, -roomDepth,  0.0f, -1.0f,  0.0f,

     // 顶面
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
     -roomWidth,  roomHeight,  roomDepth,  0.0f,  1.0f,  0.0f,
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  1.0f,  0.0f
};

//获取各个平面的数据
// ------------------------------------------------------------------
const unsigned int CeilingVerticesSize = 36;
const unsigned int FloorVerticesSize = 36;
const unsigned int LWallVerticesSize = 36;
const unsigned int RWallVerticesSize = 36;
const unsigned int FWallVerticesSize = 36;
const unsigned int blackboardVerticesSize = 36;
const unsigned int blackboardBorderVerticesSize = 36;
const unsigned int windmillVerticesSize = 144;
const unsigned int lightCubeVerticesSize = 216;


float CeilingVertices[CeilingVerticesSize];
float FloorVertices[FloorVerticesSize];
float LWallVertices[LWallVerticesSize];
float RWallVertices[RWallVerticesSize];
float FWallVertices[FWallVerticesSize];

float terrainBaseVertices[terrainBaseVerticesSize];
const unsigned int terrainTopVerticesSize = 3 * 6 * terrainRotateTimes;
float terrainTopVertices[terrainTopVerticesSize];


// 黑板的顶点信息
float blackboardVertices[blackboardVerticesSize] = {
    // positions                      // normals
    -blackboardWidth,  blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f,
    blackboardWidth,  blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f,
    blackboardWidth, -blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f,
    blackboardWidth, -blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f,
    -blackboardWidth, -blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f,
    -blackboardWidth,  blackboardHeight, 0.0f,  0.0f, 0.0f, 1.0f
};

// 黑板框的顶点信息
float blackboardBorderVertices[blackboardBorderVerticesSize] = {
    // positions                      // normals
    -blackboardWidth - blackboardBorderSize,  blackboardHeight + blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f,
    blackboardWidth + blackboardBorderSize,  blackboardHeight + blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f,
    blackboardWidth + blackboardBorderSize, -blackboardHeight - blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f,
    blackboardWidth + blackboardBorderSize, -blackboardHeight - blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f,
    -blackboardWidth - blackboardBorderSize, -blackboardHeight - blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f,
    -blackboardWidth - blackboardBorderSize,  blackboardHeight + blackboardBorderSize, -0.001f,  0.0f, 0.0f, 1.0f
};

// 载入风车的顶点信息
float windmillVertices[windmillVerticesSize] = {
    // positions                      // normals
    // 三角形1
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength, windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形2
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    0.0f, -windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength, -windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形3
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    0.0f, windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength, windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形4
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength, -windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形5
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength / 2, windmillBladeLength / 2, 0.0f,  0.0f, 0.0f, -1.0f,
    0.0f, windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形6
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength / 2, windmillBladeLength / 2, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形7
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    -windmillBladeLength / 2, -windmillBladeLength / 2, 0.0f,  0.0f, 0.0f, -1.0f,
    0, -windmillBladeLength, 0.0f,  0.0f, 0.0f, -1.0f,
    // 三角形8
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength / 2, -windmillBladeLength / 2, 0.0f,  0.0f, 0.0f, -1.0f,
    windmillBladeLength, 0, 0.0f,  0.0f, 0.0f, -1.0f,
};

// 雪粒子：每个顶点包含8个数据，分别为x, y, z, nx, ny, nz, u, v
// 是一个立方体
float snowVertices[] = {
	// positions          // normals           // texture coords
	// front face
	-0.01f, -0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	 0.01f, -0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
	 0.01f,  0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	 0.01f,  0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
	-0.01f,  0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
	-0.01f, -0.01f,  0.01f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
	// back face
	-0.01f, -0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
	 0.01f, -0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
	 0.01f,  0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 0.01f,  0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
	 -0.01f,  0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.01f, -0.01f, -0.01f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    // left face
    -0.01f,  0.01f,  0.01f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.01f,  0.01f, -0.01f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.01f, -0.01f, -0.01f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.01f, -0.01f, -0.01f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.01f, -0.01f,  0.01f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.01f,  0.01f,  0.01f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    // right face
    0.01f,  0.01f,  0.01f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.01f,  0.01f, -0.01f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.01f, -0.01f, -0.01f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.01f, -0.01f, -0.01f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.01f, -0.01f,  0.01f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.01f,  0.01f,  0.01f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    // bottom face
    -0.01f, -0.01f, -0.01f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.01f, -0.01f, -0.01f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.01f, -0.01f,  0.01f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.01f, -0.01f,  0.01f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.01f, -0.01f,  0.01f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.01f, -0.01f, -0.01f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    // top face
    -0.01f,  0.01f, -0.01f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.01f,  0.01f, -0.01f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.01f,  0.01f,  0.01f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.01f,  0.01f,  0.01f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.01f,  0.01f,  0.01f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.01f,  0.01f, -0.01f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

unsigned int snowVerticesSize = 8 * 36;


/// ///////////////
// 雪粒子：点碰撞箱
Point snowShape(glm::vec3(0.0f, 0.0f, 0.0f));
// 圣诞树：圆锥碰撞箱
// 底面半径
float treeBaseRadius = 0.10f / 3;
// 高度
float treeHeight = 0.10f;
Cone treeShape(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), treeBaseRadius, treeHeight);
// 地形：圆柱碰撞箱
Cylinder terrainShape(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), terrainRadius, terrainBaseHeight);

/// ///////////////
Geometry treeGeometry(treePos, glm::vec3(treeScale, treeScale, treeScale), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry tableGeometry(tablePos, glm::vec3(tableScale, tableScale, tableScale), glm::vec3(-90.0f, 0.0f, 0.0f));
Geometry roomGeometry(cubePos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry lightCubeGeometry(lightPos, glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry blackboardGeometry(blackboardPos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry windmillGeometry(windmillPos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry terrainGeometry(terrainPos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry terrainBaseGeometry(terrainBasePos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry snowGeometry(glm::vec3(0.0f), glm::vec3(0.000001f * 2.5f), glm::vec3(0.0f));
Geometry starGeometry(treePos + glm::vec3(0.0f, treeHeight - 0.003f, 0.0f), glm::vec3(0.0006f), glm::vec3(90.0f, 0.0f, 0.0f));
Geometry lightGeometry(treePos + glm::vec3(0.0f, treeHeight, 0.0f), glm::vec3(0.001f, 0.001f, 0.001f) * 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
//Geometry lightGeometry(treePos + glm::vec3(0.0f, treeHeight, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f), glm::vec3(0.0f, 0.0f, 0.0f));

//纹理
std::vector<std::string> texturePaths = {
	"resources/textures/seamless+.jpg",
	"resources/textures/seamless.jpg",
	"resources/textures/tilable.png",
	"resources/textures/water.png"
};

std::vector<std::string> snowTexturePaths = {
	"resources/textures/snow.png"
};


void initData() {
    std::copy(vertices + 180, vertices + 216, CeilingVertices);
    std::copy(vertices + 144, vertices + 180, FloorVertices);
    std::copy(vertices + 72, vertices + 108, LWallVertices);
    std::copy(vertices + 108, vertices + 144, RWallVertices);
    std::copy(vertices + 0, vertices + 36, FWallVertices);

	float angle = 360.0f / terrainRotateTimes;

    // 构造圆柱
    for (int i = 0; i < terrainRotateTimes; ++i) {
        // 旋转得到x与z
        float x = terrainRadius * cos(glm::radians(i * angle));
        float z = terrainRadius * sin(glm::radians(i * angle));
        // 旋转得到法向量
        float nx = cos(glm::radians(i * angle));
        float nz = sin(glm::radians(i * angle));

        // 顶点1
        terrainBaseVertices[i * 36 + 0] = x;
        terrainBaseVertices[i * 36 + 1] = 0.0f;
        terrainBaseVertices[i * 36 + 2] = z;
        terrainBaseVertices[i * 36 + 3] = nx;
        terrainBaseVertices[i * 36 + 4] = 0.0f;
        terrainBaseVertices[i * 36 + 5] = nz;

        // 顶点2
        terrainBaseVertices[i * 36 + 6] = x;
		terrainBaseVertices[i * 36 + 7] = terrainBaseHeight;
        terrainBaseVertices[i * 36 + 8] = z;
        terrainBaseVertices[i * 36 + 9] = nx;
        terrainBaseVertices[i * 36 + 10] = 0.0f;
        terrainBaseVertices[i * 36 + 11] = nz;

        // 顶点3
        float nextX = terrainRadius * cos(glm::radians((i + 1) * angle));
        float nextZ = terrainRadius * sin(glm::radians((i + 1) * angle));
        float nextNx = cos(glm::radians((i + 1) * angle));
        float nextNz = sin(glm::radians((i + 1) * angle));

        terrainBaseVertices[i * 36 + 12] = nextX;
		terrainBaseVertices[i * 36 + 13] = 0.0f;
        terrainBaseVertices[i * 36 + 14] = nextZ;
        terrainBaseVertices[i * 36 + 15] = nextNx;
        terrainBaseVertices[i * 36 + 16] = 0.0f;
        terrainBaseVertices[i * 36 + 17] = nextNz;

        // 顶点4
        terrainBaseVertices[i * 36 + 18] = nextX;
        terrainBaseVertices[i * 36 + 19] = terrainBaseHeight;
        terrainBaseVertices[i * 36 + 20] = nextZ;
        terrainBaseVertices[i * 36 + 21] = nextNx;
        terrainBaseVertices[i * 36 + 22] = 0.0f;
        terrainBaseVertices[i * 36 + 23] = nextNz;

        // 顶点5
        terrainBaseVertices[i * 36 + 24] = x;
        terrainBaseVertices[i * 36 + 25] = terrainBaseHeight;
        terrainBaseVertices[i * 36 + 26] = z;
        terrainBaseVertices[i * 36 + 27] = nx;
        terrainBaseVertices[i * 36 + 28] = 0.0f;
        terrainBaseVertices[i * 36 + 29] = nz;

        // 顶点6
        terrainBaseVertices[i * 36 + 30] = nextX;
        terrainBaseVertices[i * 36 + 31] = 0.0f;
        terrainBaseVertices[i * 36 + 32] = nextZ;
        terrainBaseVertices[i * 36 + 33] = nextNx;
        terrainBaseVertices[i * 36 + 34] = 0.0f;
        terrainBaseVertices[i * 36 + 35] = nextNz;
    }

    // 圆柱顶
    for (int i = 0; i < terrainRotateTimes; ++i) {
        // 顶点1
        terrainTopVertices[i * 18 + 0] = terrainBaseVertices[i * 36 + 18];
        terrainTopVertices[i * 18 + 1] = terrainBaseVertices[i * 36 + 19];
        terrainTopVertices[i * 18 + 2] = terrainBaseVertices[i * 36 + 20];
        terrainTopVertices[i * 18 + 3] = 0.0f;
        terrainTopVertices[i * 18 + 4] = 1.0f;
        terrainTopVertices[i * 18 + 5] = 0.0f;

        // 顶点2
        terrainTopVertices[i * 18 + 6] = terrainBaseVertices[i * 36 + 24];
        terrainTopVertices[i * 18 + 7] = terrainBaseVertices[i * 36 + 25];
        terrainTopVertices[i * 18 + 8] = terrainBaseVertices[i * 36 + 26];
        terrainTopVertices[i * 18 + 9] = 0.0f;
        terrainTopVertices[i * 18 + 10] = 1.0f;
        terrainTopVertices[i * 18 + 11] = 0.0f;

        // 顶点3
        terrainTopVertices[i * 18 + 12] = 0.0f;
        terrainTopVertices[i * 18 + 13] = terrainBaseHeight;
        terrainTopVertices[i * 18 + 14] = 0.0f;
        terrainTopVertices[i * 18 + 15] = 0.0f;
        terrainTopVertices[i * 18 + 16] = 1.0f;
        terrainTopVertices[i * 18 + 17] = 0.0f;

    }
}

// 第二个光源
// 位置
glm::vec3 lightPos2 = treePos + glm::vec3(0.0f, treeHeight - 0.005f, 0.01f);
// 颜色
glm::vec3 lightColor2 = glm::vec3(1.0f, 0.0f, 0.0f);
// constant, linear, and quadratic coefficients
float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;