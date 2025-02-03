#pragma once

#include "Object.h"
#include "Renderer.h"

#include <memory>

struct Cell
{
	struct { float x, y; } Position;
	bool Empty;
	static const float SIZE;
};

class Map : public Object
{
private:
	static Map* m_Current;
	Cell m_Cells[12][18];

	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;
public:
	Map();
	~Map() override;

	void Update(float deltaTime) override;
	void Render() override;
	void OnGuiLeft() override;
	void OnGuiRight() override;

	static Map* Current();

	const int COLUMN_COUNT = 12;
	const int ROW_COUNT = 18;
	const struct { const float WIDTH = 600.0f, HEIGHT = 900.0f; } SIZE;
	const struct { const float X = 500.0f, Y = 0.0f; } ORIGIN;
};

