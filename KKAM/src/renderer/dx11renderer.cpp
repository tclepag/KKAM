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
	}
	void DX11Renderer::render() {
		const float clearColor[4] = { 0.392f, 0.584f, 0.929f, 1.0f };

		if (m_viewports.empty()) {
			return;
		}
		for (auto& viewport : m_viewports) {
			viewport.second->bind(m_context.Get());
			m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);
			m_swapChain->Present(0, 0);
		}
	}
	void DX11Renderer::shutdown() {
		m_context->ClearState();
		m_context->Flush();
		m_context.Reset();
		m_swapChain.Reset();
		m_renderTargetView.Reset();
		m_device.Reset();
		m_viewports.clear();
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

		// Recreate depth stencil view if necessary
		for (auto& viewport : m_viewports) {
			viewport.second->createDepthStencil(m_context.Get(), m_swapChain.Get());
		}

		// Render again
		render();
    }
	void DX11Renderer::resize() {
		// Release existing resources
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

		// Recreate render target view
		createRenderTargetView();

		// Recreate depth stencil view if necessary
		for (auto& viewport : m_viewports) {
			viewport.second->createDepthStencil(m_context.Get(), m_swapChain.Get());
		}

		// Rebind viewports
		for (auto& viewport : m_viewports) {
			viewport.second->bind(m_context.Get());
		}

		// Render again
		render();
	}
    DX11Viewport* DX11Renderer::createViewport(const String& name) {
        auto viewport = std::make_shared<DX11Viewport>();
		viewport->init(m_context.Get(), m_swapChain.Get());
        m_viewports[name] = viewport;
        return viewport.get();
    }

    DX11Viewport* DX11Renderer::getViewport(const String& name) {
        auto it = m_viewports.find(name);
        if (it != m_viewports.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    DX11Viewport* DX11Renderer::removeViewport(const String& name) {
        auto it = m_viewports.find(name);
        if (it != m_viewports.end()) {
            auto viewport = it->second;
            m_viewports.erase(it);
            return viewport.get();
        }
        return nullptr;
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
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
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
		m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.GetAddressOf());
	}
}