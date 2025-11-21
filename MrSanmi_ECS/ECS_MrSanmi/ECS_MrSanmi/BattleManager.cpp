#include "BattleManager.h"
#include <iostream>
#include <random>
#include <thread>

/**
 * @brief Constructs a BattleManager and initializes core systems.
 */
BattleManager::BattleManager() {
    initializeSkills();
    setupEventHandlers();
}

/**
 * @brief Populates the skill registry with predefined combat abilities.
 */
void BattleManager::initializeSkills() {
    availableSkills["attack"] = SkillFactory::createAttackSkill();
    availableSkills["heal"] = SkillFactory::createHealSkill();
    availableSkills["fireball"] = Skill("Fireball",
        [](Entity& user, Entity& target) {
            auto* userStats = user.getComponent<HealthComponent>();
            auto* targetStats = target.getComponent<HealthComponent>();
            if (userStats && targetStats && userStats->stats.mana >= 15) {
                int damage = userStats->stats.attack * 2 - targetStats->stats.defense / 2;
                damage = std::max(1, damage);
                targetStats->stats.health -= damage;
                userStats->stats.mana -= 15;
                std::cout << "Fireball cast! " << damage << " fire damage to "
                    << target.getComponent<TransformComponent>()->name << ".\n";
            }
            else if (userStats && userStats->stats.mana < 15) {
                std::cout << "Not enough mana to cast Fireball!\n";
            }
        }, 15);
}

/**
 * @brief Configures event handlers for turn-based battle events.
 */
void BattleManager::setupEventHandlers() {
    // Event at the start of each turn
    turnSystem.subscribeToTurnStart([this]() {
        if (auto actor = turnSystem.getCurrentActor()) {
            if (auto* healthComp = actor->getComponent<HealthComponent>()) {
                // Mana regeneration each turn
                healthComp->stats.mana = std::min(healthComp->stats.maxMana, healthComp->stats.mana + 5);
            }
        }
        });
}

/**
 * @brief Factory method for entity creation with proper component initialization.
 */
Ref<Entity> BattleManager::createEntity(const std::string& name, Team team, const Stats& stats) {
    auto entity = std::make_shared<Entity>();
    entity->addComponent<TransformComponent>(name, team);
    entity->addComponent<HealthComponent>(stats);
    return entity;
}

/**
 * @brief Creates and registers a player-controlled entity.
 */
void BattleManager::addPlayer(const std::string& name, const Stats& stats) {
    auto entity = createEntity(name, Team::PLAYER, stats);
    allEntities.push_back(entity);
}

/**
 * @brief Creates and registers an enemy entity.
 */
void BattleManager::addEnemy(const std::string& name, const Stats& stats) {
    auto entity = createEntity(name, Team::ENEMY, stats);
    allEntities.push_back(entity);
}

/**
 * @brief Initializes and starts a new battle sequence.
 */
void BattleManager::startBattle() {
    turnSystem.initializeBattle(allEntities);
}

/**
 * @brief Executes a player-initiated action during their turn.
 */
void BattleManager::executePlayerAction(const std::string& skillName, Ref<Entity> target) {
    if (turnSystem.getCurrentState() != BattleState::PLAYER_CHOICE) {
        std::cout << "It's not time to act yet!\n";
        return;
    }

    auto actor = turnSystem.getCurrentActor();
    auto skillIt = availableSkills.find(skillName);

    if (skillIt == availableSkills.end()) {
        std::cout << "Skill not found!\n";
        return;
    }

    if (target && target->getComponent<HealthComponent>()->isAlive) {
        // Verify mana if the skill has cost
        auto* actorHealth = actor->getComponent<HealthComponent>();
        if (skillIt->second.getCost() > 0 && actorHealth->stats.mana < skillIt->second.getCost()) {
            std::cout << "Not enough mana! You need " << skillIt->second.getCost() << " mana.\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return;
        }

        skillIt->second.execute(*actor, *target);

        // Advance to next state
        turnSystem.setState(BattleState::ACTION_EXECUTE);
    }
}

/**
 * @brief Updates battle state and processes non-player entity actions.
 */
void BattleManager::update() {
    // AI update logic
    if (turnSystem.getCurrentState() == BattleState::ENEMY_THINKING) {
        std::cout << "The enemy is thinking...\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));

        auto enemy = turnSystem.getCurrentActor();
        auto players = getPlayerEntities();

        // Simple AI: attack player with lowest health
        Ref<Entity> target = nullptr;
        for (auto& player : players) {
            if (player->getComponent<HealthComponent>()->isAlive) {
                if (!target || player->getComponent<HealthComponent>()->stats.health <
                    target->getComponent<HealthComponent>()->stats.health) {
                    target = player;
                }
            }
        }

        if (target) {
            // 70% chance for basic attack, 30% for fireball if has mana
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 1.0);

            std::string skillToUse = "attack";
            auto* enemyHealth = enemy->getComponent<HealthComponent>();

            if (dis(gen) > 0.7 && enemyHealth->stats.mana >= 15) {
                skillToUse = "fireball";
            }

            std::cout << turnSystem.getCurrentActor()->getComponent<TransformComponent>()->name
                << " uses " << skillToUse << "!\n";

            availableSkills[skillToUse].execute(*enemy, *target);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        turnSystem.setState(BattleState::ACTION_EXECUTE);
    }
}

/**
 * @brief Filters entities that are currently active in combat.
 */
std::vector<Ref<Entity>> BattleManager::getAliveEntities() const {
    std::vector<Ref<Entity>> alive;
    for (const auto& entity : allEntities) {
        if (entity->getComponent<HealthComponent>()->isAlive) {
            alive.push_back(entity);
        }
    }
    return alive;
}

/**
 * @brief Filters entities belonging to the enemy team.
 */
std::vector<Ref<Entity>> BattleManager::getEnemyEntities() const {
    std::vector<Ref<Entity>> enemies;
    for (const auto& entity : allEntities) {
        if (auto* transform = entity->getComponent<TransformComponent>()) {
            if (transform->team == Team::ENEMY &&
                entity->getComponent<HealthComponent>()->isAlive) {
                enemies.push_back(entity);
            }
        }
    }
    return enemies;
}

/**
 * @brief Filters entities belonging to the player team.
 */
std::vector<Ref<Entity>> BattleManager::getPlayerEntities() const {
    std::vector<Ref<Entity>> players;
    for (const auto& entity : allEntities) {
        if (auto* transform = entity->getComponent<TransformComponent>()) {
            if (transform->team == Team::PLAYER &&
                entity->getComponent<HealthComponent>()->isAlive) {
                players.push_back(entity);
            }
        }
    }
    return players;
}