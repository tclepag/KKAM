#pragma once

#include "common/common.h"

namespace renderer {
	class Renderer {
	public:
		virtual ~Renderer() = default;
		virtual void init() = 0;
		virtual void beginFrame() = 0;
		virtual void endFrame() = 0;
		virtual void shutdown() = 0;
		virtual void rebuild() = 0;
		virtual void resize() = 0;
		virtual void setViewMatrix(const Matrix4& view) = 0;
		virtual void setProjectionMatrix(const Matrix4& projection) = 0;
		void getViewMatrix(Matrix4& view) const {
			view = m_viewMatrix;
		}
		void getProjectionMatrix(Matrix4& projection) const {
			projection = m_projectionMatrix;
		}
		void setClearColor(const Vector4& color) {
			m_clearColor = color;
		}
		Matrix4 getViewMatrix() {
			return m_viewMatrix;
		}
		Matrix4 getProjectionMatrix() {
			return m_projectionMatrix;
		}
	protected:
		Matrix4 m_viewMatrix = Matrix4::identity();
		Matrix4 m_projectionMatrix = Matrix4::identity();
		Vector4 m_clearColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	};
} // namespace renderer