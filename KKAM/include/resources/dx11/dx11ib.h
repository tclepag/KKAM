#pragma once

#include "common/common.h"
#include "resources/buffer.h"
#include "common/directx11.h"

namespace resources {
    namespace dx11 {
        class DX11IndexBuffer : public Buffer<Context11> {
        public:
            virtual ~DX11IndexBuffer() = default;

            void initialize(Context11* context) override;
            void bind(Context11* context) override;
            void unbind(Context11* context) override;

            void setIndices(const std::vector<uint32_t>& indices);
        private:
			ComPtr<Buffer11> m_buffer;
			std::vector<uint32_t> m_indices;
        };
    }
}
