#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

namespace classes {
	class CBaseEntity;
}

namespace core {
	class EntityManager {
	public:
		EntityManager();
		~EntityManager();
		size_t addEntity(classes::CBaseEntity* entity);
		void removeEntity(size_t entity);
		void markEntityForDestruction(size_t entity);
		std::shared_ptr<classes::CBaseEntity> getEntity(size_t id) const;
		void updateEntities();
		void renderEntities();

		void update();
	private:
		size_t m_index;
		std::vector<std::shared_ptr<classes::CBaseEntity>> m_entitiesToDestroy;
		std::unordered_map<size_t, std::shared_ptr<classes::CBaseEntity>> m_entities;
	};

	extern EntityManager g_entityManager;
}