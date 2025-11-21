#pragma once
#include "GameTypes.h"
#include <string>
#include <unordered_map>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: Component.h
 * @brief: Defines the Component system, a core pattern for building flexible game entities.
 */

 /**
  * @brief Base interface for all components.
  * @details Components are data-only structures that label an Entity as possessing a particular set of attributes or capabilities.
  */
struct IComponent {
	virtual ~IComponent() = default;
};

/**
 * @brief Component containing core identity and world-state information for an Entity.
 */
struct TransformComponent : IComponent 
{
	std::string name;
	Team team;

	TransformComponent(const std::string& entityName, Team entityTeam)
		: name(entityName), team(entityTeam) { }
};

/**
 * @brief Component for entities that can participate in combat, managing health and stats.
 */
struct HealthComponent : IComponent {
	Stats stats;
	bool isAlive{true};

	explicit HealthComponent(const Stats& entityStats) : stats(entityStats) {}
};

/**
 * @brief Component for managing turn-based battle state per entity.
 */
struct BattleComponent : IComponent {
	//Flag to track if the entity has taken its action in the current turn.
	bool hasActed{ false };

	//Calculated value to determine action sequence (higher speed acts first).
	int turnOrder{ 0 };

	BattleComponent() = default;
};