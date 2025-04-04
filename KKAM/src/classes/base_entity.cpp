#include "classes/base_entity.h"

namespace classes {
	CBaseEntity::CBaseEntity() {
		m_id = g_entityManager.addEntity(this);
		load();
	}
	CBaseEntity::~CBaseEntity() {
		g_entityManager.removeEntity(m_id);
	}
	void CBaseEntity::load() {
		m_isLoaded = true;
	}
	void CBaseEntity::ready() {
		m_isReady = true;
	}
	void CBaseEntity::update() {
		if (!m_isDormant) {
			return;
		}
	}
	void CBaseEntity::render() {
		if (!m_isDormant || !m_isLoaded) {
			return;
		}
		if (!m_isReady) {
			ready();
		}
		else {
			render();
		}
	}
	void CBaseEntity::destroy() const {
		if (m_isDestroying) {
			return;
		}
		g_entityManager.markEntityForDestruction(m_id);
	}
}