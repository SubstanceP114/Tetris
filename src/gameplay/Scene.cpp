#include "Scene.h"

#include "Map.h"
#include "Block.h"

void Scene::Init(GLFWwindow* window)
{
	m_Window = window;
	Add(Map::Current());
	Add(Block::Random());
	Add(Block::Random());
}

void Scene::Update(float deltaTime)
{
	for (Object* object : m_Objects)
		if (object && object->IsDestroyed()) {
			delete object;
			object = nullptr;
		}
	for (auto it = m_Objects.begin(); it != m_Objects.end();)
		if (*it == nullptr) it = m_Objects.erase(it);
		else it++;

	while (!m_WaitQueue.empty()) {
		Object* object = m_WaitQueue.front();
		object->Init();
		m_Objects.push_back(object);
		m_WaitQueue.pop();
	}

	for (Object* object : m_Objects) if (object) object->Update(deltaTime);
	for (Object* object : m_Objects) if (object) object->Render();
	for (Object* object : m_Objects) if (object && object->IsDestroyed()) object->OnDestroy();
}

void Scene::OnGuiLeft()
{
	for (Object* object : m_Objects) if (object) object->OnGuiLeft();
}

void Scene::OnGuiRight()
{
	for (Object* object : m_Objects) if (object) object->OnGuiRight();
}

void Scene::Delete()
{
	for (Object* object : m_Objects) if (object) delete object;
	while (!m_WaitQueue.empty())
	{
		Object* object = m_WaitQueue.front();
		if (object) delete object;
		m_WaitQueue.pop();
	}
}

Scene& Scene::Current()
{
	static Scene scene;
	return scene;
}
