#include "Map.h"

#include "Block.h"
#include "Camera.h"

#include "imgui/imgui.h"

#define lerp(min, max, ratio) (min) * (1.0f - (ratio)) + (max) * (ratio)
#define curve(num) (num) * (num)

const float Cell::OUTER = 50.0f;
const float Cell::INNER = Cell::OUTER * 0.96f;

Map* Map::m_Current = nullptr;

Map::Map()
	: m_Cells(), m_LineCnt(0), m_Timer(0.0f), m_End(false)
	, m_Vertices(nullptr), m_Indices(nullptr)
{
}

Map::~Map()
{
	delete[] m_Vertices;
	delete[] m_Indices;
	m_Current = nullptr;
}

void Map::Init()
{
	int vertexCnt = COLUMN_COUNT * ROW_COUNT * 2 * 4;
	int rectangleCnt = COLUMN_COUNT * ROW_COUNT * 2;
	int indexCnt = rectangleCnt * 6;

	unsigned int vertex = 0, index = 0;
	m_Vertices = new Vertice[vertexCnt];
	m_Indices = new Rectangle[rectangleCnt];

	struct { int x, y; } vertexDirs[] = { {-1,1},{-1,-1},{1,-1},{1,1} };
	for (int i = 0; i < COLUMN_COUNT; i++)
		for (int j = 0; j < ROW_COUNT; j++, index += 2, vertex += 8) {
			m_Cells[i][j] = { (i + 0.5f) * Cell::OUTER, (j + 0.5f) * Cell::OUTER, nullptr };

			m_Indices[index] = { { index * 4, index * 4 + 1, index * 4 + 2 }, { index * 4 + 2, index * 4 + 3, index * 4 } };
			m_Indices[index + 1] = { { index * 4 + 4, index * 4 + 5, index * 4 + 6 }, { index * 4 + 6, index * 4 + 7, index * 4 + 4 } };

			for (int k = 0; k < 4; k++) {
				m_Vertices[vertex + k] =
				{
					{ vertexDirs[k].x * Cell::OUTER / 2, vertexDirs[k].y * Cell::OUTER / 2 },
					{ 0.2f, 0.3f, 0.8f, 1.0f },
					{ m_Cells[i][j].Position.x, m_Cells[i][j].Position.y }
				};
				m_Vertices[vertex + 4 + k] =
				{
					{ vertexDirs[k].x * Cell::INNER / 2, vertexDirs[k].y * Cell::INNER / 2 },
					{ 0.1f, 0.1f, 0.1f, 1.0f },
					{ m_Cells[i][j].Position.x, m_Cells[i][j].Position.y }
				};
			}
		}

	m_IndexBuffer = std::make_unique<IndexBuffer>((unsigned int*)m_Indices, indexCnt, GL_STATIC_DRAW);
	m_VertexBuffer = std::make_unique<VertexBuffer>(m_Vertices, vertexCnt * sizeof(Vertice), GL_STREAM_DRAW);

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(4);
	layout.Push<float>(2);
	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

	m_Shader = std::make_unique<Shader>("res/shaders/Cell.shader");
	glm::mat4 mvp = Camera::Current().GetProj() * Camera::Current().GetView() * glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_MVP", mvp);
}

void Map::Update(float deltaTime)
{
	if (m_End) return;

	const static float HALF = 0.5f;
	static float timer = 0.0f;
	m_Timer += deltaTime;
	timer += deltaTime;
	if (timer > HALF * 2) timer -= HALF * 2;
	for (int i = 0, vertex = 0; i < COLUMN_COUNT; i++)
		for (int j = 0; j < ROW_COUNT; j++, vertex += 8)
			for (int k = 0; k < 4; k++) {
				m_Vertices[vertex + k].color.r = lerp(0.2f, 0.8f, curve((timer > HALF ? HALF * 2 - timer : timer) / HALF));
				Vec4 color = m_Cells[i][j].Item ? ((Block*)m_Cells[i][j].Item)->GetColor() : Vec4{ 0.1f, 0.1f, 0.1f, 1.0f };
				m_Vertices[vertex + 4 + k].color = { color.r, color.g, color.b, color.a };
			}
}

void Map::Render()
{
	m_VertexBuffer->UpdateData(m_Vertices, COLUMN_COUNT * ROW_COUNT * 2 * 4 * sizeof(Vertice));
	Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
}

void Map::OnGuiLeft()
{
	ImGui::SetWindowFontScale(2.0f);
	ImGui::TextColored({ 0.8f, 0.3f, 0.8f, 1.0f }, "You've cleared %d lines!", m_LineCnt);
	ImGui::TextColored({ 0.2f, 0.3f, 0.8f, 1.0f }, "It's passed %.3f seconds...", m_Timer);
	ImGui::SetWindowFontScale(3.0f);
	if (m_End) ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "END");
	ImGui::SetWindowFontScale(1.5f);
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

void Map::Refresh()
{
	for (int i = 0, stride = 0; i < ROW_COUNT; i++)
		if (Row(i, [](const Cell& cell) { return cell.Item != nullptr; })) {
			stride++, m_LineCnt++;
			SetRow(i, [this](Cell& cell)
				{
					Object* temp = cell.Item;
					cell.Item = nullptr;
					if (All([temp](const Cell& another) { return another.Item != temp; }))temp->Destroy();
				});
		}
		else for (int j = 0; j < COLUMN_COUNT; j++) m_Cells[j][i - stride].Item = m_Cells[j][i].Item;
}
