#pragma once
#include <iostream>
using std::cout;

/**
 * @class Stack
 * @brief A LIFO (Last-In-First-Out) stack implementation using linked list
 * @tparam T The type of elements stored in the stack
 * @author Miguel Ángel García Elizalde
 * @date 2024-05-07
 *
 * This stack implementation provides constant time O(1) operations for push, pop, and peek.
 * It uses a singly linked list structure where elements are added and removed from the top.
 */
template<class T> class Stack {
private:
    /**
     * @class Node
     * @brief Internal class representing a node in the linked list
     */
    class Node {
        T m_data;           ///< Data stored in the node
        Node* m_pNext = nullptr;  ///< Pointer to the next node in the stack

        friend class Stack;
    };

    Node* m_pRoot = nullptr;  ///< Pointer to the top node of the stack
    Node* m_pLast = nullptr;  ///< Pointer to the bottom node of the stack (for potential extensions)
    size_t m_size = 0;        ///< Number of elements in the stack

    Node* generateNode(T t_data);

public:
    // Constructors & Destructor
    Stack();
    Stack(T t_data);
    Stack(const Stack<T>& other);

    // Accessors
    void traverse();
    T peek();
    bool empty();
    size_t size() const { return m_size; }  ///< Returns the number of elements in the stack

    // Mutators
    void enstack(T t_data);  // push
    T destack();             // pop
};

// =============================================================================
// PRIVATE METHOD IMPLEMENTATIONS
// =============================================================================

/**
 * @brief Creates a new node with the specified data
 * @tparam T Type of data to store in the node
 * @param t_data Data to be stored in the new node
 * @return Node* Pointer to the newly created node
 */
template <class T>
typename Stack<T>::Node* Stack<T>::generateNode(T t_data) {
    Node* pNewNode = new Node();
    pNewNode->m_data = t_data;
    pNewNode->m_pNext = nullptr;
    return pNewNode;
}

// =============================================================================
// CONSTRUCTORS & DESTRUCTOR
// =============================================================================

/**
 * @brief Default constructor - creates an empty stack
 * @tparam T Type of elements to be stored in the stack
 */
template <class T>
Stack<T>::Stack() {
    // Empty stack initialization
}

/**
 * @brief Constructor that creates a stack with one initial element
 * @tparam T Type of the initial element
 * @param t_data Data for the initial stack element
 */
template <class T>
Stack<T>::Stack(T t_data) {
    m_pRoot = generateNode(t_data);
    m_pLast = m_pRoot;
    m_size = 1;
}

/**
 * @brief Copy constructor - creates a stack as a copy of another stack
 * @tparam T Type of elements stored in the stack
 * @param other Stack to be copied
 * @note This implementation currently creates an empty stack.
 *       Full copy functionality can be added as needed.
 */
template <class T>
Stack<T>::Stack(const Stack<T>& other) {
    // Copy constructor implementation can be expanded as needed
    // Currently creates an empty stack to avoid memory issues
}

// =============================================================================
// ACCESSOR METHODS
// =============================================================================

/**
 * @brief Prints all elements in the stack from top to bottom
 * @tparam T Type of elements in the stack
 *
 * This method traverses the stack and prints each element on a new line.
 * The output shows the stack contents in LIFO order (top element first).
 */
template <class T>
void Stack<T>::traverse() {
    Node* current = m_pRoot;
    while (current) {
        cout << current->m_data << "\n";
        current = current->m_pNext;
    }
}

/**
 * @brief Returns the top element of the stack without removing it
 * @tparam T Type of elements in the stack
 * @return T Reference to the top element of the stack
 * @throws std::runtime_error if the stack is empty
 *
 * This method provides read-only access to the top element.
 * Useful for inspecting the next element to be popped.
 */
template <class T>
T Stack<T>::peek() {
    if (!m_pRoot) {
        throw std::runtime_error("Cannot peek from an empty stack");
    }
    return m_pRoot->m_data;
}

/**
 * @brief Checks if the stack is empty
 * @tparam T Type of elements in the stack
 * @return bool True if the stack contains no elements, false otherwise
 */
template <class T>
bool Stack<T>::empty() {
    return !m_pRoot;
}

// =============================================================================
// MUTATOR METHODS
// =============================================================================

/**
 * @brief Pushes an element onto the top of the stack
 * @tparam T Type of the element to push
 * @param t_data Data to be pushed onto the stack
 *
 * This operation has O(1) time complexity as it only modifies the top pointer
 * and adds a new node at the beginning of the linked list.
 */
template <class T>
void Stack<T>::enstack(T t_data) {
    if (!m_pRoot) {
        // Stack is empty - create first element
        m_pRoot = generateNode(t_data);
        m_pLast = m_pRoot;
        m_size = 1;
        return;
    }

    // Add new element to the top of the stack
    Node* newNode = generateNode(t_data);
    newNode->m_pNext = m_pRoot;
    m_pRoot = newNode;
    m_size++;
}

/**
 * @brief Removes and returns the top element from the stack
 * @tparam T Type of elements in the stack
 * @return T The element that was at the top of the stack
 * @throws std::domain_error if the stack is empty
 *
 * This operation has O(1) time complexity as it only modifies the top pointer
 * and removes the first node from the linked list.
 * The method follows RAII principles by properly deallocating memory.
 */
template <class T>
T Stack<T>::destack() {
    if (!m_pRoot) {
        throw std::domain_error("No elements in the stack");
    }

    T returnValue = m_pRoot->m_data;

    if (m_pRoot == m_pLast) {
        // Stack has only one element
        delete m_pRoot;
        m_pRoot = nullptr;
        m_pLast = nullptr;
        m_size = 0;
        return returnValue;
    }

    // Stack has multiple elements
    Node* nodeToDelete = m_pRoot;
    m_pRoot = m_pRoot->m_pNext;
    delete nodeToDelete;
    m_size--;

    return returnValue;
}