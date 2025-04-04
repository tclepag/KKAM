#pragma once

#include "classes/component.h"
#include "common/common.h"
#include "common/directx11.h"

#include <map>

#include "resources/dx11/dx11vb.h"
#include "resources/dx11/dx11ib.h"
#include "resources/dx11/dx11shader.h"
#include "resources/dx11/dx11texture.h"

namespace classes {
	using namespace resources;

	using Texture = DX11Texture*;
	using TextureArray = std::map<size_t, Texture>;
	using Vertices = std::vector<Vertex>;
	using Indices = std::vector<uint32_t>;

	class CMesh : public CComponent {
	public:
		CMesh();
		~CMesh() override;

		// Mesh properties

		const Vertices& getVertices() const { return m_vb->getVertices(); } // Returns the vertices of the mesh
		const Indices& getIndices() const { return m_ib->getIndices(); } // Returns the indices of the mesh
		const TextureArray& getTextures() const { return m_shader->getTextures(); } // Returns the textures of the mesh
		const Transform& getTransform() const { return m_transform; } // Returns the transform of the mesh

		void setContext(Context11* context) { m_context = context; } // Sets the context of the mesh

		void setVertexShader(const String& vertexPath) { m_shader->setVertexPath(vertexPath); } // Sets the vertex shader of the mesh
		void setPixelShader(const String& pixelPath) { m_shader->setPixelPath(pixelPath); } // Sets the pixel shader of the mesh
		void setLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout) { m_shader->setLayout(layout); } // Sets the input layout of the mesh

		void setVertices(const Vertices& vertices) { m_vb->setVertices(vertices); } // Sets the vertices of the mesh
		void setIndices(const Indices& indices) { m_ib->setIndices(indices); } // Sets the indices of the mesh

		void setTexture(size_t index, Texture texture); // Sets the texture of the mesh

		void setTransform(const Transform& transform) { m_transform = transform; } // Sets the transform of the mesh

		void load() override;
		void ready() override;
		void update() override {}
		void render() override;
	private:
		Context11* m_context = nullptr;
		UniquePtr<DX11VertexBuffer> m_vb = nullptr;
		UniquePtr<DX11IndexBuffer> m_ib = nullptr;
		UniquePtr<DX11Shader> m_shader = nullptr;
		Transform m_transform;
	};
}