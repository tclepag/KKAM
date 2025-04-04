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
		}
	protected:
		unsigned int textureID;
		int width, height, channels;
		String m_filepath;
	};
}