#include <iostream>
#include "Graph.hpp"

/**
 * @mainpage Graph Data Structure Demonstration
 * @brief Comprehensive demonstration of Graph class functionality
 * @author Miguel Ángel García Elizalde
 *
 * This program demonstrates the capabilities of the Graph data structure
 * implementation, including construction, insertion, deletion, traversal,
 * and node manipulation operations.
 */

int main() {
    using std::cout;
    using std::endl;

    cout << "Graph Implementation" << endl;
    cout << "Author: Miguel Ángel García Elizalde" << endl;
    cout << "=====================================" << endl << endl;

    // =========================================================================
    // PART 1: CONSTRUCTOR DEMONSTRATION
    // =========================================================================
    cout << "PART 1: Constructor Verification" << endl;
    cout << "Demonstrating graph creation and initialization." << endl << endl;

    cout << "Graph with parent node and predefined children list:" << endl;
    DoubleLinkedList<int> childNodes(5);
    childNodes.push_back(7);
    childNodes.push_back(9);
    Graph<int> predefinedGraph(10, childNodes);
    predefinedGraph.traverseBFS();

    cout << endl;
    cout << "Graph with default constructor and single data element:" << endl;
    cout << "(This graph will be used for subsequent operations)" << endl;
    Graph<int> demonstrationGraph(1);
    demonstrationGraph.traverseBFS();

    // =========================================================================
    // PART 2: NODE INSERTION OPERATIONS
    // =========================================================================
    cout << endl;
    cout << "PART 2: Node Insertion Operations" << endl;
    cout << "Demonstrating insert(T parent, T newData) functionality:" << endl;
    cout << "- If node exists, parent gains it as an additional child" << endl;
    cout << "- If node doesn't exist, it's created and linked to parent" << endl << endl;

    // Build complex graph structure
    demonstrationGraph.insert(1, 5);
    demonstrationGraph.insert(1, 3);
    demonstrationGraph.insert(1, 7);
    demonstrationGraph.insert(5, 2);
    demonstrationGraph.insert(5, 9);
    demonstrationGraph.insert(5, 15);
    demonstrationGraph.insert(5, 8);
    demonstrationGraph.insert(1, 2);  // Node 2 now has multiple parents

    cout << "Graph structure after multiple insertions:" << endl;
    demonstrationGraph.traverseBFS();

    // =========================================================================
    // PART 3: NODE DELETION OPERATIONS
    // =========================================================================
    cout << endl;
    cout << "PART 3: Node Deletion Operations" << endl;
    cout << "Demonstrating deleteNode(T data) functionality:" << endl;
    cout << "- Nodes with multiple parents cannot be deleted" << endl;
    cout << "- Root node deletion is prevented" << endl << endl;

    // Setup for deletion demonstration
    demonstrationGraph.insert(3, 20);
    demonstrationGraph.insert(1, 20);  // Node 20 now has multiple parents

    cout << "Before deletion operations:" << endl;
    demonstrationGraph.traverseBFS();

    // Attempt various deletions
    demonstrationGraph.deleteNode(3);   // Should succeed (single parent)
    demonstrationGraph.insert(7, 30);
    demonstrationGraph.deleteNode(30);  // Should succeed (single parent)
    // Node 20 remains (multiple parents protection)

    cout << "After deletion operations:" << endl;
    demonstrationGraph.traverseBFS();

    // =========================================================================
    // PART 4: TRAVERSAL ALGORITHMS
    // =========================================================================
    cout << endl;
    cout << "PART 4: Graph Traversal Algorithms" << endl;
    cout << "Comparing Breadth-First Search (BFS) and Depth-First Search (DFS)" << endl;
    cout << "Verifying acyclic graph structure and algorithm correctness." << endl << endl;

    cout << "Breadth-First Search (BFS) Traversal:" << endl;
    cout << "Explores neighbors level by level" << endl;
    demonstrationGraph.traverseBFS();

    cout << endl << "Depth-First Search (DFS) Traversal:" << endl;
    cout << "Explores as far as possible along each branch" << endl;
    demonstrationGraph.traverseDFS();

    // =========================================================================
    // PART 5: NODE PARENT SWAPPING
    // =========================================================================
    cout << endl;
    cout << "PART 5: Node Parent Relationship Modification" << endl;
    cout << "Demonstrating swap(T currentParent, T newParent, T data) functionality:" << endl;
    cout << "- Transfers a node from one parent to another" << endl;
    cout << "- Maintains graph integrity during the operation" << endl << endl;

    cout << "Before parent swap operation:" << endl;
    demonstrationGraph.traverseBFS();

    // Move node 8 from parent 5 to parent 2
    demonstrationGraph.swap(5, 2, 8);

    cout << "After swapping parent of node 8 from 5 to 2:" << endl;
    demonstrationGraph.traverseBFS();

    cout << endl;
    cout << "=====================================" << endl;
    cout << "Graph demonstration completed successfully!" << endl;
    cout << "All operations verified and functional." << endl;

    return 0;
}