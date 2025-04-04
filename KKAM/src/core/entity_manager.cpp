#include "core/entity_manager.h"
#include "classes/base_entity.h"

using namespace classes;

namespace core {
	EntityManager g_entityManager;
	EntityManager::EntityManager() {
		g_entityManager = *this;
	}
	EntityManager::~EntityManager() {
		for (auto& [key, entity] : m_entities) {
			if (entity) {
				entity->destroy();
			}
		}
		m_entities.clear();
		g_entityManager = *this;
	}
	size_t EntityManager::addEntity(classes::CBaseEntity* entity) {
		if (entity) {
			auto sharedEntity = std::shared_ptr<classes::CBaseEntity>(entity);
			sharedEntity->m_id = m_index;
			m_entities[m_index] = sharedEntity;
			m_index++;
			return m_index - 1;
		}
	}
	void EntityManager::removeEntity(size_t entity) {
		if (entity) {
			m_entities.erase(entity);
		}
	}
	void EntityManager::markEntityForDestruction(size_t entity) {
		auto it = m_entities.find(entity);
		if (it != m_entities.end()) {
			auto& entity = it->second;
			entity->m_isDestroying = true;
			m_entitiesToDestroy.push_back(entity);
		}
	}
	void EntityManager::updateEntities() {
		for (auto& [key, entity] : m_entities) {
			if (entity && entity->m_isDormant) {
				entity->update();
			}
		}
	}
	void EntityManager::renderEntities() {
		for (auto& [key, entity] : m_entities) {
			if (entity && entity->m_isDormant && entity->m_isVisible) {
				if (entity->m_isReady) {
					entity->render();
				}
				else {
					entity->ready();
				}
			}
		}
	}
	void EntityManager::update() {
		for (auto& entity : m_entitiesToDestroy) {
			if (entity && entity->m_isDestroying) {
				removeEntity(entity->m_id);
			}
		}
		m_entitiesToDestroy.clear();
	}
}