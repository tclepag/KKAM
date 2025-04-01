#pragma once

namespace renderer {
	class Renderer {
	public:
		virtual ~Renderer() = default;
		virtual void init() = 0;
		virtual void render() = 0;
		virtual void shutdown() = 0;
		virtual void rebuild() = 0;
		virtual void resize() = 0;
	};
} // namespace renderer