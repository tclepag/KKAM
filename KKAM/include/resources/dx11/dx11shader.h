#pragma once

#include "common/directx11.h"
#include "common/common.h"
#include "resources/shader.h"
#include "dx11cb.h"
#include <filesystem>

namespace resources {
	using namespace dx11;
	class DX11Shader : public Shader {
	public:
		DX11Shader();
		~DX11Shader();
		void build() override;
		void bind() override;
		void unbind() override;
		void releaseResources() override;
		void setVertexPath(const String& vertexPath) {
			m_vertexPath = vertexPath;
		}
		void setPixelPath(const String& pixelPath) {
			m_pixelPath = pixelPath;
		}
		void setContext(Context11* context) {
			m_context = context;
		}

		void setLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout) {
			m_layout = layout;
		}
		
		template<typename T>
		void createConstantBuffer(const String& name) {
			DX11ConstantBuffer<T>* constBuffer = new DX11ConstantBuffer<T>();
			constBuffer->initialize(m_context, sizeof(T));
			m_buffers[name] = constBuffer;
		}

		template<typename T>
		void updateConstantBuffer(const String& name, const T& data) {
			auto buffer = m_buffers[name];

			DX11ConstantBuffer<T>* constantBuffer = dynamic_cast<DX11ConstantBuffer<T>*>(buffer);
			if (constantBuffer) {
				constantBuffer->update(m_context, &data, sizeof(T));
			}
		}

		void removeConstantBuffer(const String& name) {
			m_buffers.erase(name);
		}
	private:
		void createVertexShader();
		void createPixelShader();
		void createInputLayout();
		static ComPtr<ID3DBlob> findShaderFile(const String& shaderPath, const char* profile);

		HRESULT CloneBlob(ID3DBlob* source, ID3DBlob** destination) {
			if (!source || !destination) return E_INVALIDARG;

			// Get the size of the source blob
			SIZE_T size = source->GetBufferSize();

			// Create a new blob with the same size
			HRESULT hr = D3DCreateBlob(size, destination);
			if (FAILED(hr)) return hr;

			// Copy the data from source to destination
			memcpy((*destination)->GetBufferPointer(), source->GetBufferPointer(), size);

			return S_OK;
		}
		
		String m_vertexPath;
		String m_pixelPath;

		Map<String, Buffer<Context11>*> m_buffers;

		std::vector<D3D11_INPUT_ELEMENT_DESC> m_layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		Context11* m_context;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11PixelShader> m_pixelShader;
		ComPtr<ID3D11VertexShader> m_fbVertexShader;
		ComPtr<ID3D11PixelShader> m_fbPixelShader;
		ComPtr<ID3D11InputLayout> m_inputLayout;
		ComPtr<ID3D11Buffer> m_constantBuffer;
	};
}