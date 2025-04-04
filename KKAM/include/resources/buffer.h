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

		bool needsUpdate() const {
			return m_needsUpdate;
		}
	protected:
		size_t m_slot = 0;
		bool m_needsUpdate = false;
	};
}