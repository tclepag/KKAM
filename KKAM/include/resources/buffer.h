#pragma once

namespace resources {
	template <typename CONTEXT>
	class Buffer {
	public:
		virtual ~Buffer() = default;

		virtual void initialize(CONTEXT* context) {}
		virtual void bind(CONTEXT* context) {}
		virtual void unbind(CONTEXT* context) {}
		void setSlot(size_t slot) {
			m_slot = slot;
		}
	protected:
		size_t m_slot = 0;
	};
}