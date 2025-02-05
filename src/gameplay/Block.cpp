#include "Block.h"

#include "Scene.h"
#include "Camera.h"

#include "imgui/imgui.h"

#include <random>

#define elif(key, func) \
else if (!pressed && glfwGetKey(Scene::Current().GetWindow(), (key)) == GLFW_PRESS) { pressed = true; current = (key); (func); }\
else if (pressed && current == (key) && glfwGetKey(Scene::Current().GetWindow(), (key)) == GLFW_RELEASE) pressed = false

#define index_block(i) (i) * 4, (i) * 4 + 1, (i) * 4 + 2, (i) * 4 + 2, (i) * 4 + 3, (i) * 4
#define index_cell(i) index_block((i) * 2), index_block((i) * 2 + 1)
#define INDEX_PREVIEW index_cell(0), index_cell(1), index_cell(2), index_cell(3)

const float OUTER = 25.0f, INNER = 24.0f;
#define VERTEX_COLOR_OUTER 1.0f - m_Color.r, 1.0f - m_Color.g, 1.0f - m_Color.b, 1.0f - m_Color.a
#define VERTEX_COLOR_INNER m_Color.r, m_Color.g, m_Color.b, m_Color.a
#define vertex_offset(i) m_Offsets[i].x * 2 * OUTER, m_Offsets[i].y * 2 * OUTER
#define vertex_cell(i)\
-OUTER, OUTER, VERTEX_COLOR_OUTER, vertex_offset(i), \
-OUTER,-OUTER, VERTEX_COLOR_OUTER, vertex_offset(i), \
 OUTER,-OUTER, VERTEX_COLOR_OUTER, vertex_offset(i), \
 OUTER, OUTER, VERTEX_COLOR_OUTER, vertex_offset(i), \
-INNER, INNER, VERTEX_COLOR_INNER, vertex_offset(i), \
-INNER,-INNER, VERTEX_COLOR_INNER, vertex_offset(i), \
 INNER,-INNER, VERTEX_COLOR_INNER, vertex_offset(i), \
 INNER, INNER, VERTEX_COLOR_INNER, vertex_offset(i)
#define VERTEX_PREVIEW vertex_cell(0), vertex_cell(1), vertex_cell(2), vertex_cell(3)

std::unique_ptr<VertexArray> Block::m_VertexArray = nullptr;
std::unique_ptr<VertexBuffer> Block::m_VertexBuffer = nullptr;
std::unique_ptr<IndexBuffer> Block::m_IndexBuffer = nullptr;
std::unique_ptr<Shader> Block::m_Shader = nullptr;

Block* Block::m_Current = nullptr;
Block* Block::m_Preview = nullptr;

void Block::Rotate(bool anti = true)
{
	static auto rotate = [](bool anti, Vec2 offsets[4]) {
		for (int i = 1; i < 4; i++) {
			Vec2 temp = offsets[i];
			offsets[i] = { temp.y * (anti ? -1 : 1), temp.x * (anti ? 1 : -1) };
		}};
	ForEach([](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, nullptr); });
	rotate(anti, m_Offsets);
	if (All([](Vec2 cell) { return Map::Current()->IsValid(cell.x, cell.y) && Map::Current()->IsEmpty(cell.x, cell.y); }))
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	else {
		rotate(!anti, m_Offsets);
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	}
}

void Block::Move(Vec2 dir)
{
	ForEach([](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, nullptr); });
	m_Center += dir;
	if (All([](Vec2 cell) { return Map::Current()->IsValid(cell.x, cell.y) && Map::Current()->IsEmpty(cell.x, cell.y); }))
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	else {
		m_Center += -dir;
		ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
		if (dir.x == 0 && dir.y == -1) Switch();
	}
}

void Block::Switch()
{
	Map::Current()->Refresh();
	m_Current = m_Preview;
	if (m_Current->All([](Vec2 cell) { return Map::Current()->IsValid(cell.x, cell.y) && Map::Current()->IsEmpty(cell.x, cell.y); })) {
		m_Preview = Random();
		Scene::Current().Add(m_Preview);
	}
	else {
		m_Current = nullptr;
		Map::Current()->End();
	}
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
	float vertices[] = { VERTEX_PREVIEW };
	if (m_Current == nullptr) {
		m_Current = this;
		struct {
			struct { float x, y; } position;
			struct { float r, g, b, a; } color;
			struct { float x, y; } offset;
		} vertices[4 * 2 * 4];
		m_VertexBuffer = std::make_unique<VertexBuffer>(vertices, sizeof(vertices), GL_DYNAMIC_DRAW);

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(2);
		m_VertexArray = std::make_unique<VertexArray>();
		m_VertexArray->AddBuffer(*m_VertexBuffer, layout);

		unsigned int indices[]{ INDEX_PREVIEW };
		m_IndexBuffer = std::make_unique<IndexBuffer>(&indices[0], 4 * 2 * 6);

		auto mvp = Camera::Right().GetProj() * Camera::Right().GetView() *
			glm::translate(glm::mat4(1.0f), glm::vec3(Map::Current()->ORIGIN.X / 2, Map::Current()->SIZE.HEIGHT - 100.0f, 0.0f));
		m_Shader = std::make_unique<Shader>("res/shaders/Cell.shader");
		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", mvp);
	}
	else if (m_Preview == nullptr)  m_Preview = this;
	m_VertexBuffer->UpdateData(&vertices[0], sizeof(vertices));
}

void Block::Update(float deltaTime)
{
	if (m_Current != this) return;

	ForEach([this](Vec2 cell) { Map::Current()->SetCell(cell.x, cell.y, this); });
	static bool pressed = false;
	static unsigned int current = 0;
	static float timer = 0.0f;
	const static float INTERVAL = 60.0f / 170.0f * 2;
	timer += deltaTime;
	if (timer > INTERVAL) {
		timer = 0.0f;
		Move({ 0,-1 });
	}
	elif(GLFW_KEY_A, Move({ -1, 0 }));
	elif(GLFW_KEY_D, Move({ 1, 0 }));
	elif(GLFW_KEY_Q, Rotate(false));
	elif(GLFW_KEY_E, Rotate(true));
	else if (glfwGetKey(Scene::Current().GetWindow(), GLFW_KEY_S) == GLFW_PRESS) Move({ 0,-1 });
}

void Block::Render()
{
	if (m_Preview != this) return;

	Renderer::Draw(*m_VertexArray, *m_IndexBuffer, *m_Shader);
}

void Block::OnGuiLeft()
{
}

void Block::OnGuiRight()
{
	if (m_Preview != this) return;

	ImGui::SetWindowFontScale(2.5f);
	ImGui::TextColored({ m_Color.r, m_Color.g, m_Color.b, m_Color.a }, "Next:");
	ImGui::SetWindowFontScale(1.5f);
}

Block* Block::Random()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_int_distribution<> color(55, 255);
	static std::uniform_int_distribution<> index(0, 6);
	static Block blocks[] = {
		{{ 1, 0 },{ 2, 0 },{ 3, 0 }},
		{{ 0,-1 },{-1, 0 },{ 1, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 2, 0 }},
		{{ 0,-1 },{-1, 0 },{-2, 0 }},
		{{ 0,-1 },{ 1, 0 },{ 1,-1 }},
		{{ 0,-1 },{-1, 0 },{ 1,-1 }},
		{{ 0,-1 },{ 1, 0 },{-1,-1 }}
	};
	return new Block(blocks[index(gen)], { (float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,(float)color(gen) / 255.0f ,1.0f });
}
