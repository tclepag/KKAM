#include "resources/dx11/dx11ib.h"

namespace resources {
	namespace dx11 {
		void DX11IndexBuffer::initialize(Context11* context) {
			// Get D3D11Device from D3D11DeviceContext
			Device11* device;
			context->GetDevice(&device);

			// Create index buffer description
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(uint32_t) * m_indices.size();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = m_indices.data();
			// Create index buffer
			device->CreateBuffer(&bd, &initData, m_buffer.GetAddressOf());
		}
		void DX11IndexBuffer::bind(Context11* context) {
			UINT stride = sizeof(uint32_t);
			UINT offset = 0;
			context->IASetIndexBuffer(m_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);
		}
		void DX11IndexBuffer::unbind(Context11* context) {
			context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
		}
		void DX11IndexBuffer::setIndices(const std::vector<uint32_t>& indices) {
			this->m_indices = indices;
		}
	}
}