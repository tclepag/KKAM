#include "resources/dx11/dx11cb.h"

namespace resources {
    namespace dx11 {

        template<typename T>
        bool DX11ConstantBuffer<T>::initialize(Context11* context, size_t size, const T* initialData) {
			// Get D3D11Device from D3D11DeviceContext
			Device11* device;
			context->GetDevice(&device);

            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.ByteWidth = static_cast<UINT>(size);
            bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            bufferDesc.CPUAccessFlags = 0;

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = initialData;

            HRESULT hr = device->CreateBuffer(&bufferDesc, initialData ? &initData : nullptr, m_buffer.GetAddressOf());
            return SUCCEEDED(hr);
        }

        template<typename T>
        void DX11ConstantBuffer<T>::bind(Context11* context) {
            context->VSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
            context->PSSetConstantBuffers(m_slot, 1, m_buffer.GetAddressOf());
        }

        template<typename T>
        void DX11ConstantBuffer<T>::unbind(Context11* context) {
            ID3D11Buffer* nullBuffer = nullptr;
            context->VSSetConstantBuffers(m_slot, 1, &nullBuffer);
            context->PSSetConstantBuffers(m_slot, 1, &nullBuffer);
        }

        template<typename T>
        void DX11ConstantBuffer<T>::update(Context11* context, const T* data, size_t size) {
            context->UpdateSubresource(m_buffer.Get(), 0, nullptr, data, 0, 0);
        }

    }
}