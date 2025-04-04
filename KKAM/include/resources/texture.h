#pragma once

#include "common/common.h"

namespace resources {
	class Texture {
	public:
		Texture() = default;
		~Texture() = default;
		virtual void loadFromFile() = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		void setFilePath(const String& filepath) {
			m_filepath = filepath;
			m_needsUpdate = true;
		}
		void setSlot(size_t slot) {
			m_slot = slot;
		}
		const size_t& getSlot() const {
			return m_slot;
		}

		bool needsUpdate() const {
			return m_needsUpdate;
		}
	protected:
		bool m_needsUpdate = false;
		size_t m_slot;
		unsigned int m_textureID;
		int m_width, m_height, m_channels;
		String m_filepath;
	};
}