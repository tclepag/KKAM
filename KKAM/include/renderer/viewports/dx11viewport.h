#pragma once

#include "common/directx11.h"
#include "viewport.h"

namespace renderer {
	class DX11Renderer;
	namespace viewports {
		class DX11Viewport : public Viewport<Context11, SwapChain11> {
		public:
			DX11Viewport() = default;
			~DX11Viewport() = default;
			void init(Context11* context, SwapChain11* swapChain, UINT height, UINT width) override;
			void bind(Context11* context) override;
			void clear(Context11* context) override;

			
		private:
			friend class DX11Renderer;
			ComPtr<ID3D11DepthStencilView> m_depthStencilView;
			ComPtr<ID3D11RenderTargetView> m_renderTargetView;
			ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
			D3D11_VIEWPORT m_viewport;

			void releaseResources();

			void createViewport(UINT height, UINT width);
			void createRenderTarget(Context11* context, SwapChain11* swapChain);
			void createDepthStencil(Context11* context, SwapChain11* swapChain, UINT height, UINT width);
			void updateViewportDimensions(UINT width, UINT height);


		};
	}
}