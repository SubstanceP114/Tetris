#pragma once
class VertexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Usage;
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void UpdateData(const void* data, unsigned int size);
};

