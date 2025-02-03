#pragma once

#include <vector>

struct VertexBufferElement
{
	unsigned int type, count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout();

	template<typename T>
	void Push(unsigned int count);
	template<>
	void Push<float>(unsigned int count);
	template<>
	void Push<unsigned int>(unsigned int count);
	template<>
	void Push<unsigned char>(unsigned int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};