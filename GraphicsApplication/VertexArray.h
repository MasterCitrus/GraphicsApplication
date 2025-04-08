#pragma once
#include "Buffer.h"
#include <memory>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

	const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return vertexBuffers; }
	const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return indexBuffer; }

private:
	unsigned int vertexArrayID;
	std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
	std::shared_ptr<IndexBuffer> indexBuffer;
};