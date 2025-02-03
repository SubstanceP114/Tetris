#pragma once

#include "Object.h"

#include <vector>
#include <queue>

class Scene
{
private:
	std::vector<Object*> m_Objects;
	static Scene* m_Current;

	std::queue<Object*> m_WaitQueue;
public:
	Scene();
	~Scene();

	void Update(float deltaTime);
	void OnGuiLeft();
	void OnGuiRight();

	inline static Scene* Current() { return m_Current; }

	void Add(Object* object);
};