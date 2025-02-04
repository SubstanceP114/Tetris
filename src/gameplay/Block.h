#pragma once

#include "Map.h"

#include <functional>

struct Vec2
{
	int x, y;
	inline Vec2 operator + (const Vec2& other) { return { x + other.x, y + other.y }; }
	inline void operator += (const Vec2& other) { x += other.x, y += other.y; }
	inline Vec2 operator - () { return { -x, -y }; }
};
struct Vec4 { float r, g, b, a; };

class Block : public Object
{
private:
	Vec2 m_Center;
	Vec2 m_Offsets[4];
	Vec4 m_Color;

	static Block* m_Current;
	static Block* m_Preview;

	inline void ForEach(std::function<void(Vec2)> func) { for (int i = 0; i < 4; i++) func(m_Offsets[i] + m_Center); }
	inline bool All(std::function<bool(Vec2)> func)
	{
		for (int i = 0; i < 4; i++)
			if (!func(m_Offsets[i] + m_Center))
				return false;
		return true;
	}

	void Rotate(bool anti);
	void Move(Vec2 dir);
	void Switch();

	Block(Vec2 cell1, Vec2 cell2, Vec2 cell3);
	Block(const Block& block, Vec4 color);
public:
	void Init() override;
	void Update(float deltaTime) override;
	void OnGuiLeft() override;
	void OnGuiRight() override;

	static Block* Random();

	inline Vec4 GetColor() const { return m_Color; }
};

