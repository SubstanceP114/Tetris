#pragma once

#include "Object.h"
#include "Renderer.h"

#include <memory>
#include <functional>

struct Cell
{
	struct { float x, y; } Position;
	Object* Item;
	static const float OUTER;
	static const float INNER;
};

class Map : public Object
{
private:
	static Map* m_Current;

	Cell m_Cells[12][18];
	int m_LineCnt;
	float m_Timer;

	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
	std::unique_ptr<Shader> m_Shader;

	struct Vertice {
		struct { float x, y; } pos;
		struct { float r, g, b, a; } color;
		struct { float x, y; } offset;
	}*m_Vertices;
	struct Rectangle { struct { unsigned int p1, p2, p3; }t1, t2; }*m_Indices;

	inline bool All(std::function<bool(const Cell&)> func) const
	{
		for (int i = 0; i < COLUMN_COUNT; i++)
			for (int j = 0; j < ROW_COUNT; j++)
				if (!func(m_Cells[i][j]))
					return false;
		return true;
	}
	inline void SetRow(int row, std::function<void(Cell&)> func) { for (int i = 0; i < COLUMN_COUNT; i++) func(m_Cells[i][row]); }
	inline bool Row(int row, std::function<bool(const Cell&)> func) const
	{
		for (int i = 0; i < COLUMN_COUNT; i++)
			if (!func(m_Cells[i][row]))
				return false;
		return true;
	}
public:
	Map();
	~Map() override;

	void Init() override;
	void Update(float deltaTime) override;
	void Render() override;
	void OnGuiLeft() override;
	void OnGuiRight() override;

	static Map* Current();

	inline bool IsValid(int x, int y) const { return x >= 0 && x < COLUMN_COUNT && y >= 0 && y < ROW_COUNT; }
	inline bool IsEmpty(int x, int y) const { return m_Cells[x][y].Item == nullptr; }
	inline void SetCell(int x, int y, Object* item) { m_Cells[x][y].Item = item; }

	void Refresh();

	const int COLUMN_COUNT = 12;
	const int ROW_COUNT = 18;
	const struct { const float WIDTH = 600.0f, HEIGHT = 900.0f; } SIZE;
	const struct { const float X = 500.0f, Y = 0.0f; } ORIGIN;
};

