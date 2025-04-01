#pragma once

namespace resources {
	template <typename CONTEXT>
	class Buffer {
	public:
		virtual ~Buffer() = default;

		virtual void initialize(CONTEXT* context) = 0;
		virtual void bind(CONTEXT* context) = 0;
		virtual void unbind(CONTEXT* context) = 0;
	};
}