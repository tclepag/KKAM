#include "classes/entities/entity.h"

namespace classes {
	CEntity::CEntity(): CBaseEntity() {
	}
	CEntity::~CEntity() {
		removeAllComponents();
		removeAllChildren();
	}

	void CEntity::removeComponent(CComponent* component) {
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (*it == component) {
				delete* it;
				m_components.erase(it);
				break;
			}
		}
	}

	void CEntity::removeAllComponents() {
		for (auto& component : m_components) {
			delete component;
		}
		m_components.clear();
	}

	CEntity* CEntity::getChild(size_t index) const {
		if (index < m_children.size()) {
			return m_children[index];
		}
		return nullptr;
	}

	CEntity* CEntity::getChild(const CString& name) const {
		for (auto& child : m_children) {
			if (child->name == name) {
				return child;
			}
		}
		return nullptr;
	}

	void CEntity::addChild(CEntity* child) {
		if (child) {
			child->setParent(this);
			m_children.push_back(child);
		}
	}

	void CEntity::removeChild(CEntity* child) {
		for (auto it = m_children.begin(); it != m_children.end(); ++it) {
			if (*it == child) {
				child->setParent(nullptr);
				m_children.erase(it);
				break;
			}
		}
	}

	void CEntity::removeAllChildren() {
		for (auto& child : m_children) {
			child->setParent(nullptr);
			delete child;
		}
		m_children.clear();
	}

	void CEntity::setParent(CEntity* parent) {
		if (m_parent) {
			m_parent->removeChild(this);
		}
		m_parent = parent;
		if (m_parent) {
			m_parent->addChild(this);
		}
	}

	void CEntity::load() {
		CBaseEntity::load();
	}
	void CEntity::ready() {
		CBaseEntity::ready();
	}
	void CEntity::update() {
		if (!isReady()) {
			return;
		}
		for (auto& component : m_components) {
			if (component && component->m_isDormant) {
				if (!component->isReady()) {
					continue;
				}
				component->update();
			}
		}
		for (auto& child : m_children) {
			if (child && child->isDormant()) {
				child->update();
			}
		}
	}
	void CEntity::render() {
		if (!isLoaded()) {
			return;
		}
		for (auto& component : m_components) {
			if (component && component->m_isDormant && component->m_isVisible) {
				if (!component->isLoaded()) {
					continue;
				}
				if (component->isReady()) {
					component->render();
				}
				else {
					component->ready();
				}
			}
		}
		for (auto& child : m_children) {
			if (child && child->isDormant() && child->isVisible()) {
				if (child->isReady()) {
					child->render();
				}
				else {
					child->ready();
				}
			}
		}
	}
}