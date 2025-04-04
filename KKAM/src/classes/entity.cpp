#include "classes/entities/entity.h"

namespace classes {
	CEntity::CEntity(): CBaseEntity() {
	}
	CEntity::~CEntity() {
		removeAllComponents();
		removeAllChildren();
	}

	template<typename T, typename... Args>
	T* CEntity::addComponent(Args&&... args) {
		static_assert(std::is_base_of<CComponent, T>::value, "T must be derived from CComponent");
		T* component = new T(std::forward<Args>(args)...);
		component->m_parent = this;
		m_components.push_back(component);
		return component;
	}

	template<typename T>
	T* CEntity::getComponent() const {
		static_assert(std::is_base_of<CComponent, T>::value, "T must be derived from CComponent");
		for (auto& component : m_components) {
			if (dynamic_cast<T*>(component)) {
				return dynamic_cast<T*>(component);
			}
		}
		return nullptr;
	}

	template<typename T>
	void CEntity::removeComponent() {
		static_assert(std::is_base_of<CComponent, T>::value, "T must be derived from CComponent");
		for (auto it = m_components.begin(); it != m_components.end(); ++it) {
			if (dynamic_cast<T*>(*it)) {
				delete* it;
				m_components.erase(it);
				break;
			}
		}
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
		for (auto& component : m_components) {
			if (component && component->m_isDormant) {
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
		for (auto& component : m_components) {
			if (component && component->m_isDormant && component->m_isVisible) {
				component->render();
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