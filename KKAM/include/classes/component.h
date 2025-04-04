#pragma once

#include "common/common.h"
#include "core/engine.h"

namespace classes {
	using namespace core;
	class CEntity;
	class CComponent {
	public:
		CComponent();
		virtual ~CComponent();
		// Component properties
		CString name; // Component name
		// Lifecycle methods
		virtual void load(); // Component loading
		virtual void ready(); // Component's first process call
		virtual void update(); // Component fixed update call
		virtual void render(); // Component render call
		// Marks this component for destruction
		void destroy(); // Component destroy call

		void setDormant(bool dormant) {
			m_isDormant = dormant;
		}

		void setParent(CEntity* parent) { m_parent = parent; } // Sets the parent entity
		CEntity* getParent() const { return m_parent; } // Returns the parent entity

		const bool isReady() const { return m_isReady; } // Component is ready to be processed (passed load() and ready())
		const bool isLoaded() const { return m_isLoaded; } // Component is loaded (passed load())
		const bool isDestroying() const { return m_isDestroying; } // Component is marked for destruction
		const bool isDormant() const { return m_isDormant; } // Component is able to process (update, render)
		const bool isVisible() const { return m_isVisible; } // Component is visible in the game
	private:
		friend class CEntity;
		// Component properties
		CEntity* m_parent = nullptr; // Parent entity
		// Component states
		bool m_isDestroying = false; // Component is marked for destruction
		bool m_isLoaded = false; // Component is loaded (passed load())
		bool m_isReady = false; // Component is ready to be processed (passed load() and ready())
		bool m_isDormant = true; // Component is able to process (update, render)
		bool m_isVisible = true; // Component is visible in the game
	};
}