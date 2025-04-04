#include "renderer/dx11renderer.h"

namespace renderer {
	DX11Renderer::DX11Renderer() {
	}
	DX11Renderer::~DX11Renderer() {
		shutdown();
	}
	void DX11Renderer::init() {
		createDeviceAndSwapChain();
		createRenderTargetView();
		createDepthStencil(800, 600);
		createBlendState();
		createRasterizerState();
	}
	void DX11Renderer::beginFrame() {
		m_context->RSSetViewports(1, &m_viewport);
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		clearRenderTargetAndDepthStencil();
	}
	void DX11Renderer::endFrame() {
		m_swapChain->Present(0, 0);
	}
	void DX11Renderer::shutdown() {
		m_context->ClearState();
		m_context->Flush();
		m_context.Reset();
		m_swapChain.Reset();
		m_renderTargetView.Reset();
		m_device.Reset();
		m_depthStencilView.Reset();
		m_depthStencilBuffer.Reset();
	}
	void DX11Renderer::rebuild() {
		OutputDebugString(L"Rebuilding\n");
		if (m_context) {
			m_context->ClearState();
		}
		if (m_renderTargetView) {
			m_renderTargetView.Reset();
		}
		if (m_swapChain) {
			m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		}
		createDeviceAndSwapChain();
		createRenderTargetView();
		createDepthStencil(m_backBufferWidth, m_backBufferHeight);
		beginFrame();
		endFrame();
	}
	void DX11Renderer::resize() {
		if (m_context) {
			m_context->ClearState();
		}
		if (m_renderTargetView) {
			m_renderTargetView.Reset();
		}
		if (m_swapChain) {
			HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			if (FAILED(hr)) {
				throw std::runtime_error("Failed to resize swap chain buffers");
			}
		}
		createRenderTargetView();
		createDepthStencil(m_backBufferWidth, m_backBufferHeight);
		m_context->RSSetViewports(1, &m_viewport);
		m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
		if (m_rasterizerState) {
			m_context->RSSetState(m_rasterizerState.Get());
		}
		if (m_blendState) {
			float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
			m_context->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
		}
		beginFrame();
		endFrame();
	}
	ComPtr<Device11> DX11Renderer::getDevice() const {
		return m_device;
	}
	ComPtr<Context11> DX11Renderer::getContext() const {
		return m_context;
	}
	ComPtr<IDXGISwapChain> DX11Renderer::getSwapChain() const {
		return m_swapChain;
	}
	ComPtr<ID3D11RenderTargetView> DX11Renderer::getRenderTargetView() const {
		return m_renderTargetView;
	}
	void DX11Renderer::createDeviceAndSwapChain() {
		DXGI_SWAP_CHAIN_DESC scd = {};
		scd.BufferCount = 1;
		scd.BufferDesc.Width = 800;
		scd.BufferDesc.Height = 600;
		scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.OutputWindow = GetActiveWindow();
		scd.SampleDesc.Count = 1;
		scd.Windowed = TRUE;
		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
		D3D_FEATURE_LEVEL featureLevel;
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			featureLevels,
			1,
			D3D11_SDK_VERSION,
			&scd,
			&m_swapChain,
			&m_device,
			&featureLevel,
			&m_context
		);
	}
	void DX11Renderer::createRenderTargetView() {
		m_renderTargetView.Reset();
		ComPtr<ID3D11Texture2D> backBuffer;
		HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to get back buffer\n");
			throw std::runtime_error("Failed to get back buffer");
		}
		D3D11_TEXTURE2D_DESC backBufferDesc;
		backBuffer->GetDesc(&backBufferDesc);
		hr = m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
		if (FAILED(hr)) {
			OutputDebugString(L"Failed to create render target view\n");
			throw std::runtime_error("Failed to create render target view");
		}
		m_backBufferWidth = backBufferDesc.Width;
		m_backBufferHeight = backBufferDesc.Height;
		m_viewport.Width = static_cast<float>(m_backBufferWidth);
		m_viewport.Height = static_cast<float>(m_backBufferHeight);
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;
	}
	void DX11Renderer::createDepthStencil(UINT width, UINT height) {
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
		m_device->CreateTexture2D(&descDepth, nullptr, m_depthStencilBuffer.GetAddressOf());
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), &descDSV, m_depthStencilView.GetAddressOf());
	}
	void DX11Renderer::createBlendState() {
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* blendState = nullptr;
		HRESULT hr = m_device->CreateBlendState(&blendDesc, &blendState);
		if (FAILED(hr)) {
			return;
		}
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_context->OMSetBlendState(blendState, blendFactor, 0xffffffff);
		m_blendState = blendState;
	}
	void DX11Renderer::createRasterizerState() {
		D3D11_RASTERIZER_DESC rastDesc = {};
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = TRUE;
		rastDesc.DepthClipEnable = TRUE;
		ID3D11RasterizerState* rasterizerState = nullptr;
		HRESULT hr = m_device->CreateRasterizerState(&rastDesc, &rasterizerState);
		if (FAILED(hr)) {
			return;
		}
		m_context->RSSetState(rasterizerState);
		m_rasterizerState = rasterizerState;
	}
	void DX11Renderer::clearRenderTargetAndDepthStencil() {
		const float clearColor[4] = { m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w };
		m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
		m_context->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}