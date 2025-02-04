#include "Map.h"

#include "Camera.h"

const float Cell::OUTER = 50.0f;
const float Cell::INNER = Cell::OUTER * 0.96f;

Map* Map::m_Current = nullptr;

Map::Map()
{
	int vertexCnt = COLUMN_COUNT * ROW_COUNT * 2 * 4;
	int rectangleCnt = COLUMN_COUNT * ROW_COUNT * 2;
	int indexCnt = rectangleCnt * 6;

	unsigned int vertex = 0, index = 0;
	struct Vertice {
		struct { float x, y; } pos;
		struct { float r, g, b, a; } color;
	}*vertices = new Vertice[vertexCnt];
	struct Rectangle { struct { unsigned int p1, p2, p3; }t1, t2; }*indices = new Rectangle[rectangleCnt];

	struct { int x, y; } vertexDirs[] = { {-1,1},{-1,-1},{1,-1},{1,1} };
	for (int i = 0; i < COLUMN_COUNT; i++)
		for (int j = 0; j < ROW_COUNT; j++, index += 2, vertex += 4) {
			m_Cells[i][j] = { (i + 0.5f) * Cell::OUTER, (j + 0.5f) * Cell::OUTER, nullptr };

			indices[index] = { { index * 4, index * 4 + 1, index * 4 + 2 }, { index * 4 + 2, index * 4 + 3, index * 4 } };
			indices[index + 1] = { { index * 4 + 4, index * 4 + 5, index * 4 + 6 }, { index * 4 + 6, index * 4 + 7, index * 4 + 4 } };

			for (int k = 0; k < 4; k++, vertex++) {
				vertices[vertex] =
				{
					{
						m_Cells[i][j].Position.x + vertexDirs[k].x * Cell::OUTER / 2,
						m_Cells[i][j].Position.y + vertexDirs[k].y * Cell::OUTER / 2
					},
					{ 0.2f, 0.3f, 0.8f, 1.0f }
				};
				vertices[vertex + 4] =
				{
					{
						m_Cells[i][j].Position.x + vertexDirs[k].x * Cell::INNER / 2,
						m_Cells[i][j].Position.y + vertexDirs[k].y * Cell::INNER / 2
					},
					{ 0.1f, 0.1f, 0.1f, 1.0f }
				};
			}
		}

	m_IndexBuffer = std::make_unique<IndexBuffer>((unsigned int*)indices, indexCnt);
	delete[] indices;
	m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, vertexCnt * sizeof(Vertice));
	delete[] vertices;
	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(4);
	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
	m_Shader = std::make_unique<Shader>("res/shaders/Color.shader");
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
	glm::mat4 mvp = Camera::Current().GetProj() * Camera::Current().GetView() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
	Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
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
