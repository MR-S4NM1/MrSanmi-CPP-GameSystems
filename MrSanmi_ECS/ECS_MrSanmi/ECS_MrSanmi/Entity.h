#pragma once
#include "Component.h"
#include <typeindex>
#include <unordered_map>

/**
 * @Author: Miguel Angel Garcia Elizalde
 * @Date: October, 2025
 * @file: Entity.h
 * @brief: Core Entity class implementing the Entity-Component-System (ECS) pattern.
 * @details: Entities are generic containers for components, enabling flexible and data-oriented design.
 */

 /**
  * @brief Container class for components in the Entity-Component-System architecture.
  * @details Entities are simple identifiers that aggregate components. They contain no logic themselves,
  *          but serve as handles to collections of data components that systems operate upon.
  *          This implementation uses type-safe component storage with runtime type identification.
  */
class Entity {
private:
    /// Type-indexed map storing component instances with ownership
    std::unordered_map<std::type_index, std::unique_ptr<IComponent>> components;

public:
    /**
     * @brief Adds a component of specified type to the entity.
     * @tparam T The component type to add (must inherit from IComponent).
     * @tparam Args Variadic template parameter pack for component constructor arguments.
     * @param args Forwarded arguments to the component's constructor.
     * @note Overwrites existing component of the same type if present.
     * @example
     * @code
     * entity.addComponent<TransformComponent>("Player", Team::PLAYER);
     * entity.addComponent<HealthComponent>(Stats{100, 10, 5, 15, 50});
     * @endcode
     */
    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[std::type_index(typeid(T))] =
            std::make_unique<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Retrieves a component of specified type from the entity.
     * @tparam T The component type to retrieve (must inherit from IComponent).
     * @return Raw pointer to the component if found, nullptr otherwise.
     * @warning Returns non-owning pointer. Component lifetime is managed by the entity.
     * @example
     * @code
     * auto* health = entity.getComponent<HealthComponent>();
     * if (health && health->isAlive) { /* process living entity *\/ }
     * @endcode
     */
    template<typename T>
    T* getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        return it != components.end() ?
            static_cast<T*>(it->second.get()) : nullptr;
    }

    /**
     * @brief Checks if the entity possesses a component of specified type.
     * @tparam T The component type to check for existence.
     * @return True if the component is present, false otherwise.
     * @example
     * @code
     * if (entity.hasComponent<TransformComponent>()) {
     *     // Entity has transform data
     * }
     * @endcode
     */
    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    /**
     * @brief Removes all components from the entity.
     * @details Resets the entity to an empty state. Useful for entity pooling or cleanup.
     * @warning Invalidates all previously obtained component pointers.
     */
    void clearComponents() {
        components.clear();
    }

    /**
     * @brief Checks if the entity contains any components.
     * @return True if the entity has no components, false if it contains one or more components.
     * @note Empty entities are typically invalid and should not be processed by systems.
     */
    bool isEmpty() const {
        return components.empty();
    }
};