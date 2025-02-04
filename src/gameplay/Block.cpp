#include "Block.h"

#include <random>

Block* Block::m_Current = nullptr;
Block* Block::m_Preview = nullptr;

void Block::Rotate()
{
}

void Block::Switch()
{
	m_Current = m_Preview;
	m_Preview = Random();

}

Block::Block(Vec2 cell1, Vec2 cell2, Vec2 cell3)
	:m_Cells{ {0, 0} ,cell1, cell2, cell3 }, m_Color(), m_Rotation(0)
{
}

Block::Block(const Block& block, Vec4 color)
	:m_Cells{ block.m_Cells[0] ,block.m_Cells[1] ,block.m_Cells[2] ,block.m_Cells[3] }, m_Color(color), m_Rotation(0)
{
}

void Block::Init()
{
}

void Block::Update(float deltaTime)
{
	if (m_Current != this) return;
	static float timer = 0.0f;
	const static float INTERVAL = 1.0f;
	timer += deltaTime;
	if (timer > INTERVAL) {
		timer = 0.0f;
		ForEach([](Vec2& cell) {
			
			});
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
	static std::uniform_int_distribution<> index(0, 3);
	static Block blocks[] = {
		{{ 1, 0 },{ 2, 0 },{ 3, 0 }},
		{{ 0,-1 },{-1, 0 },{ 1, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 2, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 1, 1 }}
	};
	return new Block(blocks[index(gen)], { (float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,1.0f });
}
