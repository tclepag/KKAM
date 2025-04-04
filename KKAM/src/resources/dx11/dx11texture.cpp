#include "resources/dx11/dx11texture.h"
#include <directxtk/WICTextureLoader.h>
#include <filesystem>
#include <iostream>

namespace resources {
	DX11Texture::DX11Texture() : m_texture(nullptr), m_textureView(nullptr), m_samplerState(nullptr) {
	}

	DX11Texture::~DX11Texture() {
		releaseResources();
	}

	void DX11Texture::loadFromFile() {
		// Get Device
		Device11* device;
		m_context->GetDevice(&device);

		HRESULT hr = DirectX::CreateWICTextureFromFile(
			device,
			m_filepath.c_str(),
			(ID3D11Resource**)&m_texture,
			&m_textureView
		);
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to load texture from file\n");
			return;
		}
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = device->CreateSamplerState(&samplerDesc, &m_samplerState);
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create sampler state\n");
			return;
		}
	}

	void DX11Texture::bind() {
		m_context->PSSetShaderResources(0, 1, &m_textureView);
		m_context->PSSetSamplers(0, 1, &m_samplerState);
	}

	void DX11Texture::unbind() {
		ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
		m_context->PSSetShaderResources(0, 1, nullSRV);
		ID3D11SamplerState* nullSampler[1] = { nullptr };
		m_context->PSSetSamplers(0, 1, nullSampler);
	}

	void DX11Texture::releaseResources() {
		if (m_samplerState) {
			m_samplerState->Release();
			m_samplerState = nullptr;
		}
		if (m_textureView) {
			m_textureView->Release();
			m_textureView = nullptr;
		}
		if (m_texture) {
			m_texture->Release();
			m_texture = nullptr;
		}
	}
}
