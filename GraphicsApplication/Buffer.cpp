#include "Buffer.h"
#include <cassert>
#include <glad/glad.h>

static unsigned int ShaderDataTypeSize(ShaderDataType type)
{
	switch( type )
	{
	case ShaderDataType::Float:
		return 4;
	case ShaderDataType::Vec2:
		return 4 * 2;
	case ShaderDataType::Vec3:
		return 4 * 3;
	case ShaderDataType::Vec4:
		return 4 * 4;
	case ShaderDataType::Mat3:
		return 4 * 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4 * 4;
	case ShaderDataType::Int:
		return 4;
	case ShaderDataType::IVec2:
		return 4 * 2;
	case ShaderDataType::IVec3:
		return 4 * 3;
	case ShaderDataType::IVec4:
		return 4 * 4;
	case ShaderDataType::Bool:
		return 1;
	default:
		return -1;
	}
}

BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalised)
	: name(name), type(type), offset(0), size(ShaderDataTypeSize(type)), normalised(normalised)
{

}

unsigned int BufferElement::GetComponentCount() const
{
	switch( type )
	{
	case ShaderDataType::Float:
		return 1;
	case ShaderDataType::Vec2:
		return 2;
	case ShaderDataType::Vec3:
		return 3;
	case ShaderDataType::Vec4:
		return 4;
	case ShaderDataType::Mat3:
		return 3 * 3;
	case ShaderDataType::Mat4:
		return 4 * 4;
	case ShaderDataType::Int:
		return 1;
	case ShaderDataType::IVec2:
		return 2;
	case ShaderDataType::IVec3:
		return 3;
	case ShaderDataType::IVec4:
		return 4;
	case ShaderDataType::Bool:
		return 1;
	}

	assert(false && "Unknown ShaderDataType");
	return 0;
}

BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
	: elements(elements)
{
	CalculateOffsetsAndStride();
}

void BufferLayout::CalculateOffsetsAndStride()
{
	unsigned int offset = 0;
	stride = 0;
	for( auto& element : elements )
	{
		element.offset = offset;
		offset += element.size;
		stride += element.size;
	}
}

VertexBuffer::VertexBuffer(Vertex* vertices, unsigned int size)
{
	glCreateBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &bufferID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count)
{
	glCreateBuffers(1, &bufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &bufferID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
