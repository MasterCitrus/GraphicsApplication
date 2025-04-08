#pragma once

#include "Vertex.h"
#include <vector>
#include <string>

enum class ShaderDataType
{
	None = 0,
	Float,
	Vec2,
	Vec3,
	Vec4,
	Mat3,
	Mat4,
	Int,
	IVec2,
	IVec3,
	IVec4,
	Bool
};

struct BufferElement
{
	std::string name;
	ShaderDataType type;
	unsigned int offset;
	unsigned int size;
	bool normalised;

	BufferElement() = default;
	BufferElement(ShaderDataType type, const std::string& name, bool normalised = false);

	unsigned int GetComponentCount() const;
};

class BufferLayout
{
public:
	BufferLayout() = default;
	BufferLayout(const std::initializer_list<BufferElement>& elements);

	const unsigned int GetStride() const { return stride; }
	const std::vector<BufferElement>& GetElements() const { return elements; }

	std::vector<BufferElement>::iterator begin() { return elements.begin(); }
	std::vector<BufferElement>::iterator end() { return elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

private:
	void CalculateOffsetsAndStride();

private:
	std::vector<BufferElement> elements;
	unsigned int stride = 0;
};

class VertexBuffer
{
public:
	VertexBuffer(Vertex* vertices, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void SetLayout(const BufferLayout& layout) { this->layout = layout; }
	const BufferLayout& GetLayout() const { return layout; }

private:
	unsigned int bufferID;
	BufferLayout layout;
};

class IndexBuffer
{
public:
	IndexBuffer(unsigned int* indices, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const { return count; }

private:
	unsigned int bufferID;
	unsigned int count;
};