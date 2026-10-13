#pragma once
#include <memory>
#include <IVertexBuffer.hpp>

namespace VoidCore::RHI
{
    class IVertexArray
    {
    public:
        virtual ~IVertexArray() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void AddVertexBuffer(const std::shared_ptr<IVertexBuffer> &vertexBuffer, const BufferLayout &layout) = 0;

        // Добавляем привязку EBO
        virtual void SetIndexBuffer(const std::shared_ptr<IIndexBuffer> &indexBuffer) = 0;
        virtual std::shared_ptr<IIndexBuffer> GetIndexBuffer() const = 0;
    };
}