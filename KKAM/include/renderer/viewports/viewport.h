#pragma once

namespace renderer {
	namespace viewports {
		template <typename CONTEXT, typename SWAPCHAIN>
		class Viewport {
		public:
			virtual ~Viewport() = default;
			virtual void init(CONTEXT* context, SWAPCHAIN* swapChain) = 0;
			virtual void bind(CONTEXT* context) = 0;
		};
	}
}