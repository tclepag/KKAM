#pragma once

#include "common/common.h"
#include "common/directx11.h"
#include "renderer.h"

namespace renderer {
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
		void setViewMatrix(const Matrix4& view) override { m_viewMatrix = view; }
		void setProjectionMatrix(const Matrix4& projection) override { m_projectionMatrix = projection; }

		ComPtr<Device11> getDevice() const;
		ComPtr<Context11> getContext() const;
		ComPtr<SwapChain11> getSwapChain() const;
		ComPtr<RenderTargetView11> getRenderTargetView() const;


	private:
		ComPtr<Device11> m_device;
		ComPtr<Context11> m_context;
		ComPtr<SwapChain11> m_swapChain;
		ComPtr<RenderTargetView11> m_renderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		D3D11_VIEWPORT m_viewport;

		UINT m_backBufferWidth;
		UINT m_backBufferHeight;

		void createDeviceAndSwapChain();
		void createRenderTargetView();
		void createDepthStencil(UINT width, UINT height);
		void createBlendState();
		void createRasterizerState();
		void clearRenderTargetAndDepthStencil();
	};
}