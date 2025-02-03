#include "Map.h"

const float Cell::SIZE = 50.0f;

Map* Map::m_Current = nullptr;

Map::Map()
{
	for (int i = 0; i < COLUMN_COUNT; i++)
		for (int j = 0; j < ROW_COUNT; j++)
			m_Cells[i][j] = { ORIGIN.X + (i + 0.5f) * Cell::SIZE, ORIGIN.Y + (j + 0.5f) * Cell::SIZE, true };
}

Map::~Map()
{
	m_Current = nullptr;
}

void Map::Update(float deltaTime)
{
}

void Map::Render()
{
}

void Map::OnGuiLeft()
{
}

void Map::OnGuiRight()
{
}

Map* Map::Current()
{
	if (m_Current == nullptr)
		m_Current = new Map();
	return m_Current;
}
