#include "classes/components/mesh.h"

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
		if (m_vb && m_vb->needsUpdate()) {
			m_vb->initialize(m_context);
		}
		if (m_ib && m_ib->needsUpdate()) {
			m_ib->initialize(m_context);
		}
		if (m_shader && m_shader->needsUpdate()) {
			m_shader->build();
		}

		if (m_shader) {
			m_shader->updateConstantBuffer(L"Transform", &m_transform);
		}

		if (m_vb && m_ib && m_shader) {
			m_shader->bind();
			m_vb->bind(m_context);
			m_ib->bind(m_context);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_context->DrawIndexed(m_ib->getIndexCount(), 0, 0);
		}
	}
}