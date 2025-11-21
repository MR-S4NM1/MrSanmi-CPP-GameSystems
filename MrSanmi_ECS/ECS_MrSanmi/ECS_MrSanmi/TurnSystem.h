#pragma once
#include "GameTypes.h"
#include "Entity.h"
#include "Skill.h"
#include <queue>
#include <memory>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: TurnSystem.h
 * @brief: Core turn management system for implementing turn-based combat mechanics.
 * @details: Manages turn order calculation, state transitions, and event handling for battle sequences.
 *           Implements priority-based turn ordering with team-based prioritization.
 */

 /**
  * @brief Represents an entity's position in the turn order queue.
  * @details Used by the priority queue to determine action sequence based on speed and team priority.
  */
struct TurnOrder
{
    Ref<Entity> entity;     ///< Reference to the entity taking the turn
    int speed{ 0 };           ///< Speed stat used for ordering (higher = acts sooner)
    int priority{ 0 };        ///< Team-based priority (players > enemies)

    /**
     * @brief Comparison operator for priority queue ordering.
     * @param other The TurnOrder to compare against.
     * @return True if this entity should act after the other entity.
     * @details Orders by priority first (higher priority acts first), then by speed (higher speed acts first).
     */
    bool operator<(const TurnOrder& other) const {
        if (priority != other.priority)
            return priority < other.priority;

        return speed < other.speed;
    }
};

/**
 * @brief Manages turn-based combat flow, state transitions, and event systems.
 * @details Implements a finite state machine for battle progression and handles turn order calculation
 *          using a priority queue. Supports event-driven programming through subscription system.
 */
class TurnSystem
{
private:
    // Core System State
    std::priority_queue<TurnOrder> turnQueue;           ///< Priority queue determining turn order
    std::vector<Ref<Entity>> battleEntities;           ///< All entities participating in the battle
    BattleState currentState{ BattleState::TURN_START }; ///< Current phase of battle execution
    Ref<Entity> currentActor;                          ///< Entity currently permitted to take actions

    // Event System
    std::vector<BattleEvent> onTurnStartEvents;        ///< Callbacks executed when a turn begins
    std::vector<BattleEvent> onTurnEndEvents;          ///< Callbacks executed when a turn ends

public:
    // Battle Lifecycle Management
    /**
     * @brief Initializes the battle system with participating entities.
     * @param entities Collection of all entities involved in the battle.
     * @details Calculates initial turn order and transitions to first turn.
     */
    void initializeBattle(const std::vector<Ref<Entity>>& entities);

    /**
     * @brief Calculates and sorts turn order based on entity speed and team priority.
     * @details Only includes alive entities. Players receive higher priority than enemies.
     */
    void calculateTurnOrder();

    /**
     * @brief Advances to the next entity's turn in the queue.
     * @details Executes turn start events, updates entity status, and determines next battle state.
     */
    void startNextTurn();

    /**
     * @brief Completes the current turn and advances battle state.
     * @details Executes turn end events, checks battle conditions, and triggers next turn if battle continues.
     */
    void endCurrentTurn();

    // State Management
    /**
     * @brief Transitions the battle to a new state.
     * @param newState The target state to transition to.
     * @details Handles automatic state transitions and side effects for specific state changes.
     */
    void setState(BattleState newState);

    /**
     * @brief Retrieves the current battle state.
     * @return The current BattleState enum value.
     */
    BattleState getCurrentState() const { return currentState; }

    /**
     * @brief Retrieves the entity currently taking actions.
     * @return Shared pointer to the active entity.
     */
    Ref<Entity> getCurrentActor() const { return currentActor; }

    // Event System
    /**
     * @brief Subscribes a callback to turn start events.
     * @param event Callback function to execute when a turn begins.
     * @note Callbacks are executed in subscription order.
     */
    void subscribeToTurnStart(BattleEvent event);

    /**
     * @brief Subscribes a callback to turn end events.
     * @param event Callback function to execute when a turn ends.
     * @note Callbacks are executed in subscription order.
     */
    void subscribeToTurnEnd(BattleEvent event);

    // Battle Status Queries
    /**
     * @brief Checks if the battle is currently active.
     * @return True if battle is in progress, false if victory/defeat conditions are met.
     */
    bool isBattleActive() const;

    /**
     * @brief Evaluates victory/defeat conditions based on entity status.
     * @details Checks if all players or all enemies have been defeated.
     */
    void checkBattleConditions();

private:
    // Internal Event Processing
    /**
     * @brief Executes all registered turn start event callbacks.
     */
    void executeTurnStartEvents();

    /**
     * @brief Executes all registered turn end event callbacks.
     */
    void executeTurnEndEvents();

    /**
     * @brief Updates entity status based on current health values.
     * @details Marks entities as defeated if health drops to zero or below.
     */
    void updateEntityStatus();
};