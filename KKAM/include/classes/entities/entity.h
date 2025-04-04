#pragma once

#include "classes/base_entity.h"
#include "classes/component.h"

namespace classes {
	class CEntity : public CBaseEntity {
	public:
		CEntity() = default;
		~CEntity() override = default;

		// Entity properties

		CEntity* getParent() const { return m_parent; } // Returns the parent entity
		std::vector<CEntity*> getChildren() const { return m_children; } // Returns the child entities

		// Component methods

		template<typename T, typename... Args>
		T* addComponent(Args&&... args);
		template<typename T>
		T* getComponent() const;
		template<typename T>
		void removeComponent();
		void removeComponent(CComponent* component);
		void removeAllComponents();

		// Children methods

		CEntity* getChild(size_t index) const; // Returns the child entity at the specified index
		CEntity* getChild(const CString& name) const; // Returns the child entity with the specified name
		void addChild(CEntity* child); // Adds a child entity
		void removeChild(CEntity* child); // Removes a child entity
		void removeAllChildren(); // Removes all child entities

		// Parent methods

		void setParent(CEntity* parent); // Sets the parent entity

		// Lifecycle methods

		void load() override;
		void ready() override;
		void update() override;
		void render() override;
	protected:
		CEntity* m_parent = nullptr; // Parent entity
		std::vector<CEntity*> m_children; // Child entities
		std::vector<CComponent*> m_components; // Components attached to this entity
	};
}