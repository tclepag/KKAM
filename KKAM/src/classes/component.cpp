#include "classes/component.h"
#include "classes/entities/entity.h"

namespace classes {
	CComponent::CComponent() {
		m_parent = nullptr;
		load();
	}
	CComponent::~CComponent() {
		if (m_parent) {
			m_parent->removeComponent(this);
		}
	}
	void CComponent::load() {
		m_isLoaded = true;
	}
	void CComponent::ready() {
		m_isReady = true;
	}
	void CComponent::update() {
		if (m_isDormant) {
			return;
		}
	}
	void CComponent::render() {
		if (m_isDormant) {
			return;
		}
		if (!m_isReady) {
			ready();
		}
		if (m_isVisible) {
			render();
		}
	}
	void CComponent::destroy() {
		if (m_isDestroying) {
			return;
		}
		m_isDestroying = true;
	}
} // namespace classes