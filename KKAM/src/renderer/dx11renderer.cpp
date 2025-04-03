#include "renderer/dx11renderer.h"

namespace renderer {
DX11Renderer::DX11Renderer() {
}
DX11Renderer::~DX11Renderer() {
	shutdown();
}
void DX11Renderer::init() {
	m_viewport = std::make_shared<DX11Viewport>();
	createDeviceAndSwapChain();
	m_viewport->init(m_context.Get(), m_swapChain.Get(), 800, 600);
	createRenderTargetView();

	createBlendState();
	createRasterizerState();
}
void DX11Renderer::beginFrame() {
	m_viewport->bind(m_context.Get());
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
	m_viewport.reset();
}
   void DX11Renderer::rebuild() {
	OutputDebugString(L"Rebuilding\n");
       // Release existing resources
       if (m_context) {
           m_context->ClearState();
       }
       if (m_renderTargetView) {
           m_renderTargetView.Reset();
       }
       if (m_swapChain) {
           m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
       }

       // Recreate device and swap chain
       createDeviceAndSwapChain();

       // Recreate render target view
       createRenderTargetView();

	  m_viewport->createDepthStencil(m_context.Get(), m_swapChain.Get(), m_backBufferWidth, m_backBufferHeight);

	// Render again
	beginFrame();
	endFrame();
   }
void DX11Renderer::resize() {
	// Release existing resources
	if (m_context) {
		m_context->ClearState();
	}
	if (m_renderTargetView) {
		m_renderTargetView.Reset();
	}

	m_viewport->releaseResources();

	if (m_swapChain) {
		HRESULT hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (FAILED(hr)) {
			throw std::runtime_error("Failed to resize swap chain buffers");
		}
	}

	// Recreate render target view
	createRenderTargetView();

	// Recreate depth stencil view if necessary
	m_viewport->createRenderTarget(m_context.Get(), m_swapChain.Get());
	m_viewport->createDepthStencil(m_context.Get(), m_swapChain.Get(), m_backBufferWidth, m_backBufferHeight);

	m_viewport->updateViewportDimensions(m_backBufferWidth, m_backBufferHeight);
	m_viewport->bind(m_context.Get());

	if (m_rasterizerState) {
		m_context->RSSetState(m_rasterizerState.Get());
	}

	if (m_blendState) {
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_context->OMSetBlendState(m_blendState.Get(), blendFactor, 0xffffffff);
	}

	// Render again
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

	// Store the dimensions of the back buffer
	m_backBufferWidth = backBufferDesc.Width;
	m_backBufferHeight = backBufferDesc.Height;
}
void DX11Renderer::createBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;    // Source blend factor
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;  // Destination blend factor
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;  // Blend operation

	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;    // Source alpha blend factor
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;  // Destination alpha blend factor
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;  // Alpha blend operation

	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;  // Allow writing to all channels

	// Create and set the blend state
	ID3D11BlendState* blendState = nullptr;
	HRESULT hr = m_device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr))
	{
		// Handle error
		return;
	}

	// Apply the blend state (the float array is blend factor, typically all zeros for default)
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_context->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	// Store for later cleanup
	m_blendState = blendState;
}
void DX11Renderer::createRasterizerState()
{
	// Add rasterizer state setup here
	D3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.CullMode = D3D11_CULL_NONE;  // Enable back-face culling
	rastDesc.FrontCounterClockwise = TRUE;  // Clockwise vertices are front-facing
	rastDesc.DepthClipEnable = TRUE;

	// Create and set the rasterizer state
	ID3D11RasterizerState* rasterizerState = nullptr;
	HRESULT hr = m_device->CreateRasterizerState(&rastDesc, &rasterizerState);
	if (FAILED(hr))
	{
		return;
	}

	m_context->RSSetState(rasterizerState);

	// Store for later use and cleanup
	m_rasterizerState = rasterizerState;
}
}