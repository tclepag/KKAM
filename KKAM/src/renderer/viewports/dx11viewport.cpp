#include "renderer/viewports/dx11viewport.h"

namespace renderer {
namespace viewports {
	void DX11Viewport::init(Context11* context, SwapChain11* swapChain, UINT height, UINT width) {
		createViewport(height, width);
		createRenderTarget(context, swapChain);
		createDepthStencil(context, swapChain, height, width);
	}
	void DX11Viewport::bind(Context11* context) {
		context->RSSetViewports(1, &m_viewport);
		context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	}
	void DX11Viewport::clear(Context11* context) {
		const float clearColor[4] = { m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3] };
		context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
	}
	void DX11Viewport::releaseResources()
	{
		m_depthStencilView.Reset();
		m_renderTargetView.Reset();
		m_depthStencilBuffer.Reset();
	}
	void DX11Viewport::createViewport(UINT width, UINT height) {
		m_viewport.Width = static_cast<float>(width);
		m_viewport.Height = static_cast<float>(height);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
	}
	void DX11Viewport::createRenderTarget(Context11* context, SwapChain11* swapChain) {
		// Get ID3D11Device from ID3D11DeviceContext
		Device11* device;
		context->GetDevice(&device);

		// Clear render target view if it exists
		m_renderTargetView.Reset();

		// Get the back buffer from the swap chain
		ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to get the back buffer from the swap chain\n");
			return;
		}

		// Create the render target view
		hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create the render target view\n");
			return;
		}
	}
	void DX11Viewport::createDepthStencil(Context11* context, SwapChain11* swapChain, UINT width, UINT height) {
		// Get ID3D11Device from ID3D11DeviceContext
		Device11* device;
		context->GetDevice(&device);

		// Clear depth stencil view and buffer if it exists
		m_depthStencilView.Reset();
		m_depthStencilBuffer.Reset();

		D3D11_TEXTURE2D_DESC descDepth = {};
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		device->CreateTexture2D(&descDepth, nullptr, m_depthStencilBuffer.GetAddressOf());
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &descDSV, m_depthStencilView.GetAddressOf());
	}
	void DX11Viewport::updateViewportDimensions(UINT width, UINT height) {
		m_viewport.Width = static_cast<float>(width);
		m_viewport.Height = static_cast<float>(height);
	}
}
}