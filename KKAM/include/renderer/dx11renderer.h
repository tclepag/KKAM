#pragma once

#include "common/common.h"
#include "common/directx11.h"
#include "renderer.h"

#include "viewports/dx11viewport.h"

namespace renderer {
	using namespace viewports;
	class DX11Renderer : public Renderer {
	public:
		DX11Renderer();
		~DX11Renderer();
		void init() override;
		void beginFrame() override;
		void endFrame() override;
		void shutdown() override;
		void rebuild() override;
		void resize() override;

		ComPtr<Device11> getDevice() const;
		ComPtr<Context11> getContext() const;
		ComPtr<SwapChain11> getSwapChain() const;
		ComPtr<RenderTargetView11> getRenderTargetView() const;
		SharedPtr<DX11Viewport> getViewport() const { return m_viewport; }
	private:
		ComPtr<Device11> m_device;
		ComPtr<Context11> m_context;
		ComPtr<SwapChain11> m_swapChain;
		ComPtr<RenderTargetView11> m_renderTargetView;
		SharedPtr<DX11Viewport> m_viewport;

		UINT m_backBufferWidth;
		UINT m_backBufferHeight;

		void createDeviceAndSwapChain();
		void createRenderTargetView();
	};
}