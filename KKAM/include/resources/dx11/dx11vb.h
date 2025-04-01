#pragma once

#include "common/common.h"
#include "resources/buffer.h"
#include "common/directx11.h"

namespace resources {
	namespace dx11 {
		class DX11VertexBuffer : public Buffer<Context11> {
		public:
			virtual ~DX11VertexBuffer() = default;
			void initialize(Context11* context) override;
			void bind(Context11* context) override;
			void unbind(Context11* context) override;
			void setVertices(const std::vector<float>& vertices);
		private:
			ComPtr<Buffer11> m_buffer;
			std::vector<float> m_vertices;
		};
	}
}