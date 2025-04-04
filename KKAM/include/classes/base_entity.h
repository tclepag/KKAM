#pragma once

#include "common/common.h"
#include "core/entity_manager.h"

namespace classes {
	using core::g_entityManager;
	class CBaseEntity {
	public:
		CBaseEntity() = default;
		virtual ~CBaseEntity() = default;

		// Entity properties

		CString name; // Entity name
		const size_t getId() const { return m_id; } // Entity ID
		const bool isReady() const { return m_isReady; } // Entity is ready to be processed (passed load() and ready())
		const bool isLoaded() const { return m_isLoaded; } // Entity is loaded (passed load())
		const bool isDestroying() const { return m_isDestroying; } // Entity is marked for destruction
		const bool isDormant() const { return m_isDormant; } // Entity is able to process (update, render)
		const bool isVisible() const { return m_isVisible; } // Entity is visible in the game

		// Lifecycle methods

		virtual void load(); // Entity loading
		virtual void ready(); // Entity's first process call
		virtual void update(); // Entity fixed update call
		virtual void render(); // Entity render call
		// Marks this entity for destruction
		void destroy() const; // Entity destroy call

		// Processing states

		// Enables or disables the entity's processing functionality (update, render)
		virtual void setDormant(bool dormant) { m_isDormant = dormant; }
		// Sets the entity's visibility
		virtual void setVisible(bool visible) { m_isVisible = visible; }
	private:
		friend class core::EntityManager;
		// Entity properties
		
		size_t m_id = 0; // Entity ID


		// Entity states

		bool m_isDormant = true; // Entity is able to process (update, render)
		bool m_isVisible = true; // Entity is visible in the game, hides descendants if disabled
		bool m_isReady = false; // Entity is ready to be processed (passed load() and ready())
		bool m_isLoaded = false; // Entity is loaded (passed load())
		bool m_isDestroying = false; // Entity is marked for destruction
	};
}