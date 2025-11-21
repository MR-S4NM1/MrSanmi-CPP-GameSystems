#pragma once
#include <memory>
#include <cstdint>
#include <vector>
#include <functional>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: GameTypes.h
 * @brief: Core type definitions and aliases used throughout the game engine.
 * @details: Centralizes common types, enums, and smart pointer aliases to ensure consistency across the codebase.
 */

 /**
  * @brief Smart pointer alias for exclusive ownership semantics.
  * @tparam T The type managed by the pointer.
  * @details Equivalent to std::unique_ptr<T>. Use for resources with single ownership.
  */
template<typename T>
using Scope = std::unique_ptr<T>;

/**
 * @brief Smart pointer alias for shared ownership semantics.
 * @tparam T The type managed by the pointer.
 * @details Equivalent to std::shared_ptr<T>. Use for resources with shared ownership across multiple systems.
 */
template<typename T>
using Ref = std::shared_ptr<T>;

/**
 * @brief Comprehensive statistical attributes for game entities.
 * @details Contains all numerical values that define an entity's combat capabilities and current state.
 *          Used by systems such as combat, AI, and UI rendering.
 */
struct Stats {
    int32_t health;         ///< Current health points (0 = defeated)
    int32_t maxHealth;      ///< Maximum possible health points
    int32_t attack;         ///< Base attack power for damage calculations
    int32_t defense;        ///< Damage reduction capability
    int32_t speed;          ///< Determines turn order (higher = acts sooner)
    int32_t mana;           ///< Current magical resource for special abilities
    int32_t maxMana;        ///< Maximum possible mana points

    /**
     * @brief Constructs a Stats object with specified attribute values.
     * @param hp Initial and maximum health points (default: 100)
     * @param atk Base attack power (default: 10)
     * @param def Base defense value (default: 5)
     * @param spd Speed attribute (default: 10)
     * @param mp Initial and maximum mana points (default: 50)
     * @details Current values are initialized to their maximum counterparts.
     */
    Stats(int32_t hp = 100, int32_t atk = 10, int32_t def = 5,
        int32_t spd = 10, int32_t mp = 50)
        : health(hp), maxHealth(hp), attack(atk), defense(def),
        speed(spd), mana(mp), maxMana(mp) {
    }
};

/**
 * @brief Defines entity allegiance for team-based mechanics.
 * @details Used by AI systems, targeting logic, and victory condition checks.
 */
enum class Team {
    PLAYER,     ///< Player-controlled entities and allies
    ENEMY       ///< AI-controlled hostile entities
};

/**
 * @brief Finite state machine states for battle flow control.
 * @details Drives the turn-based battle progression and determines valid player interactions.
 */
enum class BattleState {
    TURN_START,     ///< Initialization phase for a new turn
    PLAYER_CHOICE,  ///< Waiting for player input to select actions
    ENEMY_THINKING, ///< AI decision-making phase
    ACTION_EXECUTE, ///< Skill resolution and effect application
    TURN_END,       ///< Cleanup and transition to next turn
    VICTORY,        ///< Battle concluded with player success
    DEFEAT          ///< Battle concluded with player failure
};

/**
 * @brief Callback type for battle event subscriptions.
 * @details Used by the event system to notify subscribers of state changes and turn transitions.
 */
using BattleEvent = std::function<void()>;