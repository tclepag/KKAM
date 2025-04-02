#pragma once

#include "common/common.h"

namespace resources {
	class Shader {
	public:
		Shader() = default;
		~Shader() = default;
		virtual void build() = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;
		virtual void releaseResources() = 0;
		virtual void setVertexPath(const String& vertexPath) = 0;
		virtual void setPixelPath(const String& pixelPath) = 0;
	};
}