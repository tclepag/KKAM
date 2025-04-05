#include "classes/components/mesh.h"
#include "classes/entities/entity.h"

namespace classes {
	CMesh::CMesh() {
		setDormant(false);
		m_vb = std::make_unique<DX11VertexBuffer>();
		m_ib = std::make_unique<DX11IndexBuffer>();
		m_shader = std::make_unique<DX11Shader>();

		m_context = g_engine->getRenderer()->getContext().Get();
		m_shader->setContext(m_context);
		setDormant(true);
	}
	CMesh::~CMesh() {
		if (m_shader) {
			m_shader->releaseResources();
		}
	}
	void CMesh::setTexture(size_t index, Texture texture) {
		m_shader->setTexture(index, texture);
	}
	void CMesh::load() {
		m_shader->createConstantBuffer(L"Transform", sizeof(Transform));
		CComponent::load();
	}

	void CMesh::ready() {
		loadMesh();
		if (m_vb) {
			m_vb->initialize(m_context);
		}
		if (m_ib) {
			m_ib->initialize(m_context);
		}
		if (m_shader) {
			m_shader->build();
		}
		CComponent::ready();
	}

	void CMesh::render() {
		CComponent::render();

		if (m_meshNeedsUpdate) {
			loadMesh();
			m_meshNeedsUpdate = false;
		}

		if (m_vb && m_vb->needsUpdate()) {
			m_vb->initialize(m_context);
		}
		if (m_ib && m_ib->needsUpdate()) {
			m_ib->initialize(m_context);
		}
		if (m_shader && m_shader->needsUpdate()) {
			m_shader->build();
		}

		Matrix4 view = g_engine->getRenderer()->getViewMatrix();
		Matrix4 proj = g_engine->getRenderer()->getProjectionMatrix();

		Transform meshTransform;
		meshTransform.model = getParent()->matrix;
		meshTransform.view = view.transpose();
		meshTransform.projection = proj.transpose();

		if (m_shader) {
			m_shader->updateConstantBuffer(L"Transform", &meshTransform);
		}

		if (m_vb && m_ib && m_shader) {
			m_shader->bind();
			m_vb->bind(m_context);
			m_ib->bind(m_context);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_context->DrawIndexed(m_ib->getIndexCount(), 0, 0);
		}
	}

	void CMesh::loadMesh() {
		try {
			m_data = MeshLoader::load(m_path);
		}
		catch (const nlohmann::json::parse_error& e) {
			utils::Console::error(CString("Failed to parse JSON: ") + CString(e.what()).c_str());
			return;
		}
		for (const auto& texture : m_data.textures) {
			texture.second->setContext(m_context);
			texture.second->loadFromFile();
			setTexture(texture.first, texture.second);
		}
		std::vector<Vertex> vertices;

		if (!m_data.vertices.empty()) {
			for (int i = 0; i < m_data.vertices.size() - 1; i++) {
				vertices.push_back({ m_data.vertices[i].x, m_data.vertices[i].y, m_data.vertices[i].z,
					m_data.normals[i].x, m_data.normals[i].y, m_data.normals[i].z,
					m_data.texCoords[i].x, m_data.texCoords[i].y });
			}
		}
		setVertices(vertices);
		setIndices(m_data.indices);
	}
}