#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "Map.h"

class Camera
{
private:
	glm::mat4 m_Proj, m_View;

	Camera()
		: m_Proj(glm::ortho(0.0f, 1600.0f, 0.0f, 900.0f, -1.0f, 1.0f))
		, m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))) {}
	~Camera() {}
	Camera(const Camera& camera) = delete;
	const Camera& operator = (const Camera& camera) = delete;
public:
	inline glm::mat4 GetProj() const { return m_Proj; }
	inline glm::mat4 GetView() const { return m_View; }

	static Camera& Current() {
		static Camera camera;
		return camera;
	}
};