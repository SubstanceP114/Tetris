#include "Block.h"

#include "Scene.h"

#include <random>

Block* Block::m_Current = nullptr;
Block* Block::m_Preview = nullptr;

void Block::Rotate(bool anti = true)
{
	static auto rotate = [this](bool anti) {
		for (int i = 1; i < 4; i++) {
			Vec2 temp = m_Offsets[i];
			m_Offsets[i] = { temp.y * anti ? -1 : 1, temp.x * anti ? 1 : -1 };
		}};
	ForEach([](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, nullptr); });
	rotate(anti);
	if (All([](Vec2 cell) { return Map::Current()->IsValid(cell.x, cell.y) && Map::Current()->IsEmpty(cell.x, cell.y); }))
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	else {
		rotate(!anti);
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	}
}

void Block::Switch()
{
	m_Current = m_Preview;
	m_Preview = Random();
	Scene::Current().Add(m_Preview);
}

Block::Block(Vec2 cell1, Vec2 cell2, Vec2 cell3)
	: m_Center{ Map::Current()->COLUMN_COUNT / 2, Map::Current()->ROW_COUNT - 1 }
	, m_Offsets{ {0, 0} ,cell1, cell2, cell3 }, m_Color()
{
}

Block::Block(const Block& block, Vec4 color)
	: m_Center{ Map::Current()->COLUMN_COUNT / 2, Map::Current()->ROW_COUNT - 1 }
	, m_Offsets{ block.m_Offsets[0] ,block.m_Offsets[1] ,block.m_Offsets[2] ,block.m_Offsets[3] }, m_Color(color)
{
}

void Block::Init()
{
	if (m_Current == nullptr) m_Current = this;
	else if (m_Preview == nullptr) m_Preview = this;
}

void Block::Update(float deltaTime)
{
	if (m_Current != this) return;
	ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	static float timer = 0.0f;
	const static float INTERVAL = 1.0f;
	timer += deltaTime;
	if (timer > INTERVAL) {
		timer = 0.0f;
		ForEach([](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, nullptr); });
		m_Center += Vec2{ 0, -1 };
		if (All([](Vec2 cell) { return Map::Current()->IsValid(cell.x, cell.y) && Map::Current()->IsEmpty(cell.x, cell.y); }))
			ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
		else {
			m_Center += Vec2{ 0, 1 };
			ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
			Switch();
		}
	}
}

void Block::OnGuiLeft()
{
}

void Block::OnGuiRight()
{
	if (m_Preview != this) return;

}

Block* Block::Random()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> color(50, 200);
	static std::uniform_int_distribution<> index(0, 4);
	static Block blocks[] = {
		{{ 1, 0 },{ 2, 0 },{ 3, 0 }},
		{{ 0,-1 },{-1, 0 },{ 1, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 2, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 1,-1 }},
		{{ 0,-1 },{-1, 0 },{ 1,-1 }}
	};
	return new Block(blocks[index(gen)], { (float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,1.0f });
}
