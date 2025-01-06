#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

// 贝塞尔曲线
// 使用De Casteljau 算法：
class BezierCurve {
public:
	std::vector<glm::vec3> controlPoints;
	std::vector<glm::vec3> curvePoints;

	BezierCurve(std::vector<glm::vec3> controlPoints) {
		this->controlPoints = controlPoints;
	}

	// 计算曲线上的点
	void calCurvePoints(int numSamples = 100) {
		curvePoints.clear();
		for (int i = 0; i <= numSamples; i++) {
			float t = (float)i / numSamples;
			curvePoints.push_back(deCasteljau(t));
		}
	}

	// De Casteljau 算法
	glm::vec3 deCasteljau(float t) {
		std::vector<glm::vec3> points = controlPoints;
		while (points.size() > 1) {
			std::vector<glm::vec3> newPoints;
			for (int i = 0; i < points.size() - 1; i++) {
				newPoints.push_back((1 - t) * points[i] + t * points[i + 1]);
			}
			points = newPoints;
		}
		return points[0];
	}

	glm::vec3 getPoint(int i) {
		return curvePoints[i];
	}
};
