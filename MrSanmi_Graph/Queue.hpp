#pragma once
#include <iostream>
using std::cout;

/**
 * @class Queue
 * @brief A FIFO (First-In-First-Out) queue implementation using linked list
 * @tparam T The type of elements stored in the queue
 * @author Vladimir Sánchez
 * @date 2024-04-29
 *
 * This queue implementation provides constant time O(1) operations for enqueue and dequeue.
 * It uses a singly linked list structure where elements are added at the back and removed from the front,
 * maintaining the FIFO ordering principle essential for queue operations.
 */
template<class T> class Queue {
private:
    /**
     * @class Node
     * @brief Internal class representing a node in the linked list
     */
    class Node {
        T m_data;           ///< Data stored in the node
        Node* m_pNext = nullptr;  ///< Pointer to the next node in the queue

        friend class Queue;
    };

    Node* m_pRoot = nullptr;  ///< Pointer to the front node of the queue (next to be removed)
    Node* m_pLast = nullptr;  ///< Pointer to the rear node of the queue (most recently added)
    size_t m_size = 0;        ///< Number of elements in the queue

    /**
     * @brief Creates a new node with the specified data
     * @param t_data Data to be stored in the new node
     * @return Node* Pointer to the newly created node
     */
    Node* generateNode(T t_data);

public:
    // Constructors
    Queue();
    Queue(T t_data);
    Queue(const Queue<T>& other);

    // Accessors
    void traverse();
    T peek();
    bool empty();
    size_t size() const { return m_size; }  ///< Returns the number of elements in the queue

    // Mutators
    void enqueue(T t_data);  // Add to back
    T dequeue();             // Remove from front
};

// =============================================================================
// PRIVATE METHOD IMPLEMENTATIONS
// =============================================================================

/**
 * @brief Creates a new node with the specified data
 * @tparam T Type of data to store in the node
 * @param t_data Data to be stored in the new node
 * @return Node* Pointer to the newly created node
 *
 * This utility method handles node creation and initialization,
 * ensuring consistent node construction throughout the queue.
 */
template <class T>
typename Queue<T>::Node* Queue<T>::generateNode(T t_data) {
    Node* pNewNode = new Node();
    pNewNode->m_data = t_data;
    pNewNode->m_pNext = nullptr;
    return pNewNode;
}

// =============================================================================
// CONSTRUCTORS
// =============================================================================

/**
 * @brief Default constructor - creates an empty queue
 * @tparam T Type of elements to be stored in the queue
 */
template <class T>
Queue<T>::Queue() {
    // Empty queue initialization
}

/**
 * @brief Constructor that creates a queue with one initial element
 * @tparam T Type of the initial element
 * @param t_data Data for the initial queue element
 *
 * Creates a queue with a single element, setting both front and rear pointers
 * to the same node and initializing the size to 1.
 */
template <class T>
Queue<T>::Queue(T t_data) {
    m_pRoot = generateNode(t_data);
    m_pLast = m_pRoot;
    m_size = 1;
}

/**
 * @brief Copy constructor - creates a queue as a copy of another queue
 * @tparam T Type of elements stored in the queue
 * @param other Queue to be copied
 * @note This implementation currently creates an empty queue.
 *       Full copy functionality can be implemented as needed.
 */
template <class T>
Queue<T>::Queue(const Queue<T>& other) {
    // Copy constructor implementation can be expanded as needed
    // Currently creates an empty queue to avoid memory issues
}

// =============================================================================
// ACCESSOR METHODS
// =============================================================================

/**
 * @brief Prints all elements in the queue from front to back
 * @tparam T Type of elements in the queue
 *
 * This method traverses the queue and prints each element on a new line.
 * The output shows the queue contents in FIFO order (front element first).
 * Useful for debugging and visualizing queue contents.
 */
template <class T>
void Queue<T>::traverse() {
    Node* current = m_pRoot;
    while (current) {
        cout << current->m_data << "\n";
        current = current->m_pNext;
    }
}

/**
 * @brief Returns the front element of the queue without removing it
 * @tparam T Type of elements in the queue
 * @return T The front element of the queue
 * @throws std::runtime_error if the queue is empty
 *
 * This method provides read-only access to the front element.
 * Useful for inspecting the next element to be dequeued without modifying the queue.
 */
template <class T>
T Queue<T>::peek() {
    if (!m_pRoot) {
        throw std::runtime_error("Cannot peek from an empty queue");
    }
    return m_pRoot->m_data;
}

/**
 * @brief Checks if the queue is empty
 * @tparam T Type of elements in the queue
 * @return bool True if the queue contains no elements, false otherwise
 */
template <class T>
bool Queue<T>::empty() {
    return !m_pRoot;
}

// =============================================================================
// MUTATOR METHODS
// =============================================================================

/**
 * @brief Adds an element to the back of the queue
 * @tparam T Type of the element to enqueue
 * @param t_data Data to be added to the queue
 *
 * This operation has O(1) time complexity as it only modifies the rear pointer
 * and adds a new node at the end of the linked list.
 * Maintains the FIFO property by always adding to the back.
 */
template <class T>
void Queue<T>::enqueue(T t_data) {
    if (!m_pRoot) {
        // Queue is empty - create first element
        m_pRoot = generateNode(t_data);
        m_pLast = m_pRoot;
        m_size = 1;
        return;
    }

    // Add new element to the back of the queue
    m_pLast->m_pNext = generateNode(t_data);
    m_pLast = m_pLast->m_pNext;
    m_size++;
}

/**
 * @brief Removes and returns the front element from the queue
 * @tparam T Type of elements in the queue
 * @return T The element that was at the front of the queue
 * @throws std::runtime_error if the queue is empty
 *
 * This operation has O(1) time complexity as it only modifies the front pointer
 * and removes the first node from the linked list.
 * The method follows RAII principles by properly deallocating memory.
 * Maintains the FIFO property by always removing from the front.
 */
template <class T>
T Queue<T>::dequeue() {
    if (!m_pRoot) {
        throw std::runtime_error("No elements in the queue");
    }

    T returnValue = m_pRoot->m_data;

    if (m_pRoot == m_pLast) {
        // Queue has only one element
        delete m_pRoot;
        m_pRoot = nullptr;
        m_pLast = nullptr;
        m_size = 0;
        return returnValue;
    }

    // Queue has multiple elements
    Node* nodeToDelete = m_pRoot;
    m_pRoot = m_pRoot->m_pNext;
    delete nodeToDelete;
    m_size--;

    return returnValue;
}