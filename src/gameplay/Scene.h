#pragma once

#include "Object.h"

#include "Renderer.h"

#include <vector>
#include <queue>

class Scene
{
private:
	std::vector<Object*> m_Objects;
	std::queue<Object*> m_WaitQueue;
	GLFWwindow* m_Window;

	Scene() :m_Window(nullptr) {}
	~Scene() {}
	Scene(const Scene& scene) = delete;
	const Scene& operator = (const Scene& scene) = delete;
public:
	void Init(GLFWwindow* window);
	void Update(float deltaTime);
	void OnGuiLeft();
	void OnGuiRight();
	void Delete();

	static Scene& Current();

	inline void Add(Object* object) { m_WaitQueue.push(object); }
	inline GLFWwindow* GetWindow() const { return m_Window; }

	const struct { const float WIDTH = 1600.0f, HEIGHT = 900.0f; } SIZE;
};