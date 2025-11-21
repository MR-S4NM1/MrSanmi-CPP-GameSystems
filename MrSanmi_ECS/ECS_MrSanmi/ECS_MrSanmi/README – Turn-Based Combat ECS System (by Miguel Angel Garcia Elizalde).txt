README – Turn-Based Combat ECS System

Author: Miguel Ángel García Elizalde
Date: October 2025
Language: C++17
Project Type: Technical Gameplay Prototype / ECS Demo

---

Overview

This project implements a modular Entity–Component–System (ECS) architecture for a turn-based combat framework in C++.

The goal was to design a clean, extensible, and engine-friendly system that could serve as the foundation for RPG-style combat while demonstrating professional-level code structure and design patterns.

The system was built entirely from scratch (no external libraries beyond the STL) to show strong understanding of C++ memory management, game architecture patterns, and turn-based gameplay logic.

---

Core Features

Entity–Component–System Architecture

- Entities are lightweight containers with no logic — they simply aggregate components.
- Components hold pure data such as stats, team affiliation, and battle state.
- Systems perform all logic.

  - TurnSystem manages the combat flow and battle state machine.
  - BattleManager acts as a facade, coordinating all high-level operations (entities, skills, and turns).

Battle Flow & FSM

- Uses a finite state machine (BattleState) to control combat progression:

  - TURN_START → PLAYER_CHOICE → ENEMY_THINKING → ACTION_EXECUTE → TURN_END
- Supports automatic state transitions and event callbacks for extensibility.
- Integrates a simple but functional enemy AI that chooses between attacks and spells with weighted randomness.

Skill System (Command Pattern)

- Each skill is represented as an independent object encapsulating its logic through lambdas.
- Includes a Skill Factory** to generate predefined actions:

  - Basic Attack – physical damage based on attack and defense.
  - Heal – restores health using a mana cost.
  - Fireball – high-damage elemental attack consuming mana.
- New skills can be added without modifying the core battle logic.

Turn System & Event Handling

- Uses a priority queue to determine turn order dynamically (based on speed and team).
- Event subscription system allows hooking custom logic into the start or end of each turn.
- Implements automatic mana regeneration per turn as an example of event-driven mechanics.

Console-Based Demo

- Includes a fully interactive main.cpp that serves as a playable demo.
- Displays a simple UI showing HP, Mana, and turn information in real time.
- Features a companion character, multiple enemies, and automatic AI behavior.

---

Design Patterns Used

- Entity–Component–System (ECS) – modular and data-oriented entity design.
- Facade Pattern – BattleManager simplifies interaction between systems.
- Command Pattern – each Skill encapsulates its execution logic.
- Factory Method – centralizes skill creation and balancing.
- Finite State Machine (FSM)** – for structured battle flow.
- Event System (Observer Pattern)** – for decoupled updates between systems.

---

Technical Highlights

- Smart Pointers (Ref, Scope) ensure safe memory management.
- Type-safe component retrieval using std::type_index.
- No hard-coded logic; the architecture supports data-driven gameplay.
- Fully documented codebase using Doxygen-style comments (@brief, @param, @example).
- Extremely modular: the combat layer can be ported to Unreal Engine with minimal changes.

---

Example: Creating and Registering Entities

Stats heroStats(120, 18, 8, 15, 60);
battleManager.addPlayer("Hero", heroStats);

Stats goblinStats(25, 14, 5, 10, 30);
battleManager.addEnemy("Goblin", goblinStats);

battleManager.startBattle();

Example: Skill Factory

availableSkills["attack"] = SkillFactory::createAttackSkill();
availableSkills["heal"]   = SkillFactory::createHealSkill();
availableSkills["fireball"] = SkillFactory::createFireballSkill();

---

How to Run

1. Clone the repository and open it in your C++ environment (Visual Studio, CLion, etc.).
2. Compile all .cpp files.
3. Run the executable from the console.
4. Follow on-screen prompts to execute skills and progress through the battle.

---

File Structure

ECS_Mr_Sanmi
│
├── BattleManager.h       # High-level facade coordinating skills, entities, and turn flow
├── TurnSystem.h          # Finite State Machine, turn queue, and event handling
├── Skill.h               # Skill definitions, Command Pattern, and SkillFactory
├── Entity.h              # Generic entity container with type-indexed component storage
├── Component.h           # Component definitions (Transform, Health, Battle)
├── GameTypes.h           # Core enums, Stats struct, smart pointer aliases
├── main.cpp              # Fully interactive console demo and game loop
└── README.md             # Project overview, documentation, and instructions

---

Final Notes

This project represents a complete gameplay system prototype written with an emphasis on clarity, extensibility, and maintainability.
It’s designed as a demonstration of both engineering discipline and creative problem-solving — merging game design logic with professional C++ architecture.