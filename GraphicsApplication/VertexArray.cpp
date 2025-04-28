#include "VertexArray.h"
#include <glad/glad.h>

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch( type )
	{
	case ShaderDataType::Float:
	case ShaderDataType::Vec2:
	case ShaderDataType::Vec3:
	case ShaderDataType::Vec4:
	case ShaderDataType::Mat3:
	case ShaderDataType::Mat4:
		return GL_FLOAT;
	case ShaderDataType::Int:
	case ShaderDataType::IVec2:
	case ShaderDataType::IVec3:
	case ShaderDataType::IVec4:
		return GL_INT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}

	assert(false && "Unknown ShaderDataType!");
	return 0;

}

VertexArray::VertexArray()
{
	glCreateVertexArrays(1, &vertexArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vertexArrayID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(vertexArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
	if( vertexBuffer->GetLayout().GetElements().size() == 0 ) return;

	glBindVertexArray(vertexArrayID);
	vertexBuffer->Bind();

	unsigned int index = 0;
	const auto& layout = vertexBuffer->GetLayout();
	for( const auto& element : layout )
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.type),
			element.normalised ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.offset);
		index++;
	}

	vertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
	glBindVertexArray(vertexArrayID);
	indexBuffer->Bind();

	this->indexBuffer = indexBuffer;
}
