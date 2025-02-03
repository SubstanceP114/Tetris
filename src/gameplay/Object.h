#pragma once
class Object
{
private:
	bool m_Destroyed = false;
public:
	Object() {}
	virtual ~Object() {}

	virtual void Init() {}
	virtual void Update(float deltaTime) {}
	virtual void OnRender() {}
	virtual void OnGuiLeft() {}
	virtual void OnGuiRight() {}
	virtual void OnDestroy() {}

	inline void Destroy() { m_Destroyed = true; }
	inline bool IsDestroyed() const { return m_Destroyed; }
};