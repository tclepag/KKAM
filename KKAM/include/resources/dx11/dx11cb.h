#pragma once

#include "common/directx11.h"
#include "resources/buffer.h"

namespace resources {
    namespace dx11 {
        class DX11ConstantBuffer : public Buffer<Context11> {
        public:
            DX11ConstantBuffer(size_t size) : m_size(size) {}
            virtual ~DX11ConstantBuffer() = default;

            // Initialize the constant buffer
            void initialize(Context11* context) override;

            // Bind the constant buffer to the pipeline
            void bind(Context11* context) override;

            // Unbind the constant buffer from the pipeline
            void unbind(Context11* context) override;

            // Update the constant buffer with new data
            void update(Context11* context, void* data);
        private:
            ComPtr<Buffer11> m_buffer;
            size_t m_size = 0;
        };
    }
}
