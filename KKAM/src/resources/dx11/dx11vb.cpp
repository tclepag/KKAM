#include "resources/dx11/dx11vb.h"

namespace resources {
	namespace dx11 {
		void DX11VertexBuffer::initialize(Context11* context) {
			// Get D3D11Device from D3D11DeviceContext
			Device11* device;
			context->GetDevice(&device);
			// Create vertex buffer description
			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(float) * m_vertices.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA initData = {};
			initData.pSysMem = m_vertices.data();
			// Create vertex buffer
			device->CreateBuffer(&bd, &initData, m_buffer.GetAddressOf());
		}
		void DX11VertexBuffer::bind(Context11* context) {
			UINT stride = sizeof(float);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, m_buffer.GetAddressOf(), &stride, &offset);
		}
		void DX11VertexBuffer::unbind(Context11* context) {
			context->IASetVertexBuffers(0, 1, nullptr, nullptr, nullptr);
		}
		void DX11VertexBuffer::setVertices(const std::vector<float>& vertices) {
			this->m_vertices = vertices;
		}
	}
}