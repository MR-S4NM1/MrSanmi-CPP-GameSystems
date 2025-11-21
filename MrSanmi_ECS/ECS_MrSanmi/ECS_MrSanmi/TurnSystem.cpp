#include "TurnSystem.h"
#include <iostream>
#include <algorithm>

/**
 * @brief Initializes the battle system with participating entities.
 */
void TurnSystem::initializeBattle(const std::vector<Ref<Entity>>& entities) {
    battleEntities = entities;
    calculateTurnOrder();

    // DIRECT START OF FIRST TURN
    std::cout << "=== BATTLE STARTED ===\n";
    startNextTurn();
}

/**
 * @brief Calculates and sorts turn order based on entity speed and team priority.
 */
void TurnSystem::calculateTurnOrder() {
    // Clear previous queue
    while (!turnQueue.empty()) turnQueue.pop();

    for (const auto& entity : battleEntities) {
        if (auto* health = entity->getComponent<HealthComponent>()) {
            if (health->isAlive) {
                TurnOrder order;
                order.entity = entity;
                order.speed = health->stats.speed;

                // Give priority to players over enemies
                if (auto* transform = entity->getComponent<TransformComponent>()) {
                    order.priority = (transform->team == Team::PLAYER) ? 1 : 0;
                }

                turnQueue.push(order);
                std::cout << "DEBUG - " << order.entity->getComponent<TransformComponent>()->name
                    << " added to queue (speed: " << order.speed << ")\n";
            }
        }
    }
}

/**
 * @brief Advances to the next entity's turn in the queue.
 */
void TurnSystem::startNextTurn() {
    std::cout << "DEBUG - Starting next turn...\n";

    if (turnQueue.empty()) {
        calculateTurnOrder();
        if (turnQueue.empty()) {
            setState(BattleState::VICTORY);
            return;
        }
    }

    currentActor = turnQueue.top().entity;
    turnQueue.pop();

    executeTurnStartEvents();
    updateEntityStatus();

    std::cout << "\n--- NEW TURN ---\n";
    if (auto* transform = currentActor->getComponent<TransformComponent>()) {
        std::cout << "Turn of: " << transform->name << "\n";
    }

    // Determine next state based on team
    if (auto* transform = currentActor->getComponent<TransformComponent>()) {
        BattleState nextState = (transform->team == Team::PLAYER) ?
            BattleState::PLAYER_CHOICE :
            BattleState::ENEMY_THINKING;
        std::cout << "DEBUG - Changing to state: " << static_cast<int>(nextState) << "\n";
        currentState = nextState;
    }
}

/**
 * @brief Completes the current turn and advances battle state.
 */
void TurnSystem::endCurrentTurn() {
    executeTurnEndEvents();
    checkBattleConditions();

    if (isBattleActive()) {
        startNextTurn();
    }
}

/**
 * @brief Transitions the battle to a new state.
 */
void TurnSystem::setState(BattleState newState) {
    std::cout << "DEBUG - Changing state from " << static_cast<int>(currentState)
        << " to " << static_cast<int>(newState) << "\n";
    currentState = newState;

    // Automatic logic for state transitions
    switch (currentState) {
    case BattleState::ENEMY_THINKING:
        std::cout << "The enemy is thinking...\n";
        setState(BattleState::ACTION_EXECUTE);
        break;

    case BattleState::ACTION_EXECUTE:
        std::cout << "Action executed!\n";
        setState(BattleState::TURN_END);
        break;

    case BattleState::TURN_END:
        std::cout << "DEBUG - Ending current turn\n";
        endCurrentTurn();
        break;

    default:
        break;
    }
}

// Event System
/**
 * @brief Subscribes a callback to turn start events.
 */
void TurnSystem::subscribeToTurnStart(BattleEvent event) {
    onTurnStartEvents.push_back(std::move(event));
}

/**
 * @brief Subscribes a callback to turn end events.
 */
void TurnSystem::subscribeToTurnEnd(BattleEvent event) {
    onTurnEndEvents.push_back(std::move(event));
}

/**
 * @brief Executes all registered turn start event callbacks.
 */
void TurnSystem::executeTurnStartEvents() {
    for (const auto& event : onTurnStartEvents) {
        if (event) event();
    }
}

/**
 * @brief Executes all registered turn end event callbacks.
 */
void TurnSystem::executeTurnEndEvents() {
    for (const auto& event : onTurnEndEvents) {
        if (event) event();
    }
}

/**
 * @brief Updates entity status based on current health values.
 */
void TurnSystem::updateEntityStatus() {
    for (auto& entity : battleEntities) {
        if (auto* health = entity->getComponent<HealthComponent>()) {
            if (health->stats.health <= 0) {
                health->isAlive = false;
                health->stats.health = 0;
                if (auto* transform = entity->getComponent<TransformComponent>()) {
                    std::cout << transform->name << " has been defeated!\n";
                }
            }
        }
    }
}

/**
 * @brief Evaluates victory/defeat conditions based on entity status.
 */
void TurnSystem::checkBattleConditions() {
    bool playersAlive = false;
    bool enemiesAlive = false;

    for (const auto& entity : battleEntities) {
        if (auto* health = entity->getComponent<HealthComponent>()) {
            if (health->isAlive) {
                if (auto* transform = entity->getComponent<TransformComponent>()) {
                    if (transform->team == Team::PLAYER) {
                        playersAlive = true;
                    }
                    else {
                        enemiesAlive = true;
                    }
                }
            }
        }
    }

    if (!playersAlive) {
        setState(BattleState::DEFEAT);
        std::cout << "=== DEFEAT ===\n";
    }
    else if (!enemiesAlive) {
        setState(BattleState::VICTORY);
        std::cout << "=== VICTORY ===\n";
    }
}

/**
 * @brief Checks if the battle is currently active.
 */
bool TurnSystem::isBattleActive() const {
    return currentState != BattleState::VICTORY &&
        currentState != BattleState::DEFEAT;
}