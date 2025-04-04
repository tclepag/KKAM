#include "classes/component.h"
#include "classes/entities/entity.h"

namespace classes {
	CComponent::CComponent() {
		m_parent = nullptr;
		load();
	}
	CComponent::~CComponent() {

	}
	void CComponent::load() {
		m_isLoaded = true;
	}
	void CComponent::ready() {
		m_isReady = true;
	}
	void CComponent::update() {
		if (!m_isDormant) {
			return;
		}
	}
	void CComponent::render() {
		if (!m_isDormant) {
			return;
		}
		if (!m_isReady) {
			ready();
		}
	}
	void CComponent::destroy() {
		if (m_isDestroying) {
			return;
		}
		m_isDestroying = true;
	}
} // namespace classes