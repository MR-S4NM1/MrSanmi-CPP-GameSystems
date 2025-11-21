#pragma once
#include <iostream>
using std::cout;

/**
 * @class DoubleLinkedList
 * @brief A doubly linked list implementation supporting bidirectional traversal
 * @tparam T The type of elements stored in the list
 * @author Miguel Ángel García Elizalde
 * @date 2024-04-08
 *
 * This implementation provides efficient insertion and deletion from both ends (O(1)),
 * bidirectional traversal, and random access via indexing. Each node contains pointers
 * to both next and previous nodes, enabling flexible list manipulation.
 */
template<class T> class DoubleLinkedList {
private:
    /**
     * @class Node
     * @brief Internal class representing a node in the doubly linked list
     */
    class Node {
        T m_data;           ///< Data stored in the node
        Node* m_pNext = nullptr;  ///< Pointer to the next node in the list
        Node* m_pPrev = nullptr;  ///< Pointer to the previous node in the list

        friend class DoubleLinkedList;
    };

    Node* m_pRoot = nullptr;  ///< Pointer to the first node in the list
    Node* m_pLast = nullptr;  ///< Pointer to the last node in the list
    size_t m_size = 0;        ///< Number of elements in the list

    /**
     * @brief Creates a new node with the specified data
     * @param t_data Data to be stored in the new node
     * @return Node* Pointer to the newly created node
     */
    Node* generateNode(T t_data);

public:
    // Constructors
    DoubleLinkedList();
    DoubleLinkedList(T t_data);

    // Accessors
    void traverse();
    void inverseTraverse();
    size_t size();
    T at(size_t t_index);

    // Mutators
    void push_back(T t_data);
    void push_front(T t_data);
    void pop_back();
    void pop_front();
    void insert_after(T t_data, size_t t_index);
    void erase(T t_data);
    void erase_all(T t_data);
    void reverse();

    // Operators
    T& operator [](size_t t_index);
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
typename DoubleLinkedList<T>::Node* DoubleLinkedList<T>::generateNode(T t_data) {
    Node* pNewNode = new Node();
    pNewNode->m_data = t_data;
    return pNewNode;
}

// =============================================================================
// CONSTRUCTORS
// =============================================================================

/**
 * @brief Default constructor - creates an empty list
 * @tparam T Type of elements to be stored in the list
 */
template <class T>
DoubleLinkedList<T>::DoubleLinkedList() {
    // Empty list initialization
}

/**
 * @brief Constructor that creates a list with one initial element
 * @tparam T Type of the initial element
 * @param t_data Data for the initial list element
 */
template <class T>
DoubleLinkedList<T>::DoubleLinkedList(T t_data) {
    m_pRoot = generateNode(t_data);
    m_pLast = m_pRoot;
    m_size = 1;
}

// =============================================================================
// ACCESSOR METHODS
// =============================================================================

/**
 * @brief Prints all elements in the list from first to last
 * @tparam T Type of elements in the list
 *
 * Traverses the list in forward direction and prints each element on a new line.
 * Optimized for single-element lists to avoid unnecessary pointer operations.
 */
template <class T>
void DoubleLinkedList<T>::traverse() {
    if (m_pRoot == m_pLast) {
        cout << m_pRoot->m_data << "\n";
        return;
    }

    Node* current = m_pRoot;
    while (current) {
        cout << current->m_data << "\n";
        current = current->m_pNext;
    }
}

/**
 * @brief Prints all elements in the list from last to first
 * @tparam T Type of elements in the list
 *
 * Traverses the list in reverse direction and prints each element on a new line.
 * Demonstrates the advantage of bidirectional linking in doubly linked lists.
 */
template <class T>
void DoubleLinkedList<T>::inverseTraverse() {
    if (m_pRoot == m_pLast) {
        cout << m_pLast->m_data << "\n";
        return;
    }

    Node* current = m_pLast;
    while (current) {
        cout << current->m_data << "\n";
        current = current->m_pPrev;
    }
}

/**
 * @brief Returns the number of elements in the list
 * @tparam T Type of elements in the list
 * @return size_t Current size of the list
 */
template <class T>
size_t DoubleLinkedList<T>::size() {
    return m_size;
}

/**
 * @brief Returns the element at the specified position
 * @tparam T Type of elements in the list
 * @param t_index Zero-based index of the element to retrieve
 * @return T Copy of the element at the specified position
 * @throws std::out_of_range if index is out of bounds or list is empty
 *
 * This method has O(n) time complexity in the worst case when accessing elements
 * near the end of the list.
 */
template <class T>
T DoubleLinkedList<T>::at(size_t t_index) {
    if (!m_pRoot || t_index >= m_size) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* current = m_pRoot;
    size_t counter = 0;
    while (counter != t_index) {
        current = current->m_pNext;
        counter++;
    }
    return current->m_data;
}

// =============================================================================
// MUTATOR METHODS
// =============================================================================

/**
 * @brief Adds an element to the end of the list
 * @tparam T Type of the element to add
 * @param t_data Data to be added to the end of the list
 *
 * Time complexity: O(1) - constant time operation
 * Maintains proper bidirectional links between nodes.
 */
template <class T>
void DoubleLinkedList<T>::push_back(T t_data) {
    if (!m_pRoot) {
        m_pRoot = generateNode(t_data);
        m_pLast = m_pRoot;
        m_size = 1;
        return;
    }

    m_size++;
    m_pLast->m_pNext = generateNode(t_data);
    m_pLast->m_pNext->m_pPrev = m_pLast;
    m_pLast = m_pLast->m_pNext;
}

/**
 * @brief Adds an element to the beginning of the list
 * @tparam T Type of the element to add
 * @param t_data Data to be added to the front of the list
 *
 * Time complexity: O(1) - constant time operation
 * Updates root pointer and maintains proper bidirectional links.
 */
template <class T>
void DoubleLinkedList<T>::push_front(T t_data) {
    if (!m_pRoot) {
        m_pRoot = generateNode(t_data);
        m_pLast = m_pRoot;
        m_size = 1;
        return;
    }

    m_size++;
    m_pRoot->m_pPrev = generateNode(t_data);
    m_pRoot->m_pPrev->m_pNext = m_pRoot;
    m_pRoot = m_pRoot->m_pPrev;
}

/**
 * @brief Removes the last element from the list
 * @tparam T Type of elements in the list
 *
 * Time complexity: O(1) - constant time operation
 * Properly handles edge cases including empty lists and single-element lists.
 * Updates the last pointer and maintains list integrity.
 */
template <class T>
void DoubleLinkedList<T>::pop_back() {
    if (!m_pRoot) {
        return;
    }

    if (m_pRoot == m_pLast) {
        delete m_pRoot;
        m_pRoot = nullptr;
        m_pLast = nullptr;
        m_size = 0;
        return;
    }

    m_size--;
    Node* nodeToDelete = m_pLast;
    m_pLast = m_pLast->m_pPrev;
    delete nodeToDelete;
    m_pLast->m_pNext = nullptr;
}

/**
 * @brief Removes the first element from the list
 * @tparam T Type of elements in the list
 *
 * Time complexity: O(1) - constant time operation
 * Updates the root pointer and maintains proper bidirectional links.
 * Handles edge cases including empty lists and single-element lists.
 */
template <class T>
void DoubleLinkedList<T>::pop_front() {
    if (!m_pRoot) {
        return;
    }

    if (m_pRoot == m_pLast) {
        delete m_pRoot;
        m_pRoot = nullptr;
        m_pLast = nullptr;
        m_size = 0;
        return;
    }

    m_size--;
    Node* nodeToDelete = m_pRoot;
    m_pRoot = m_pRoot->m_pNext;
    delete nodeToDelete;
    m_pRoot->m_pPrev = nullptr;
}

/**
 * @brief Inserts an element after the specified position
 * @tparam T Type of the element to insert
 * @param t_data Data to be inserted
 * @param t_index Zero-based index after which to insert the new element
 * @throws std::out_of_range if index is out of bounds
 *
 * Time complexity: O(n) in worst case, O(1) if inserting at known position
 * Maintains proper bidirectional links between all affected nodes.
 */
template <class T>
void DoubleLinkedList<T>::insert_after(T t_data, size_t t_index) {
    if (t_index >= m_size) {
        throw std::out_of_range("Index out of range");
    }

    if (!m_pRoot) {
        return;
    }

    // Optimized case: inserting at the end
    if (t_index == m_size - 1) {
        push_back(t_data);
        return;
    }

    m_size++;

    // Find the node at the specified index
    Node* current = m_pRoot;
    size_t counter = 0;
    while (current) {
        if (counter == t_index) {
            break;
        }
        current = current->m_pNext;
        counter++;
    }

    // Insert new node after current node
    Node* newNode = generateNode(t_data);
    newNode->m_pNext = current->m_pNext;
    newNode->m_pNext->m_pPrev = newNode;
    newNode->m_pPrev = current;
    current->m_pNext = newNode;
}

/**
 * @brief Removes the first occurrence of the specified data from the list
 * @tparam T Type of elements in the list
 * @param t_data Data value to remove from the list
 *
 * Time complexity: O(n) in worst case
 * Handles special cases for removal from front, back, and middle positions.
 * Maintains list integrity after removal.
 */
template <class T>
void DoubleLinkedList<T>::erase(T t_data) {
    if (!m_pRoot) {
        return;
    }

    // Special case: remove from front
    if (m_pRoot->m_data == t_data) {
        pop_front();
        return;
    }

    Node* current = m_pRoot;

    while (current) {
        // Special case: remove from back
        if ((current == m_pLast) && (current->m_data == t_data)) {
            pop_back();
            return;
        }
        else if (current->m_data == t_data) {
            // Remove from middle
            Node* nodeToDelete = current;
            current = current->m_pNext;
            current->m_pPrev = nodeToDelete->m_pPrev;
            nodeToDelete->m_pPrev->m_pNext = current;
            delete nodeToDelete;
            m_size--;
            break;
        }
        else if (current == m_pLast) {
            break;
        }
        else {
            current = current->m_pNext;
        }
    }
}

/**
 * @brief Removes all occurrences of the specified data from the list
 * @tparam T Type of elements in the list
 * @param t_data Data value to remove from the list
 *
 * Time complexity: O(n) - linear time operation
 * Efficiently handles multiple consecutive occurrences at front and back
 * before processing the middle of the list.
 */
template <class T>
void DoubleLinkedList<T>::erase_all(T t_data) {
    if (!m_pRoot) {
        return;
    }

    // Remove all matching elements from the front
    while (m_pRoot && m_pRoot->m_data == t_data) {
        pop_front();
    }

    // Remove all matching elements from the back
    while (m_pLast && m_pLast->m_data == t_data) {
        pop_back();
    }

    // Process the middle of the list
    Node* current = m_pRoot;
    while (current && current->m_pNext) {
        if (current->m_pNext->m_data == t_data) {
            Node* nodeToDelete = current->m_pNext;
            current->m_pNext = nodeToDelete->m_pNext;
            if (nodeToDelete->m_pNext) {
                nodeToDelete->m_pNext->m_pPrev = current;
            }
            delete nodeToDelete;
            m_size--;
        }
        else {
            current = current->m_pNext;
        }
    }
}

/**
 * @brief Reverses the order of elements in the list
 * @tparam T Type of elements in the list
 *
 * Time complexity: O(n) - linear time operation
 * This method completely reverses the list by rebuilding it from the end,
 * effectively swapping the roles of root and last pointers.
 */
template <class T>
void DoubleLinkedList<T>::reverse() {
    if ((!m_pRoot) || (m_size == 1)) {
        return;
    }

    // Build new list by repeatedly taking from the end of original list
    Node* newRoot = generateNode(m_pLast->m_data);
    Node* newLast = newRoot;
    pop_back();

    while (m_pRoot) {
        newLast->m_pNext = generateNode(m_pLast->m_data);
        newLast->m_pNext->m_pPrev = newLast;
        newLast = newLast->m_pNext;
        pop_back();
    }

    m_pRoot = newRoot;
    m_pLast = newLast;
}

// =============================================================================
// OPERATOR OVERLOADS
// =============================================================================

/**
 * @brief Array subscript operator for element access
 * @tparam T Type of elements in the list
 * @param t_index Zero-based index of the element to access
 * @return T& Reference to the element at the specified position
 * @throws std::out_of_range if index is out of bounds
 *
 * Time complexity: O(n) in worst case
 * Provides intuitive array-like access to list elements.
 * Returns a reference allowing modification of the element.
 */
template <class T>
T& DoubleLinkedList<T>::operator[](size_t t_index) {
    if (t_index >= m_size) {
        throw std::out_of_range("Index out of bounds");
    }

    Node* current = m_pRoot;
    for (size_t i = 0; i < t_index; i++) {
        current = current->m_pNext;
    }
    return current->m_data;
}