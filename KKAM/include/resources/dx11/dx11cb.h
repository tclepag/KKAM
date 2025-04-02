#pragma once

#include "common/directx11.h"
#include "resources/buffer.h"

namespace resources {
    namespace dx11 {
        template<typename T>
        class DX11ConstantBuffer : public Buffer<Context11> {
        public:
            virtual ~DX11ConstantBuffer() = default;

            // Initialize the constant buffer
            virtual bool initialize(Context11* context, size_t size, const T* initialData = nullptr) = 0;

            // Bind the constant buffer to the pipeline
            virtual void bind(Context11* context) = 0;

            // Unbind the constant buffer from the pipeline
            virtual void unbind(Context11* context) = 0;

            // Update the constant buffer with new data
            virtual void update(Context11* context, const T* data, size_t size) = 0;
        private:
            ComPtr<Buffer11> m_buffer;
        };
    }
}
