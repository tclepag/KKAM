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

		DX11Viewport* createViewport(const String& name);
		DX11Viewport* getViewport(const String& name);
		DX11Viewport* removeViewport(const String& name);

		ComPtr<Device11> getDevice() const;
		ComPtr<Context11> getContext() const;
		ComPtr<SwapChain11> getSwapChain() const;
		ComPtr<RenderTargetView11> getRenderTargetView() const;
	private:
		ComPtr<Device11> m_device;
		ComPtr<Context11> m_context;
		ComPtr<SwapChain11> m_swapChain;
		ComPtr<RenderTargetView11> m_renderTargetView;
		Map<String, SharedPtr<DX11Viewport>> m_viewports;

		UINT m_backBufferWidth;
		UINT m_backBufferHeight;

		void createDeviceAndSwapChain();
		void createRenderTargetView();
	};
}