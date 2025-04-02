#include "resources/dx11/dx11cb.h"

namespace resources {
    namespace dx11 {
        void DX11ConstantBuffer::initialize(Context11* context) {
			// Get D3D11Device from D3D11DeviceContext
			Device11* device;
			context->GetDevice(&device);

            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.ByteWidth = static_cast<UINT>(m_size);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = nullptr;

            HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, m_buffer.GetAddressOf());
        }

        void DX11ConstantBuffer::bind(Context11* context) {
            context->VSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
            context->PSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
        }

        void DX11ConstantBuffer::unbind(Context11* context) {
            ID3D11Buffer* nullBuffer = nullptr;
            context->VSSetConstantBuffers(m_slot, 1, &nullBuffer);
            context->PSSetConstantBuffers(m_slot, 1, &nullBuffer);
        }

        void DX11ConstantBuffer::update(Context11* context, void* data) {
            context->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
        }

    }
}