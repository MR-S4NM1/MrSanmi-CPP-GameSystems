#include "BattleManager.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: main.cpp
 * @brief: Main game loop and entry point for the Turn-Based Combat System.
 * @details: Implements the core game loop with update-render-input processing pattern.
 *           Provides console-based UI for battle interactions and state visualization.
 */

 /**
  * @brief Main game class orchestrating the battle experience.
  * @details Manages the complete game lifecycle from initialization to game over states.
  *          Implements the classic game loop pattern with separation of update, render, and input processing.
  */
class Game {
private:
    BattleManager battleManager;    ///< Core battle system controller
    bool gameRunning{ true };       ///< Flag indicating if the game should continue running

public:
    /**
     * @brief Initializes game state and configures battle participants.
     * @details Sets up player and enemy entities with balanced statistics for demo purposes.
     */
    void initialize() {
        std::cout << "INITIALIZING COMBAT SYSTEM\n";

        // Create main player
        Stats playerStats(120, 18, 8, 15, 60);
        battleManager.addPlayer("Hero", playerStats);

        // Create companion
        Stats companionStats(90, 12, 6, 12, 40);
        battleManager.addPlayer("Mage", companionStats);

        // Create enemies with reduced health for faster demo
        Stats goblinStats(10, 14, 4, 10, 20);
        battleManager.addEnemy("Goblin", goblinStats);

        Stats orcStats(25, 20, 10, 8, 30);
        battleManager.addEnemy("Orc", orcStats);

        Stats bossStats(50, 25, 12, 6, 50);
        battleManager.addEnemy("Goblin Boss", bossStats);

        std::cout << "Battle configured! 2 heroes vs 3 enemies.\n\n";
    }

    /**
     * @brief Executes the main game loop until battle completion or player surrender.
     */
    void run() {
        battleManager.startBattle();

        while (gameRunning && battleManager.isBattleActive()) {
            update();
            render();
            processInput();
        }

        showGameOver();
    }

private:
    /**
     * @brief Updates game state through the battle manager.
     */
    void update() {
        battleManager.update();
    }

    /**
     * @brief Renders the current game state to the console.
     */
    void render() {
        clearScreen();
        displayHeader();

        displayBattlefield();
        displayCurrentTurnInfo();

        if (battleManager.getBattleState() == BattleState::PLAYER_CHOICE) {
            displayActionMenu();
        }
        else if (battleManager.getBattleState() == BattleState::ENEMY_THINKING) {
            std::cout << "The enemy is planning their move...\n";
        }
        else if (battleManager.getBattleState() == BattleState::ACTION_EXECUTE) {
            std::cout << "Executing action...\n";
        }
    }

    /**
     * @brief Processes player input during appropriate battle states.
     */
    void processInput() {
        if (battleManager.getBattleState() == BattleState::PLAYER_CHOICE) {
            handlePlayerInput();
        }
    }

    /**
     * @brief Clears the console screen for fresh rendering.
     */
    void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    /**
     * @brief Displays the game header and title.
     */
    void displayHeader() {
        std::cout << "=========================================\n";
        std::cout << "          C++ COMBAT SYSTEM         \n";
        std::cout << "=========================================\n\n";
    }

    /**
     * @brief Renders the battlefield showing all entities and their status.
     */
    void displayBattlefield() {
        auto players = battleManager.getPlayers();
        auto enemies = battleManager.getEnemies();

        std::cout << "=== ALLIES ===\n";
        for (size_t i = 0; i < players.size(); ++i) {
            displayEntityInfo(players[i], i, true);
        }

        std::cout << "\n=== ENEMIES ===\n";
        for (size_t i = 0; i < enemies.size(); ++i) {
            displayEntityInfo(enemies[i], i, false);
        }
        std::cout << "\n";
    }

    /**
     * @brief Displays detailed information for a single entity.
     * @param entity The entity to display information for.
     * @param index The positional index of the entity (for potential UI formatting).
     * @param isPlayer Flag indicating if the entity belongs to the player team.
     */
    void displayEntityInfo(const Ref<Entity>& entity, size_t index, bool isPlayer) {
        auto* transform = entity->getComponent<TransformComponent>();
        auto* health = entity->getComponent<HealthComponent>();

        if (!transform || !health) return;

        std::string teamIcon = isPlayer ? "[ALLY]" : "[ENEMY]";
        std::string healthBar = generateHealthBar(health->stats.health, health->stats.maxHealth);
        std::string status = health->isAlive ? "ALIVE" : "DEAD";

        std::cout << status << " " << teamIcon << " " << transform->name << "\n";
        std::cout << "   HP: " << healthBar << " " << health->stats.health << "/" << health->stats.maxHealth << "\n";
        std::cout << "   Mana: " << health->stats.mana << "/" << health->stats.maxMana;

        if (isPlayer) {
            std::cout << " | ATK: " << health->stats.attack << " | DEF: " << health->stats.defense;
        }
        std::cout << "\n\n";
    }

    /**
     * @brief Generates a visual health bar representation.
     * @param current Current health value.
     * @param max Maximum health value.
     * @return String representing the health bar visualization.
     */
    std::string generateHealthBar(int current, int max) {
        const int barWidth = 20;
        float percentage = static_cast<float>(current) / max;
        int filledWidth = static_cast<int>(barWidth * percentage);

        std::string bar = "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < filledWidth) {
                bar += "O";  // Filled character
            }
            else {
                bar += "X";  // Empty character
            }
        }
        bar += "]";

        return bar;
    }

    /**
     * @brief Displays information about the current turn and active entity.
     */
    void displayCurrentTurnInfo() {
        auto currentActor = battleManager.getCurrentActor();
        if (!currentActor) return;

        auto* transform = currentActor->getComponent<TransformComponent>();
        auto* health = currentActor->getComponent<HealthComponent>();

        if (transform && health && health->isAlive) {
            std::cout << ">>> CURRENT TURN: " << transform->name;
            std::cout << " | State: ";

            switch (battleManager.getBattleState()) {
            case BattleState::PLAYER_CHOICE: std::cout << "Choosing action"; break;
            case BattleState::ENEMY_THINKING: std::cout << "Thinking..."; break;
            case BattleState::ACTION_EXECUTE: std::cout << "Executing action"; break;
            default:
                std::cout << "Other (" << static_cast<int>(battleManager.getBattleState()) << ")";
                break;
            }
            std::cout << "\n\n";
        }
    }

    /**
     * @brief Displays the available action menu for player input.
     */
    void displayActionMenu() {
        auto currentActor = battleManager.getCurrentActor();
        if (!currentActor) return;

        std::cout << "=== AVAILABLE ACTIONS ===\n";

        // Display skills
        int skillIndex = 1;
        for (const auto& skillPair : battleManager.getSkills()) {
            std::cout << skillIndex << ". " << skillPair.first;
            if (skillPair.second.getCost() > 0) {
                std::cout << " [" << skillPair.second.getCost() << " mana]";
            }
            std::cout << "\n";
            skillIndex++;
        }

        std::cout << skillIndex << ". View detailed status\n";
        std::cout << "0. Surrender\n";
        std::cout << "\nSelect an option: ";
    }

    /**
     * @brief Handles and validates player input during choice states.
     */
    void handlePlayerInput() {
        int choice;
        std::cin >> choice;

        // Validate input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Please try again.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return;
        }

        int skillCount = battleManager.getSkills().size();

        if (choice == 0) {
            // Surrender
            gameRunning = false;
            return;
        }
        else if (choice == skillCount + 1) {
            // View detailed status
            showDetailedStatus();
            return;
        }
        else if (choice >= 1 && choice <= skillCount) {
            // Execute skill
            executePlayerSkill(choice);
        }
        else {
            std::cout << "Invalid option. Please try again.\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    /**
     * @brief Executes the selected player skill with automatic target selection.
     * @param skillChoice The numerical index of the selected skill.
     */
    void executePlayerSkill(int skillChoice) {
        // Convert number to skill name
        std::string skillName;
        int currentIndex = 1;
        for (const auto& skillPair : battleManager.getSkills()) {
            if (currentIndex == skillChoice) {
                skillName = skillPair.first;
                break;
            }
            currentIndex++;
        }

        auto enemies = battleManager.getEnemies();
        auto players = battleManager.getPlayers();

        Ref<Entity> target = nullptr;

        if (skillName == "heal") {
            // For healing, select ally with lowest health
            for (auto& player : players) {
                if (player->getComponent<HealthComponent>()->isAlive) {
                    if (!target || player->getComponent<HealthComponent>()->stats.health <
                        target->getComponent<HealthComponent>()->stats.health) {
                        target = player;
                    }
                }
            }
        }
        else {
            // For attacks, select first alive enemy
            for (auto& enemy : enemies) {
                if (enemy->getComponent<HealthComponent>()->isAlive) {
                    target = enemy;
                    break;
                }
            }
        }

        if (target) {
            battleManager.executePlayerAction(skillName, target);

            // Show action feedback
            auto* actorTransform = battleManager.getCurrentActor()->getComponent<TransformComponent>();
            auto* targetTransform = target->getComponent<TransformComponent>();

            std::cout << "\n " << actorTransform->name << " uses " << skillName
                << " on " << targetTransform->name << "!\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    /**
     * @brief Displays detailed status information for all entities.
     */
    void showDetailedStatus() {
        clearScreen();
        std::cout << "=== DETAILED STATUS ===\n\n";

        auto entities = battleManager.getEntities();
        for (const auto& entity : entities) {
            auto* transform = entity->getComponent<TransformComponent>();
            auto* health = entity->getComponent<HealthComponent>();

            if (transform && health) {
                std::string team = (transform->team == Team::PLAYER) ? "Ally" : "Enemy";
                std::string status = health->isAlive ? "Alive" : "Defeated";

                std::cout << "[" << team << "] " << transform->name << " - " << status << "\n";
                std::cout << "   Health: " << health->stats.health << "/" << health->stats.maxHealth << "\n";
                std::cout << "   Mana: " << health->stats.mana << "/" << health->stats.maxMana << "\n";
                std::cout << "   Attack: " << health->stats.attack << " | Defense: " << health->stats.defense;
                std::cout << " | Speed: " << health->stats.speed << "\n\n";
            }
        }

        std::cout << "Press Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    /**
     * @brief Displays game over screen with victory/defeat message.
     */
    void showGameOver() {
        clearScreen();

        if (!battleManager.isBattleActive()) {
            if (battleManager.getBattleState() == BattleState::VICTORY) {
                std::cout << "=========================================\n";
                std::cout << "             VICTORY            \n";
                std::cout << "=========================================\n";
                std::cout << "You have defeated all enemies!\n";
                std::cout << "Congratulations, hero!\n";
            }
            else {
                std::cout << "=========================================\n";
                std::cout << "             DEFEAT             \n";
                std::cout << "=========================================\n";
                std::cout << "All your heroes have fallen in battle...\n";
                std::cout << "The adventure ends here.\n";
            }
        }
        else {
            std::cout << "Game ended.\n";
        }

        std::cout << "\nThank you for playing!\n";
    }
};

/**
 * @brief Application entry point.
 * @return Exit status code (0 for successful execution).
 */
int main() {
    Game game;
    game.initialize();
    game.run();
    return 0;
}