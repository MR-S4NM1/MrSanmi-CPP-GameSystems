#pragma once
#include <iostream>
#include "Queue.hpp"
#include "Stack.hpp"
#include "DoubleLinkedList.hpp"

using std::cout;
using std::endl;

/**
 * @class Graph
 * @brief A generic graph implementation supporting BFS and DFS traversal algorithms.
 * @tparam T The type of data stored in graph nodes
 * @author Miguel Ángel García Elizalde
 * @date 2024-05-10
 *
 * This graph implementation uses adjacency lists to store node connections and provides
 * both Breadth-First Search (BFS) and Depth-First Search (DFS) traversal methods.
 * The graph supports insertion, deletion, node swapping, and various traversal operations.
 */
template <class T>
class Graph {
private:
    /**
     * @class NodeGraph
     * @brief Internal class representing a node in the graph
     */
    class NodeGraph {
        T m_data;                                       ///< Data stored in the node
        DoubleLinkedList<NodeGraph*> m_children;        ///< List of adjacent nodes (children)
        bool has_been_visited = false;                  ///< Visitation flag for traversal algorithms

        friend class Graph;
    };

    NodeGraph* m_pRoot = nullptr;                       ///< Root node of the graph

    // Private utility methods
    NodeGraph* generateNodeGraph(T t_data);
    NodeGraph* BFS(T t_data);
    NodeGraph* DFS(T t_data);
    void reset(DoubleLinkedList<NodeGraph*> t_children);
    size_t parentsDFS(T t_data);

public:
    // Constructors & Destructor
    Graph();
    Graph(T t_data);
    Graph(T t_parent, DoubleLinkedList<T> t_children);

    // Accessors
    void traverseBFS();
    void traverseDFS();

    // Mutators
    void insert(T t_newData, T t_parent);
    void swap(T t_currentParent, T t_newParent, T t_data);
    void deleteNode(T t_data);
};

// =============================================================================
// PRIVATE METHOD IMPLEMENTATIONS
// =============================================================================

/**
 * @brief Creates a new graph node with the specified data
 * @tparam T Type of data to store in the node
 * @param t_data Data to be stored in the new node
 * @return NodeGraph* Pointer to the newly created node
 */
template <class T>
typename Graph<T>::NodeGraph* Graph<T>::generateNodeGraph(T t_data) {
    NodeGraph* pNewNodeGraph = new NodeGraph();
    pNewNodeGraph->m_data = t_data;
    pNewNodeGraph->m_children = DoubleLinkedList<NodeGraph*>();
    return pNewNodeGraph;
}

/**
 * @brief Performs Breadth-First Search to find a node with specified data
 * @tparam T Type of data to search for
 * @param t_data Data value to locate in the graph
 * @return NodeGraph* Pointer to the found node, or nullptr if not found
 *
 * This method traverses the graph level by level using a queue-based approach.
 * It marks visited nodes to prevent cycles and resets them after completion.
 */
template <class T>
typename Graph<T>::NodeGraph* Graph<T>::BFS(T t_data) {
    if (!m_pRoot) {
        return nullptr;
    }

    DoubleLinkedList<NodeGraph*> visitedNodes;
    Queue<NodeGraph*> searchQueue;

    searchQueue.enqueue(m_pRoot);
    m_pRoot->has_been_visited = true;

    while (!searchQueue.empty()) {
        NodeGraph* currentNode = searchQueue.dequeue();

        // Check if current node contains target data
        if (currentNode->m_data == t_data) {
            reset(visitedNodes);
            return currentNode;
        }

        // Process all unvisited children
        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];
                if (!child->has_been_visited) {
                    searchQueue.enqueue(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }
            }
        }
    }

    reset(visitedNodes);
    return nullptr;
}

/**
 * @brief Performs Depth-First Search to find a node with specified data
 * @tparam T Type of data to search for
 * @param t_data Data value to locate in the graph
 * @return NodeGraph* Pointer to the found node, or nullptr if not found
 *
 * This method traverses the graph depth-first using a stack-based approach.
 * It explores as far as possible along each branch before backtracking.
 */
template <class T>
typename Graph<T>::NodeGraph* Graph<T>::DFS(T t_data) {
    if (!m_pRoot) {
        return nullptr;
    }

    DoubleLinkedList<NodeGraph*> visitedNodes;
    Stack<NodeGraph*> searchStack;

    searchStack.enstack(m_pRoot);
    m_pRoot->has_been_visited = true;
    visitedNodes.push_back(m_pRoot);

    while (!searchStack.empty()) {
        NodeGraph* currentNode = searchStack.destack();

        if (currentNode->m_data == t_data) {
            reset(visitedNodes);
            return currentNode;
        }

        // Process all unvisited children
        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];
                if (!child->has_been_visited) {
                    searchStack.enstack(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }
            }
        }
    }

    reset(visitedNodes);
    return nullptr;
}

/**
 * @brief Resets the visitation flags of all nodes in the provided list
 * @tparam T Type of data stored in graph nodes
 * @param t_children List of nodes whose visitation flags need to be reset
 *
 * This method is used after traversal algorithms to clean up visitation states
 * and prepare the graph for subsequent operations.
 */
template <class T>
void Graph<T>::reset(DoubleLinkedList<NodeGraph*> t_children) {
    if (!m_pRoot) {
        return;
    }

    for (size_t i = 0; i < t_children.size(); i++) {
        t_children[i]->has_been_visited = false;
    }
}

/**
 * @brief Counts how many parent nodes reference a node with the specified data
 * @tparam T Type of data to search for
 * @param t_data Data value to count parents for
 * @return size_t Number of parent nodes that reference the target data
 * @throws std::exception if the graph is empty
 *
 * This method helps determine if a node can be safely deleted by checking
 * how many other nodes reference it.
 */
template <class T>
size_t Graph<T>::parentsDFS(T t_data) {
    if (!m_pRoot) {
        throw std::exception();
    }

    size_t parentCount = 0;
    DoubleLinkedList<NodeGraph*> visitedNodes;
    Stack<NodeGraph*> searchStack;

    searchStack.enstack(m_pRoot);
    m_pRoot->has_been_visited = true;

    while (!searchStack.empty()) {
        NodeGraph* currentNode = searchStack.destack();

        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];

                if (!child->has_been_visited) {
                    searchStack.enstack(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }

                // Count if this child contains the target data
                if (child->m_data == t_data) {
                    parentCount++;
                }
            }
        }
    }

    reset(visitedNodes);
    return parentCount;
}

// =============================================================================
// PUBLIC METHOD IMPLEMENTATIONS
// =============================================================================

// CONSTRUCTORS

/**
 * @brief Default constructor - creates an empty graph
 * @tparam T Type of data to be stored in the graph
 */
template <class T>
Graph<T>::Graph() {
    m_pRoot = nullptr;
}

/**
 * @brief Constructor that creates a graph with a single root node
 * @tparam T Type of data to be stored in the root node
 * @param t_data Data to be stored in the root node
 */
template <class T>
Graph<T>::Graph(T t_data) {
    m_pRoot = generateNodeGraph(t_data);
}

/**
 * @brief Constructor that creates a graph with a root node and predefined children
 * @tparam T Type of data to be stored in the nodes
 * @param t_parent Data for the root node
 * @param t_children List of data values for the root node's children
 */
template <class T>
Graph<T>::Graph(T t_parent, DoubleLinkedList<T> t_children) {
    m_pRoot = generateNodeGraph(t_parent);

    // Create and attach all child nodes
    for (size_t i = 0; i < t_children.size(); i++) {
        m_pRoot->m_children.push_back(generateNodeGraph(t_children[i]));
    }
}

// ACCESSORS

/**
 * @brief Performs Breadth-First Search traversal and prints the graph structure
 * @tparam T Type of data stored in the graph
 *
 * Output format: parent_data(child1_data, child2_data, ...)
 * This method provides a hierarchical view of the graph structure.
 */
template <class T>
void Graph<T>::traverseBFS() {
    if (!m_pRoot) {
        return;
    }

    DoubleLinkedList<NodeGraph*> visitedNodes;
    Queue<NodeGraph*> traversalQueue;

    traversalQueue.enqueue(m_pRoot);
    m_pRoot->has_been_visited = true;

    while (!traversalQueue.empty()) {
        NodeGraph* currentNode = traversalQueue.dequeue();

        cout << currentNode->m_data << "(";

        // Print and process all children
        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];

                if (!child->has_been_visited) {
                    traversalQueue.enqueue(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }

                cout << child->m_data;
                if (i < currentNode->m_children.size() - 1) {
                    cout << ", ";
                }
            }
        }

        cout << ")" << endl;
    }

    reset(visitedNodes);
}

/**
 * @brief Performs Depth-First Search traversal and prints the graph structure
 * @tparam T Type of data stored in the graph
 *
 * Output format: parent_data(child1_data, child2_data, ...)
 * This method shows the depth-oriented structure of the graph.
 */
template <class T>
void Graph<T>::traverseDFS() {
    if (!m_pRoot) {
        return;
    }

    DoubleLinkedList<NodeGraph*> visitedNodes;
    Stack<NodeGraph*> traversalStack;

    traversalStack.enstack(m_pRoot);
    m_pRoot->has_been_visited = true;

    while (!traversalStack.empty()) {
        NodeGraph* currentNode = traversalStack.destack();

        cout << currentNode->m_data << "(";

        // Print and process all children
        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];

                if (!child->has_been_visited) {
                    traversalStack.enstack(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }

                cout << child->m_data;
                if (i < currentNode->m_children.size() - 1) {
                    cout << ", ";
                }
            }
        }

        cout << ")" << endl;
    }

    reset(visitedNodes);
}

// MUTATORS

/**
 * @brief Inserts a new node into the graph as a child of the specified parent
 * @tparam T Type of data stored in the graph
 * @param t_parent Data value of the parent node
 * @param t_newData Data value for the new node to insert
 * @throws std::exception if the graph is empty or parent node is not found
 *
 * If a node with the new data already exists, it will be linked to the parent
 * without creating a duplicate node.
 */
template <class T>
void Graph<T>::insert(T t_parent, T t_newData) {
    if (!m_pRoot) {
        throw std::exception();
    }

    // Find parent node
    NodeGraph* parentNode = BFS(t_parent);
    if (!parentNode) {
        throw std::exception();
    }

    // Check if node already exists, if so link it, otherwise create new node
    NodeGraph* existingNode = DFS(t_newData);
    if (existingNode) {
        parentNode->m_children.push_back(existingNode);
        return;
    }

    // Create and link new node
    NodeGraph* newNode = generateNodeGraph(t_newData);
    parentNode->m_children.push_back(newNode);
}

/**
 * @brief Moves a node from one parent to another in the graph
 * @tparam T Type of data stored in the graph
 * @param t_currentParent Data value of the current parent node
 * @param t_newParent Data value of the new parent node
 * @param t_data Data value of the node to be moved
 * @throws std::exception if the graph is empty or specified nodes are not found
 */
template <class T>
void Graph<T>::swap(T t_currentParent, T t_newParent, T t_data) {
    if (!m_pRoot) {
        throw std::exception();
    }

    // Find both parent nodes
    NodeGraph* currentParent = BFS(t_currentParent);
    if (!currentParent) {
        throw std::exception();
    }

    NodeGraph* newParent = BFS(t_newParent);
    if (!newParent) {
        throw std::exception();
    }

    // Find and move the target node
    for (size_t i = 0; i < currentParent->m_children.size(); i++) {
        if (currentParent->m_children[i]->m_data == t_data) {
            // Move node from current parent to new parent
            newParent->m_children.push_back(currentParent->m_children[i]);
            currentParent->m_children.erase(currentParent->m_children[i]);
            return;
        }
    }
}

/**
 * @brief Deletes a node from the graph if it has only one parent reference
 * @tparam T Type of data stored in the graph
 * @param t_data Data value of the node to delete
 * @throws std::exception if the graph is empty, node not found, or node is root
 *
 * Safety checks prevent deletion of root nodes or nodes with multiple parents
 * to maintain graph integrity.
 */
template <class T>
void Graph<T>::deleteNode(T t_data) {
    if (!m_pRoot) {
        throw std::exception();
    }

    // Verify node exists
    NodeGraph* targetNode = DFS(t_data);
    if (!targetNode) {
        throw std::exception();
    }

    // Safety checks
    size_t parentCount = parentsDFS(t_data);
    if (m_pRoot->m_data == t_data) {
        throw std::exception(); // Cannot delete root node
    }

    if (parentCount > 1) {
        return; // Cannot delete node with multiple parents
    }

    // Find and remove the node from its parent's children list
    DoubleLinkedList<NodeGraph*> visitedNodes;
    Stack<NodeGraph*> searchStack;

    searchStack.enstack(m_pRoot);
    m_pRoot->has_been_visited = true;

    while (!searchStack.empty()) {
        NodeGraph* currentNode = searchStack.destack();

        if (currentNode->m_children.size() > 0) {
            for (size_t i = 0; i < currentNode->m_children.size(); i++) {
                NodeGraph* child = currentNode->m_children[i];

                if (!child->has_been_visited) {
                    searchStack.enstack(child);
                    child->has_been_visited = true;
                    visitedNodes.push_back(child);
                }

                // Remove the target node when found
                if ((parentCount == 1) && (child->m_data == t_data)) {
                    currentNode->m_children.erase(child);
                    break;
                }
            }
        }
    }

    reset(visitedNodes);
}