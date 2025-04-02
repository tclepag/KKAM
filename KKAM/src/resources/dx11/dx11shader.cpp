#include "resources/dx11/dx11shader.h"

namespace resources {
	DX11Shader::DX11Shader() {
	}
	DX11Shader::~DX11Shader() {
		releaseResources();
	}
	void DX11Shader::build() {
		createVertexShader();
		createPixelShader();
		createInputLayout();
	}
	void DX11Shader::bind() {
		m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
		m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_context->IASetInputLayout(m_inputLayout.Get());
		for (auto& buffer : m_buffers) {
			unsigned int i = 0;
			buffer.second->setSlot(i);
			buffer.second->bind(m_context);
			i++;
		}
	}
	void DX11Shader::unbind() {
		m_context->VSSetShader(nullptr, nullptr, 0);
		m_context->PSSetShader(nullptr, nullptr, 0);
		m_context->IASetInputLayout(nullptr);
		for (auto& buffer : m_buffers) {
			buffer.second->unbind(m_context);
		}
	}
	void DX11Shader::releaseResources() {
		m_vertexShader.Reset();
		m_pixelShader.Reset();
		m_inputLayout.Reset();
	}

	void DX11Shader::createVertexShader() {
		ComPtr<ID3DBlob> vertexShaderBlob = findShaderFile(m_vertexPath, "vs_5_0");
		if (!vertexShaderBlob) {
			OutputDebugString(L"Failed to find vertex shader file\n");
			return;
		}

		// Get device from device context
		ID3D11Device* device;
		m_context->GetDevice(&device);

		HRESULT hr = device->CreateVertexShader(
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			nullptr,
			m_vertexShader.GetAddressOf()
		);
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create vertex shader\n");
			return;
		}
	}

	void DX11Shader::createPixelShader() {
		ComPtr<ID3DBlob> pixelShaderBlob = findShaderFile(m_pixelPath, "ps_5_0");
		if (!pixelShaderBlob) {
			OutputDebugString(L"Failed to find pixel shader file\n");
			return;
		}

		// Get device from device context
		ID3D11Device* device;
		m_context->GetDevice(&device);

		HRESULT hr = device->CreatePixelShader(
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize(),
			nullptr,
			m_pixelShader.GetAddressOf()
		);
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create pixel shader\n");
			return;
		}
	}

	void DX11Shader::createInputLayout() {
		ComPtr<ID3DBlob> vertexShaderBlob = findShaderFile(m_vertexPath, "vs_5_0");
		if (!vertexShaderBlob) {
			OutputDebugString(L"Failed to find vertex shader file\n");
			return;
		}

		ID3D11Device* device;
		m_context->GetDevice(&device);
		HRESULT hr = device->CreateInputLayout(
			m_layout.data(),
			static_cast<UINT>(m_layout.size()),
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize(),
			m_inputLayout.GetAddressOf()
		);
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create input layout\n");
			return;
		}
	}

	ComPtr<ID3DBlob> DX11Shader::findShaderFile(const String& shaderPath, const char* profile) {
		ComPtr<ID3DBlob> shaderBlob;
		ComPtr<ID3DBlob> errorBlob;

		String fileName = shaderPath.substr(shaderPath.find_last_of(L"/\\") + 1);

		// Check if a .cso file exists
		if (std::filesystem::exists(fileName + L".cso")) {
			HRESULT hr = D3DReadFileToBlob((fileName + L".cso").c_str(), shaderBlob.GetAddressOf());
			if (FAILED(hr)) {
				OutputDebugString(L"Failed to read .cso file\n");
				return nullptr;
			}
			return shaderBlob;
		}
		// If we fail to find a cso file, just compile our own.
		else {
			HRESULT hr = D3DCompileFromFile(
				shaderPath.c_str(),
				nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"main",
				profile,
				D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
				0,
				shaderBlob.GetAddressOf(),
				errorBlob.GetAddressOf()
			);
			if (FAILED(hr)) {
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				return nullptr;
			}
		}
		return shaderBlob;
	}
}