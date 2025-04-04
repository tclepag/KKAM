#pragma once

#include "common/common.h"

namespace classes {
	class CEntity;
	class CComponent {
	public:
		CComponent() = default;
		virtual ~CComponent() = default;
		// Component properties
		CString name; // Component name
		// Lifecycle methods
		virtual void load() {} // Component loading
		virtual void ready() {} // Component's first process call
		virtual void update() {} // Component fixed update call
		virtual void render() {} // Component render call
		// Marks this component for destruction
		void destroy(); // Component destroy call
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