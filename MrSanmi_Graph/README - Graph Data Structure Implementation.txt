README – Graph Data Structure Implementation in C++

Author: Miguel Ángel García Elizalde  
Date: May 2024  
Language: C++17  
Project Type: Data Structures & Algorithms Implementation  
Development Environment: Replit with classic C++

---

Overview

This project implements a comprehensive Graph data structure in C++17, featuring both Breadth-First Search (BFS) and Depth-First Search (DFS) traversal algorithms. The implementation demonstrates professional-level C++ programming with custom-built underlying data structures, memory management, and algorithm optimization.

The graph system was built entirely from scratch without external libraries, showcasing strong understanding of graph theory, data structure design, and C++ memory management.

---

Core Features

Graph Data Structure
- Directed Graph implementation supporting complex node relationships
- Multiple Parent Support - nodes can belong to multiple parents
- Dynamic Structure - supports runtime modifications and restructuring
- Cycle Detection - built-in mechanisms to prevent infinite loops during traversal

Traversal Algorithms
- Breadth-First Search (BFS) - level-by-level traversal using queue-based approach
- Depth-First Search (DFS) - depth-oriented traversal using stack-based approach
- Bidirectional Traversal - efficient navigation through graph hierarchy

Memory Management
- RAII Compliance - proper resource acquisition and release
- Smart Pointer Alternative - manual memory management with exception safety
- Visitation State Tracking - prevents infinite loops during complex traversals

Modular Architecture
- Custom DoubleLinkedList - bidirectional node relationships
- Stack & Queue Implementations - specialized for graph traversal needs
- Template-Based Design - type-safe implementation for any data type

---

Data Structures Implemented

Graph Class:

template<class T>
class Graph {
    // Supports: BFS, DFS, insertion, deletion, parent swapping
    // Features: multiple parent relationships, cycle prevention
};

Supporting Data Structures
- DoubleLinkedList - bidirectional traversal capabilities
- Stack - LIFO structure for DFS implementation
- Queue - FIFO structure for BFS implementation

---

Key Algorithms

Breadth-First Search (BFS)
- Time Complexity: O(V + E)
- Space Complexity: O(V)
- Use Case: Shortest path finding, level-order traversal

Depth-First Search (DFS)
- Time Complexity: O(V + E)
- Space Complexity: O(V)
- Use Case: Topological sorting, path existence checking

Graph Operations
- Node Insertion - O(1) when parent known, O(V + E) for search
- Node Deletion - O(V + E) with multiple parent protection
- Parent Swapping - O(V + E) for dynamic relationship modification

---

Code Architecture

Design Patterns:
- Template Method Pattern - algorithm specialization for BFS/DFS
- Factory Pattern - node creation and initialization
- RAII Pattern - resource management and memory safety

Memory Safety

- Proper memory deallocation
delete node; // Instead of delete[] for single objects
- Visitation state reset after each traversal


Exception Handling
- std::out_of_range - for invalid index access
- std::runtime_error - for empty structure operations
- std::domain_error - for invalid state operations

---

Example Usage

Graph Construction:

// Create graph with initial node
Graph<int> socialNetwork(1);

// Add connections
socialNetwork.insert(1, 5);  // Node 1 -> Node 5
socialNetwork.insert(1, 3);  // Node 1 -> Node 3
socialNetwork.insert(5, 2);  // Node 5 -> Node 2
socialNetwork.insert(1, 2);  // Node 1 -> Node 2 (multiple parents)


Traversal Examples:

// BFS Traversal
cout << "Breadth-First Search:" << endl;
socialNetwork.traverseBFS();

// DFS Traversal  
cout << "Depth-First Search:" << endl;
socialNetwork.traverseDFS();

Advanced Operations:

// Dynamic parent modification
socialNetwork.swap(5, 2, 8);  // Move node 8 from parent 5 to parent 2

// Safe node deletion (protected for multiple parents)
socialNetwork.deleteNode(3);

---

Performance Characteristics

| Operation 	| Time Complexity | Space Complexity |

| BFS Traversal | O(V + E) 	  | O(V) |
| DFS Traversal | O(V + E) 	  | O(V) |
| Node Insertion| O(V + E) 	  | O(1) |
| Node Deletion | O(V + E) 	  | O(1) |
| Parent Swap 	| O(V + E) 	  | O(1) |

---

Technical Highlights

Professional C++ Features
- Template Metaprogramming - type-safe generic implementation
- Exception Safety - strong exception guarantee for all operations
- STL Integration - compatible with standard library conventions
- Doxygen Documentation - professional-grade code documentation

Algorithm Optimization
- Visitation Flag System - prevents redundant node processing
- Bidirectional Linking - efficient forward and backward traversal
- Memory Reuse - existing node linking without duplication

Code Quality
- Enterprise Standards - suitable for professional code review
- Comprehensive Testing - demonstrated through interactive main.cpp
- Extensible Design - easy to add new algorithms and features

---

How to Run

1. Clone the repository and open it in your C++ environment (Visual Studio, CLion, etc.) / preferably Replit.
2. Compile all .cpp files.
3. Run the executable from the console.

Expected Output
The demo will show:
1. Graph construction and initialization
2. Multiple node insertion operations
3. BFS and DFS traversal comparisons
4. Dynamic parent-child relationship modifications
5. Safe node deletion with multiple parent protection

---

File Structure

Graph_MrSanmi
│
├── Graph.hpp            # Main graph implementation
├── DoubleLinkedList.hpp # Bidirectional list for node relationships
├── Stack.hpp            # LIFO structure for DFS
├── Queue.hpp            # FIFO structure for BFS
├── main.cpp             # Comprehensive demonstration
└── README.md            # This file

---

Final Notes

This project represents a complete graph data structure implementation emphasizing:

- Algorithmic Excellence - professional BFS and DFS implementations
- Memory Safety** - robust resource management in classic C++
- Software Engineering - clean architecture and extensible design
- Documentation Quality - professional-grade comments and examples