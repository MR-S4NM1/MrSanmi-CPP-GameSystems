#pragma once
#include "TurnSystem.h"
#include "Skill.h"
#include <map>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: BattleManager.h
 * @brief: Central coordinator for battle operations, managing entities, skills, and turn progression.
 * @details: Acts as the facade between game systems and provides a simplified interface for battle management.
 */

 /**
  * @brief Main controller for battle sequences and entity management.
  * @details Orchestrates the turn system, skill execution, and entity lifecycle during combat scenarios.
  *          Implements the Facade pattern to simplify complex subsystem interactions.
  */
class BattleManager {
private:
    TurnSystem turnSystem;                              ///< Manages turn order and battle state transitions
    std::vector<Ref<Entity>> allEntities;              ///< Collection of all participating entities in the battle
    std::map<std::string, Skill> availableSkills;      ///< Registry of combat skills mapped by identifier

public:
    /**
     * @brief Constructs a BattleManager and initializes core systems.
     * @details Automatically sets up skill registry and event handlers upon construction.
     */
    BattleManager();

    // Entity Management
    /**
     * @brief Creates and registers a player-controlled entity.
     * @param name The display name of the player entity.
     * @param stats The combat statistics for the player entity.
     */
    void addPlayer(const std::string& name, const Stats& stats);

    /**
     * @brief Creates and registers an enemy entity.
     * @param name The display name of the enemy entity.
     * @param stats The combat statistics for the enemy entity.
     */
    void addEnemy(const std::string& name, const Stats& stats);

    // Battle Flow Control
    /**
     * @brief Initializes and starts a new battle sequence.
     * @details Sets up turn order and transitions to the first combat turn.
     */
    void startBattle();

    /**
     * @brief Executes a player-initiated action during their turn.
     * @param skillName The identifier of the skill to execute.
     * @param target The recipient entity of the skill effect (defaults to automatic selection).
     * @throws std::runtime_error if action execution violates turn rules or resource constraints.
     */
    void executePlayerAction(const std::string& skillName, Ref<Entity> target = nullptr);

    /**
     * @brief Updates battle state and processes non-player entity actions.
     * @details Called once per frame to advance battle logic, including enemy AI decisions.
     */
    void update();

    // State Queries
    /**
     * @brief Checks if the battle is currently active.
     * @return True if battle is in progress, false if victory/defeat conditions are met.
     */
    bool isBattleActive() const { return turnSystem.isBattleActive(); }

    /**
     * @brief Retrieves the current phase of battle execution.
     * @return The current BattleState representing battle progression stage.
     */
    BattleState getBattleState() const { return turnSystem.getCurrentState(); }

    /**
     * @brief Gets the entity currently permitted to take actions.
     * @return Shared pointer to the active entity, or nullptr if no entity is active.
     */
    Ref<Entity> getCurrentActor() const { return turnSystem.getCurrentActor(); }

    /**
     * @brief Provides access to all registered battle entities.
     * @return Constant reference to the collection of all entities.
     */
    const std::vector<Ref<Entity>>& getEntities() const { return allEntities; }

    /**
     * @brief Provides access to the available skill registry.
     * @return Constant reference to the map of skill identifiers to Skill objects.
     */
    const std::map<std::string, Skill>& getSkills() const { return availableSkills; }

    // Convenience Accessors
    /**
     * @brief Retrieves all player-aligned entities.
     * @return Vector containing all entities belonging to the player team.
     */
    std::vector<Ref<Entity>> getPlayers() const { return getPlayerEntities(); }

    /**
     * @brief Retrieves all enemy-aligned entities.
     * @return Vector containing all entities belonging to the enemy team.
     */
    std::vector<Ref<Entity>> getEnemies() const { return getEnemyEntities(); }

    // Utility Methods
    /**
     * @brief Filters entities that are currently active in combat.
     * @return Vector containing only entities with positive health status.
     */
    std::vector<Ref<Entity>> getAliveEntities() const;

    /**
     * @brief Filters entities belonging to the enemy team.
     * @return Vector containing only entities with Team::ENEMY alignment.
     */
    std::vector<Ref<Entity>> getEnemyEntities() const;

    /**
     * @brief Filters entities belonging to the player team.
     * @return Vector containing only entities with Team::PLAYER alignment.
     */
    std::vector<Ref<Entity>> getPlayerEntities() const;

private:
    /**
     * @brief Populates the skill registry with predefined combat abilities.
     * @details Called during construction to initialize availableSkills map.
     */
    void initializeSkills();

    /**
     * @brief Configures event handlers for turn-based battle events.
     * @details Sets up callbacks for turn transitions and state changes.
     */
    void setupEventHandlers();

    /**
     * @brief Factory method for entity creation with proper component initialization.
     * @param name The display name for the new entity.
     * @param team The team alignment for the new entity.
     * @param stats The combat statistics for the new entity.
     * @return Shared pointer to the fully constructed entity.
     */
    Ref<Entity> createEntity(const std::string& name, Team team, const Stats& stats);
};