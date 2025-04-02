#pragma once

#include <array>

namespace renderer {
	namespace viewports {
		template <typename CONTEXT, typename SWAPCHAIN>
		class Viewport {
		public:
			virtual ~Viewport() = default;
			virtual void init(CONTEXT* context, SWAPCHAIN* swapChain, UINT height, UINT width) = 0;
			virtual void bind(CONTEXT* context) = 0;
			virtual void clear(CONTEXT* context) = 0;
			void setClearColor(float r, float g, float b, float a) {
				m_clearColor = { r, g, b, a };
			}
		protected:
			std::array<float, 4> m_clearColor = { 0.392f, 0.584f, 0.929f, 1.0f };
		};
	}
}