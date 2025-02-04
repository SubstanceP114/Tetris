#pragma once

#include "Object.h"

#include <vector>
#include <queue>

class Scene
{
private:
	std::vector<Object*> m_Objects;
	std::queue<Object*> m_WaitQueue;

	Scene() {}
	~Scene() {}
	Scene(const Scene& scene) = delete;
	const Scene& operator = (const Scene& scene) = delete;
public:
	void Init();
	void Update(float deltaTime);
	void OnGuiLeft();
	void OnGuiRight();
	void Delete();

	static Scene& Current();

	void Add(Object* object);

	const struct { const float WIDTH = 1600.0f, HEIGHT = 900.0f; } SIZE;
};