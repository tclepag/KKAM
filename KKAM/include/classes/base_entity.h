#pragma once

#include "common/common.h"

namespace classes {
	class CBaseEntity {
	public:
		CBaseEntity() = default;
		virtual ~CBaseEntity() = default;

		// Lifecycle methods

		virtual void load() = 0; // Entity loading
		virtual void ready() = 0; // Entity's first process call
		virtual void update() = 0; // Entity fixed update call
		virtual void render() = 0; // Entity render call
		// Marks this entity for destruction
		virtual void destroy() = 0; // Entity destroy call

		// Processing states

		// Enables or disables the entity's processing functionality (update, render)
		virtual void setDormant(bool dormant) { m_isDormant = dormant; }
		// Sets the entity's visibility
		virtual void setVisible(bool visible) { m_isVisible = visible; }
	private:
		// Entity states

		bool m_isDormant = true; // Entity is able to process (update, render)
		bool m_isVisible = true; // Entity is visible in the game, hides descendants if disabled
		bool m_isReady = false; // Entity is ready to be processed (passed load() and ready())
		bool m_isLoaded = false; // Entity is loaded (passed load())
		bool m_isDestroying = false; // Entity is marked for destruction
	};
}