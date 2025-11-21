#pragma once
#include "GameTypes.h"
#include "Entity.h"
#include <functional>
#include <vector>
#include <string>
#include <iostream>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: Skill.h
 * @brief: Defines the Skill system for turn-based combat actions using lambda-based effects.
 * @details: Implements a flexible skill system where combat abilities are defined as composable lambda functions,
 *           enabling easy creation of new skills without modifying core combat logic.
 */

 /**
  * @brief Encapsulates a combat skill with name, resource cost, and executable effect.
  * @details Skills use the Command Pattern to encapsulate combat actions as first-class objects.
  *          Each skill contains a lambda function that defines its gameplay behavior when executed.
  */
class Skill {
public:
    /**
     * @brief Default constructor for Skill.
     * @details Creates an empty, non-functional skill. Primarily for container compatibility.
     */
    Skill() = default;

    /**
     * @brief Constructs a fully-defined Skill with specified properties.
     * @param skillName The display name used in UI and logging.
     * @param skillEffect Lambda function defining the skill's gameplay logic and side effects.
     * @param manaCost Resource cost required to execute this skill (default: 0).
     * @note The skillEffect lambda should handle its own validation and state modifications.
     */
    Skill(const std::string& skillName,
        std::function<void(Entity&, Entity&)> skillEffect,
        int manaCost = 0)
        : name(skillName), effect(std::move(skillEffect)), cost(manaCost) {
    }

    /**
     * @brief Executes this skill's effect on the specified target entity.
     * @param user The entity initiating the skill (consumes resources).
     * @param target The entity receiving the skill's effects.
     * @note Performs null-check on the effect lambda before execution.
     * @warning Both entities must have required components for the skill to function properly.
     */
    void execute(Entity& user, Entity& target) {
        if (effect) {
            effect(user, target);
        }
    }

    /**
     * @brief Retrieves the display name of the skill.
     * @return Constant reference to the skill's name string.
     */
    const std::string& getName() const { return name; }

    /**
     * @brief Retrieves the resource cost required to use this skill.
     * @return The mana cost as an integer value.
     */
    int getCost() const { return cost; }

private:
    std::string name;                                   ///< Display name for UI and debugging
    std::function<void(Entity&, Entity&)> effect;      ///< Lambda defining skill behavior
    int cost{ 0 };                                       ///< Mana resource cost for execution
};

/**
 * @brief Factory class for creating predefined Skill instances with balanced attributes.
 * @details Implements the Factory Method Pattern to centralize skill creation and ensure
 *          consistent balancing across all combat abilities.
 */
class SkillFactory {
public:
    /**
     * @brief Creates a basic physical attack skill.
     * @return Skill instance representing a standard attack.
     * @details Calculates damage based on attacker's attack stat and target's defense.
     *          Guarantees minimum 1 damage and handles entity defeat state.
     */
    static Skill createAttackSkill() {
        return Skill("Basic Attack", [](Entity& user, Entity& target) {
            auto* userStats = user.getComponent<HealthComponent>();
            auto* targetStats = target.getComponent<HealthComponent>();
            if (userStats && targetStats && targetStats->isAlive) {
                int damage = std::max(1, userStats->stats.attack - targetStats->stats.defense / 2);
                targetStats->stats.health -= damage;
                std::cout << "Basic attack! " << damage << " damage to "
                    << target.getComponent<TransformComponent>()->name << ".\n";

                if (targetStats->stats.health <= 0) {
                    targetStats->isAlive = false;
                    std::cout << target.getComponent<TransformComponent>()->name << " has been defeated!\n";
                }
            }
            });
    }

    /**
     * @brief Creates a healing skill that restores health to target entity.
     * @return Skill instance representing a healing ability.
     * @details Restores 33% of target's maximum health, consuming 10 mana.
     *          Includes mana validation and heal amount calculation.
     */
    static Skill createHealSkill() {
        return Skill("Heal", [](Entity& user, Entity& target) {
            auto* userStats = user.getComponent<HealthComponent>();
            auto* targetStats = target.getComponent<HealthComponent>();
            if (userStats && targetStats && userStats->stats.mana >= 10) {
                int heal = targetStats->stats.maxHealth / 3;
                int oldHealth = targetStats->stats.health;
                targetStats->stats.health = std::min(targetStats->stats.maxHealth, targetStats->stats.health + heal);
                int actualHeal = targetStats->stats.health - oldHealth;
                userStats->stats.mana -= 10;
                std::cout << "Heal performed! " << actualHeal << " health restored to "
                    << target.getComponent<TransformComponent>()->name << ".\n";
            }
            else if (userStats && userStats->stats.mana < 10) {
                std::cout << "Not enough mana to heal!\n";
            }
            }, 10);
    }

    /**
     * @brief Creates a fire elemental skill dealing high damage.
     * @return Skill instance representing a fire spell.
     * @details Deals double attack damage with mana cost. Includes mana validation.
     */
    static Skill createFireballSkill() {
        return Skill("Fireball", [](Entity& user, Entity& target) {
            auto* userStats = user.getComponent<HealthComponent>();
            auto* targetStats = target.getComponent<HealthComponent>();
            if (userStats && targetStats && userStats->stats.mana >= 15) {
                int damage = userStats->stats.attack * 2 - targetStats->stats.defense / 2;
                damage = std::max(1, damage);
                targetStats->stats.health -= damage;
                userStats->stats.mana -= 15;
                std::cout << "Fireball cast! " << damage << " fire damage to "
                    << target.getComponent<TransformComponent>()->name << ".\n";

                if (targetStats->stats.health <= 0) {
                    targetStats->isAlive = false;
                    std::cout << target.getComponent<TransformComponent>()->name << " has been defeated!\n";
                }
            }
            else if (userStats && userStats->stats.mana < 15) {
                std::cout << "Not enough mana to cast Fireball!\n";
            }
            }, 15);
    }
};