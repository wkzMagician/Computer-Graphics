#pragma once

#include "geometry.h"
#include "shader.h"
#include "camera.h"
#include "material.h"
#include "shape.h"
#include "bezier.h"
#include <vector>
#include <array>

#include <algorithm> // 添加此行以包含 std::copy

// 窗口设置
const unsigned int SCR_WIDTH = 1024 * 1.6;
const unsigned int SCR_HEIGHT = 800 * 1.6;

// 房间大小
float roomWidth = 1.0f;
float roomHeight = 0.6f;
float roomDepth = 1.0f;

// 全局坐标
glm::vec3 cubePos(0.0f, 0.0f, 0.0f);

// 摄像机设置
Camera camera(glm::vec3(0.0f, 0.0f, roomDepth));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// 面光源
// 长宽高
float areaLightWidth = 0.2f;
float areaLightHeight = 0.02f;
float areaLightDepth = 0.2f;
// 位置
glm::vec3 areaLightPos(0.0f, roomHeight - areaLightHeight, 0.0f);

// 采样点数
int sampleNum = 100;
// 采样点数组
std::vector<glm::vec3> samplePoints;

// 每个点光源的颜色
glm::vec3 lightColor(1.0f / sampleNum * 1.5, 1.0f / sampleNum * 1.5, 1.0f / sampleNum * 1.5);

//glm::vec3 lightAmbient = 0.2f * lightColor;
//glm::vec3 lightDiffuse = 0.8f * lightColor;
//glm::vec3 lightSpecular = 1.0f * lightColor;
//Light light(lightAmbient, lightDiffuse, lightSpecular);

// 墙壁颜色
glm::vec3 FWallColor(1.0f, 1.0f, 1.0f);
glm::vec3 LWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 RWallColor(1.0f, 0.0f, 0.31f);
glm::vec3 CeilingColor(0.5f, 0.5f, 0.5f);
glm::vec3 FloorColor(0.5f, 0.5f, 0.5f);
glm::vec3 lightCubeColor(1.0f, 1.0f, 1.0f);

// BRDF材质
// 金属度：影响高光部分
float FWallMetallic = 0.2f;
// 粗糙度
float FWallRoughness = 0.75f;
// 环境光遮蔽
float FWallAmbientOcclusion = 0.5f;

Material_BRDF FWallMaterial(FWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF LWallMaterial(LWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF RWallMaterial(RWallColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF CeilingMaterial(CeilingColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);
Material_BRDF FloorMaterial(FloorColor, FWallMetallic, FWallRoughness, FWallAmbientOcclusion);

// 金色的管道
glm::vec3 pipeColor(0.8f, 0.6f, 0.2f);
float pipeMetallic = 0.8f;
float pipeRoughness = 0.3f;
float pipeAmbientOcclusion = 1.0f;

Material_BRDF pipeMaterial(pipeColor, pipeMetallic, pipeRoughness, pipeAmbientOcclusion);


// 统一设置用到的坐标信息(每一行前三个数字为点的坐标，后三个为法向量)
   // ------------------------------------------------------------------
float vertices[] = {
    // 后面
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
     roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
    -roomWidth,  roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,
    -roomWidth, -roomHeight, -roomDepth,  0.0f,  0.0f, 1.0f,

    // 前面
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
     roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
    -roomWidth,  roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,
    -roomWidth, -roomHeight,  roomDepth,  0.0f,  0.0f,  -1.0f,

    // 左面
    -roomWidth,  roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight, -roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth, -roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,
    -roomWidth,  roomHeight,  roomDepth, 1.0f,  0.0f,  0.0f,

    // 右面
     roomWidth,  roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight, -roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth, -roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,
     roomWidth,  roomHeight,  roomDepth,  -1.0f,  0.0f,  0.0f,

     // 底面
     -roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
      roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
     -roomWidth, -roomHeight,  roomDepth,  0.0f, 1.0f,  0.0f,
     -roomWidth, -roomHeight, -roomDepth,  0.0f, 1.0f,  0.0f,

     // 顶面
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
      roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
     -roomWidth,  roomHeight,  roomDepth,  0.0f,  -1.0f,  0.0f,
     -roomWidth,  roomHeight, -roomDepth,  0.0f,  -1.0f,  0.0f
};

float areaLightVertices[] = {
    // 后面
    -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
    -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f,  0.0f, -1.0f,

    // 前面
    -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
    -areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,
    -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f,  0.0f,  1.0f,

    // 左面
    -areaLightWidth,  areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth,  areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight, -areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth, -areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,
    -areaLightWidth,  areaLightHeight,  areaLightDepth, -1.0f,  0.0f,  0.0f,

    // 右面
     areaLightWidth,  areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth,  areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight, -areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth, -areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,
     areaLightWidth,  areaLightHeight,  areaLightDepth,  1.0f,  0.0f,  0.0f,

     // 底面
     -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
      areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
     -areaLightWidth, -areaLightHeight,  areaLightDepth,  0.0f, -1.0f,  0.0f,
     -areaLightWidth, -areaLightHeight, -areaLightDepth,  0.0f, -1.0f,  0.0f,

     // 顶面
     -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
      areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
     -areaLightWidth,  areaLightHeight,  areaLightDepth,  0.0f,  1.0f,  0.0f,
     -areaLightWidth,  areaLightHeight, -areaLightDepth,  0.0f,  1.0f,  0.0f
};

// 天空盒贴图
std::vector<std::string> faces
{
    "resources/textures/skybox/right.jpg",
    "resources/textures/skybox/left.jpg",
    "resources/textures/skybox/top.jpg",
    "resources/textures/skybox/bottom.jpg",
    "resources/textures/skybox/front.jpg",
    "resources/textures/skybox/back.jpg"
};

const float boxSize = 100.0f;

float skyboxVertices[] = {
    // positions          
    -boxSize,  boxSize, -boxSize,
    -boxSize, -boxSize, -boxSize,
     boxSize, -boxSize, -boxSize,
     boxSize, -boxSize, -boxSize,
     boxSize,  boxSize, -boxSize,
    -boxSize,  boxSize, -boxSize,

    -boxSize, -boxSize,  boxSize,
    -boxSize, -boxSize, -boxSize,
    -boxSize,  boxSize, -boxSize,
    -boxSize,  boxSize, -boxSize,
    -boxSize,  boxSize,  boxSize,
    -boxSize, -boxSize,  boxSize,

     boxSize, -boxSize, -boxSize,
     boxSize, -boxSize,  boxSize,
     boxSize,  boxSize,  boxSize,
     boxSize,  boxSize,  boxSize,
     boxSize,  boxSize, -boxSize,
     boxSize, -boxSize, -boxSize,

    -boxSize, -boxSize,  boxSize,
    -boxSize,  boxSize,  boxSize,
     boxSize,  boxSize,  boxSize,
     boxSize,  boxSize,  boxSize,
     boxSize, -boxSize,  boxSize,
    -boxSize, -boxSize,  boxSize,

    -boxSize,  boxSize, -boxSize,
     boxSize,  boxSize, -boxSize,
     boxSize,  boxSize,  boxSize,
     boxSize,  boxSize,  boxSize,
    -boxSize,  boxSize,  boxSize,
    -boxSize,  boxSize, -boxSize,

    -boxSize, -boxSize, -boxSize,
    -boxSize, -boxSize,  boxSize,
     boxSize, -boxSize, -boxSize,
     boxSize, -boxSize, -boxSize,
    -boxSize, -boxSize,  boxSize,
     boxSize, -boxSize,  boxSize
};


//获取各个平面的数据
// ------------------------------------------------------------------
const unsigned int CeilingVerticesSize = 36;
const unsigned int FloorVerticesSize = 36;
const unsigned int LWallVerticesSize = 36;
const unsigned int RWallVerticesSize = 36;
const unsigned int FWallVerticesSize = 36;
const unsigned int lightCubeVerticesSize = 216;
const unsigned int areaLightVerticesSize = 216;
const unsigned int skyboxVerticesSize = 108;

float CeilingVertices[CeilingVerticesSize];
float FloorVertices[FloorVerticesSize];
float LWallVertices[LWallVerticesSize];
float RWallVertices[RWallVerticesSize];
float FWallVertices[FWallVerticesSize];

// 位置，缩放，旋转
Geometry roomGeometry(cubePos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry areaLightGeometry(areaLightPos, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Geometry bezierGeometry(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

// 贝塞尔曲线
// 矩形 - 椭圆 - 圆形
glm::vec2 rectSize(0.2f, 0.2f);
glm::vec2 ellipseRadius(0.1f, 0.05f);
float circleRadius = 0.05f;

Rectangle rectangle(glm::vec3(1.0f, 0.0f, 0.0f), rectSize);
Ellipse ellipse(glm::vec3(1.0f, 0.0f, 0.0f), ellipseRadius);
Circle circle(glm::vec3(1.0f, 0.0f, 0.0f), circleRadius);

// 关键面的采样点数量
int shapeSampleNum = 120;
// 采样得到的顶点
std::vector<glm::vec3> rectSamplePoints;
std::vector<glm::vec3> ellipseSamplePoints;
std::vector<glm::vec3> circleSamplePoints;

// 控制顶点的坐标
std::vector<glm::vec3> controlPoints = {
	glm::vec3(-roomWidth / 2, roomHeight / 4, -roomDepth / 2),
	glm::vec3(-roomWidth / 4, -roomHeight / 8, -roomDepth / 4),
	glm::vec3(roomWidth / 4, roomHeight / 8, roomDepth / 4),
	glm::vec3(roomWidth / 2, -roomHeight / 4, roomDepth / 2)
};

//std::vector<glm::vec3> controlPoints = {
//    glm::vec3(-roomWidth / 2, -roomHeight / 4, 0.0f),
//    glm::vec3(-roomWidth / 4, 0.0f, 0.0f),
//    glm::vec3(roomWidth / 4, 0.0f, 0.0f),
//    glm::vec3(roomWidth / 2, -roomHeight / 4, 0.0f)
//};

// 贝塞尔曲线
BezierCurve bezierCurve(controlPoints);
int bezierSampleNum = 120; // 贝塞尔曲线的采样点数量

// 三角面片的顶点
// 六维，位置与法向量
class MyVertex {
public:
    glm::vec3 position;
    glm::vec3 normal;

	MyVertex(float x, float y, float z, float nx, float ny, float nz) : position(x, y, z), normal(nx, ny, nz) {}
	MyVertex(glm::vec3 pos, glm::vec3 nor) : position(pos), normal(nor) {}
};
using Layer = std::vector<MyVertex>; // 每个采样点对应一个截面
std::vector<Layer> pipeVertices; // 管道所有顶点
std::vector<float> pipeVerticesData; // 管道：顶点->float数组

// 关键位置截面线
glm::vec3 rectPos;
glm::vec3 ellipsePos;
glm::vec3 circlePos;
// 截面线顶点
std::vector<float> rectVertices;
std::vector<float> ellipseVertices;
std::vector<float> circleVertices;

// 贝塞尔曲线的顶点
std::vector<float> bezierVerticesData;

// 包装函数：顶点->float数组
void addVertexData(std::vector<float>& data, const MyVertex& vertex) {
    data.push_back(vertex.position.x);
    data.push_back(vertex.position.y);
    data.push_back(vertex.position.z);
    data.push_back(vertex.normal.x);
    data.push_back(vertex.normal.y);
    data.push_back(vertex.normal.z);
}

void initAreaLight() {
    // 初始化采样点
    for (int i = 0; i < sampleNum; i++) {
        float ratio = (rand() % 1000) / 500.0f - 1.0f;
        float x = areaLightPos.x + areaLightWidth * ratio;
        ratio = (rand() % 1000) / 500.0f - 1.0f;
        float y = areaLightPos.y + areaLightHeight * ratio;
        ratio = (rand() % 1000) / 500.0f - 1.0f;
        float z = areaLightPos.z + areaLightDepth * ratio;
        samplePoints.push_back(glm::vec3(x, y, z));
    }
}

void initBezier() {
    // 初始化关键截面采样点
    rectangle.setSamplePoints(shapeSampleNum);
    ellipse.setSamplePoints(shapeSampleNum);
    circle.setSamplePoints(shapeSampleNum);
    rectSamplePoints = rectangle.getSamplePoints();
    ellipseSamplePoints = ellipse.getSamplePoints();
    circleSamplePoints = circle.getSamplePoints();

    // 计算贝塞尔曲线上的点
    bezierCurve.calCurvePoints(bezierSampleNum);

    // 关键位置截面线
    rectPos = bezierCurve.getStartPoint();
    ellipsePos = bezierCurve.getMidPoint();
    circlePos = bezierCurve.getEndPoint();

    float amp = 1.001f;

    for (auto& p : rectSamplePoints) {
        rectVertices.push_back(p.x * amp + rectPos.x);
        rectVertices.push_back(p.y * amp + rectPos.y);
        rectVertices.push_back(p.z * amp + rectPos.z);
        rectVertices.push_back(0.0f);
        rectVertices.push_back(0.0f);
        rectVertices.push_back(1.0f);
    }
    for (auto& p : ellipseSamplePoints) {
        ellipseVertices.push_back(p.x * amp + ellipsePos.x);
        ellipseVertices.push_back(p.y * amp + ellipsePos.y);
        ellipseVertices.push_back(p.z * amp + ellipsePos.z);
        ellipseVertices.push_back(0.0f);
        ellipseVertices.push_back(0.0f);
        ellipseVertices.push_back(1.0f);
    }
    for (auto& p : circleSamplePoints) {
        circleVertices.push_back(p.x * amp + circlePos.x);
        circleVertices.push_back(p.y * amp + circlePos.y);
        circleVertices.push_back(p.z * amp + circlePos.z);
        circleVertices.push_back(0.0f);
        circleVertices.push_back(0.0f);
        circleVertices.push_back(1.0f);
    }

    bezierVerticesData.clear();

    // 贝塞尔曲线的顶点
    auto& points = bezierCurve.curvePoints;
    for (auto& p : points) {
        bezierVerticesData.push_back(p.x);
        bezierVerticesData.push_back(p.y);
        bezierVerticesData.push_back(p.z);
        bezierVerticesData.push_back(0.0f);
        bezierVerticesData.push_back(0.0f);
        bezierVerticesData.push_back(1.0f);
    }
}

void initPipe() {
    pipeVertices.clear();

    // 插值采样，组装三角面片
    for (int i = 0; i <= bezierSampleNum; i++) {
        pipeVertices.push_back(Layer());

        // 矩形和椭圆之间，椭圆和圆形之间
        if (i < bezierSampleNum / 2) {
            float ratio = (float)i / (bezierSampleNum / 2.0f);
            glm::vec3 bezierPoint = bezierCurve.getPoint(i);

            for (int j = 0; j < shapeSampleNum; j++) {
                glm::vec3 p1 = rectSamplePoints[j];
                glm::vec3 p2 = ellipseSamplePoints[j];

                glm::vec3 p = p1 + ratio * (p2 - p1);

                glm::vec3 vertexPos = bezierPoint + p;

                MyVertex vertex(vertexPos, glm::vec3(0.0f, 0.0f, 0.0f));
                pipeVertices[i].push_back(vertex);
            }
        }
        else {
            float ratio = (float)(i - bezierSampleNum / 2) / (bezierSampleNum / 2.0f);

            for (int j = 0; j < shapeSampleNum; j++) {
                glm::vec3 p1 = ellipseSamplePoints[j];
                glm::vec3 p2 = circleSamplePoints[j];
                glm::vec3 p = p1 + ratio * (p2 - p1);

                glm::vec3 vertexPos = bezierCurve.getPoint(i) + p;

                MyVertex vertex(vertexPos, glm::vec3(0.0f, 0.0f, 0.0f));
                pipeVertices[i].push_back(vertex);
            }
        }
    }

    // 计算法向量
    for (int i = 0; i < bezierSampleNum; i++) {
        for (int j = 0; j < shapeSampleNum; j++) {
            MyVertex& vertex1 = pipeVertices[i][j];
            MyVertex& vertex2 = pipeVertices[i + 1][j];
            MyVertex& vertex3 = pipeVertices[i + 1][(j + 1) % shapeSampleNum];
            MyVertex& vertex4 = pipeVertices[i][(j + 1) % shapeSampleNum];

            // 三角形1: v1, v2, v3
            // 三角形2: v1, v3, v4

            // 计算法向量
            glm::vec3 v1(vertex1.position);
            glm::vec3 v2(vertex2.position);
            glm::vec3 v3(vertex3.position);
            glm::vec3 v4(vertex4.position);

            glm::vec3 normal1 = glm::normalize(glm::cross(v2 - v1, v3 - v1));
            glm::vec3 normal2 = glm::normalize(glm::cross(v3 - v1, v4 - v1));

            // !方向问题
            normal1 = -normal1;
            normal2 = -normal2;

            // vertex的后三个元素存储法向量
            vertex1.normal += normal1;
            vertex2.normal += normal1;
            vertex3.normal += normal1;

            vertex1.normal += normal2;
            vertex3.normal += normal2;
            vertex4.normal += normal2;
        }
    }

    // 归一化法向量
    for (int i = 0; i < bezierSampleNum; i++) {
        for (int j = 0; j < shapeSampleNum; j++) {
            pipeVertices[i][j].normal = glm::normalize(pipeVertices[i][j].normal);
        }
    }

    // Vertex->float数组
    for (int i = 0; i < bezierSampleNum; i++) {
        for (int j = 0; j < shapeSampleNum; j++) {
            MyVertex& vertex1 = pipeVertices[i][j];
            MyVertex& vertex2 = pipeVertices[i + 1][j];
            MyVertex& vertex3 = pipeVertices[i + 1][(j + 1) % shapeSampleNum];
            MyVertex& vertex4 = pipeVertices[i][(j + 1) % shapeSampleNum];

            // 三角形1: v1, v2, v3
            // 三角形2: v1, v3, v4

            // 存两个三角形，6个顶点
            addVertexData(pipeVerticesData, vertex1);
            addVertexData(pipeVerticesData, vertex2);
            addVertexData(pipeVerticesData, vertex3);

            addVertexData(pipeVerticesData, vertex1);
            addVertexData(pipeVerticesData, vertex3);
            addVertexData(pipeVerticesData, vertex4);
        }
    }

    // 释放内存
    pipeVertices.clear();
}

void initData() {
    std::copy(vertices + 180, vertices + 216, CeilingVertices);
    std::copy(vertices + 144, vertices + 180, FloorVertices);
    std::copy(vertices + 72, vertices + 108, LWallVertices);
    std::copy(vertices + 108, vertices + 144, RWallVertices);
    std::copy(vertices + 0, vertices + 36, FWallVertices);

	initAreaLight();
	initBezier();
	initPipe();
}
