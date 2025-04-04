#pragma once

#include "classes/component.h"
#include "common/common.h"
#include "common/directx11.h"

#include "resources/dx11/dx11vb.h"
#include "resources/dx11/dx11ib.h"
#include "resources/dx11/dx11shader.h"
#include "resources/dx11/dx11texture.h"

namespace classes {
	using namespace resources;
	class CMesh : public CComponent {
	public:
		CMesh() = default;
		~CMesh() override = default;

		// Mesh properties

		const std::vector<Vertex>& getVertices() const { return m_vertices; } // Returns the vertices of the mesh
		const std::vector<uint32_t>& getIndices() const { return m_indices; } // Returns the indices of the mesh
		const std::vector<Texture>& getTextures() const { return m_textures; } // Returns the textures of the mesh
		const Transform& getTransform() const { return m_transform; } // Returns the transform of the mesh

		void setContext(Context11* context) { m_context = context; } // Sets the context of the mesh

		void setVertices(const std::vector<Vertex>& vertices) { m_vertices = vertices; } // Sets the vertices of the mesh
		void setIndices(const std::vector<uint32_t>& indices) { m_indices = indices; } // Sets the indices of the mesh


		void load() override;
		void ready() override;
		void update() override;
		void render() override;
	private:
		Context11* m_context = nullptr;
		UniquePtr<DX11VertexBuffer> m_vb = nullptr;
		UniquePtr<DX11IndexBuffer> m_ib = nullptr;
		UniquePtr<DX11Shader> m_shader = nullptr;
		DX11Texture* m_texture = nullptr;
		Transform m_transform;
		std::vector<Vertex> m_vertices;
		std::vector<uint32_t> m_indices;
		std::vector<Texture> m_textures;
	};
}