/**
 * @file LinkedHeapTree.h
 * @brief Template-based min-heap implementation using linked tree structure
 * 
 * This file implements a min-heap priority queue using a linked binary tree
 * structure. The heap is used by TourPlanner for efficient nearest-neighbor
 * selection during tour path calculation.
 * 
 * Key Features:
 * - Generic template supporting any key/value types
 * - O(log n) insert, remove, and key updates
 * - O(1) minimum access
 * - Position-based API for direct node manipulation
 * 
 * @author CS1D Group Project
 * @date March 2026
 */

#ifndef LINKED_HEAP_TREE
#define LINKED_HEAP_TREE
#include <cstddef>
#include <iostream>
#include <list>
#include <ostream>
#include <queue>

/**
 * @class LinkedHeapTree
 * @brief Min-heap priority queue using linked binary tree
 * 
 * A min-heap implementation where the element with the smallest key
 * is always at the root. Uses a linked tree structure instead of
 * array-based storage, allowing for position-based operations.
 * 
 * @tparam Key Comparable type for heap ordering (e.g., double for distances)
 * @tparam Value Data type stored with each key (e.g., int for campus index)
 * 
 * Data Structure Analysis:
 * - Type: Binary Min-Heap (complete binary tree)
 * - Storage: Linked nodes with parent/child pointers
 * - Ordering: Parent key <= child keys (min-heap property)
 * 
 * Usage in TourPlanner:
 * @code
 * LinkedHeapTree<double, int> heap;
 * for (int i = 0; i < campuses.size(); i++) {
 *     if (!visited[i])
 *         heap.insert(distances[current][i], i);
 * }
 * int nearest = heap.removeMin();  // Get closest campus index
 * @endcode
 */
template <typename Key, typename Value> class LinkedHeapTree
{

public:
    struct Node;
    
    /**
     * @struct Entry
     * @brief Stores a key-value pair in the heap
     * 
     * Each entry contains the ordering key, the stored value,
     * and a pointer to its containing node.
     */
    struct Entry
    {
        /**
         * @brief Constructs an entry with key and value
         * @param aKey The ordering key
         * @param aValue The stored value
         */
        Entry(Key aKey, Value aValue)
            : key(aKey), element(aValue), node(nullptr)
        {
        }

        Key key;        ///< Ordering key for heap property
        Value element;  ///< Stored value associated with key
        Node *node;     ///< Pointer to containing node

        /**
         * @brief Stream output operator for Entry
         * @param os Output stream
         * @param e Entry to output
         * @return Reference to output stream
         */
        friend std::ostream &operator<<(std::ostream &os,
                                        const Entry &e)
        {
            os << "<" << e.element << ":" << e.key << ">";
            return os;
        }
    };

    /**
     * @struct Node
     * @brief Tree node containing entry and tree pointers
     * 
     * Stores pointers to parent and children for tree traversal.
     */
    struct Node
    {
        /**
         * @brief Default constructor initializes all pointers to null
         */
        Node()
            : entry(nullptr), parent(nullptr), left(nullptr),
              right(nullptr)
        {
        }
        Entry *entry;   ///< Pointer to contained entry
        Node *parent;   ///< Pointer to parent node (null for root)
        Node *left;     ///< Pointer to left child
        Node *right;    ///< Pointer to right child
    };

    /**
     * @class Position
     * @brief Handle for accessing and manipulating heap entries
     * 
     * Provides an abstraction for interacting with heap entries
     * without direct node manipulation. Supports tree traversal
     * via left(), right(), and parent() methods.
     */
    class Position
    {
    public:
        /** @brief Default constructor creates invalid position */
        Position() : entry(nullptr)
        {
        }
        
        /**
         * @brief Constructs position from entry pointer
         * @param e Pointer to entry
         */
        Position(Entry *e) : entry(e)
        {
        }

        /**
         * @brief Dereference operator for entry access
         * @return Reference to entry
         */
        Entry &operator*()
        {
            return *entry;
        }
        
        /**
         * @brief Const dereference operator
         * @return Const reference to entry
         */
        const Entry &operator*() const
        {
            return *entry;
        }

        /**
         * @brief Gets raw entry pointer
         * @return Pointer to entry
         */
        Entry *getEntry() const
        {
            return entry;
        }

        /**
         * @brief Gets the containing node
         * @return Pointer to node, or nullptr if invalid position
         */
        Node *getNode() const
        {
            if (entry == nullptr)
                return nullptr;
            return entry->node;
        }

        /**
         * @brief Gets position of left child
         * @return Position of left child, or invalid position if none
         */
        Position left() const
        {
            if (!entry->node->left)
                return Position();
            return Position(entry->node->left->entry);
        }
        
        /**
         * @brief Gets position of right child
         * @return Position of right child, or invalid position if none
         */
        Position right() const
        {
            if (!entry->node->right)
                return Position();
            return Position(entry->node->right->entry);
        }
        
        /**
         * @brief Gets position of parent
         * @return Position of parent, or invalid position if root
         */
        Position parent() const
        {
            if (!entry->node->parent)
                return Position();
            return Position(entry->node->parent->entry);
        }
        
        /**
         * @brief Checks if this is the root position
         * @return true if root (no parent), false otherwise
         */
        bool isRoot() const
        {
            return !entry->node->parent;
        }
        
        /**
         * @brief Checks if this is a leaf position
         * @return true if no children, false otherwise
         */
        bool isLeaf() const
        {
            return !left().entry && !right().entry;
        }

        /**
         * @brief Stream output operator for Position
         * @param os Output stream
         * @param p Position to output
         * @return Reference to output stream
         */
        friend std::ostream &operator<<(std::ostream &os,
                                        const Position &p)
        {
            os << *p;
            return os;
        }

    private:
        Entry *entry = nullptr;  ///< Pointer to contained entry
    };

    typedef std::list<Position> PositionList;

private:
    Node *root;      ///< Pointer to root node
    int nodeCount;   ///< Number of nodes in heap

public:
    /**
     * @brief Default constructor creates empty heap
     */
    LinkedHeapTree<Key, Value>() : nodeCount(0), root(NULL)
    {
    }

    /**
     * @brief Returns number of elements in heap
     * @return Element count
     * 
     * Time Complexity: O(1)
     */
    int size() const
    {
        return nodeCount;
    }

    /**
     * @brief Checks if heap is empty
     * @return true if empty, false otherwise
     * 
     * Time Complexity: O(1)
     */
    bool empty() const
    {
        return !root;
    }

    /**
     * @brief Inserts a new key-value pair into the heap
     * 
     * Adds a new entry at the next available position (maintaining
     * complete binary tree property), then bubbles up to restore
     * heap property.
     * 
     * @param k Key for ordering
     * @param v Value to store
     * @return Position of the inserted entry
     * 
     * Time Complexity: O(log n)
     * - Finding insertion point: O(log n) using bit manipulation
     * - Up-heap bubble: O(log n) swaps in worst case
     * 
     * Space Complexity: O(1) additional
     */
    Position insert(Key k, Value v)
    {
        Entry *newEntry = new Entry(k, v);
        Node *newNode   = new Node;

        newNode->entry = newEntry;
        newEntry->node = newNode;
        nodeCount++;

        if (!root)
        {
            root = newNode;
            return Position(newEntry);
        }

        int lastIndex = size();
        int parentIndex = lastIndex >> 1;

        Node *parent    = findNode(parentIndex);
        newNode->parent = parent;
        if ((lastIndex & 1) == 0)
            parent->left = newNode;
        else
            parent->right = newNode;

        upHeapBubble(Position(newEntry));
        return Position(newEntry);
    }

    /**
     * @brief Removes entry at specified position
     * 
     * Swaps the target entry with the last entry, removes the
     * last node, then restores heap property via bubble operations.
     * 
     * @param originalPos Position of entry to remove
     * @return Value of the removed entry
     * 
     * Time Complexity: O(log n)
     * - Finding last node: O(log n)
     * - Bubble operation: O(log n)
     * 
     * Space Complexity: O(1) additional
     */
    Value remove(const Position &originalPos)
    {
        Entry *entryToBeRemoved = originalPos.getEntry();
        Value retValue          = entryToBeRemoved->element;

        if (size() == 1)
        {
            delete entryToBeRemoved->node;
            delete entryToBeRemoved;
            root = nullptr;
            nodeCount--;
            return retValue;
        }
        Position lastNodePos = Position(findNode(size())->entry);
        Node *lastNode       = lastNodePos.getNode();

        if (lastNode == entryToBeRemoved->node)
        {
            if (lastNode->parent->left == entryToBeRemoved->node)
                lastNode->parent->left = nullptr;
            else
                lastNode->parent->right = nullptr;

            delete entryToBeRemoved->node;
            delete entryToBeRemoved;
            nodeCount--;
            return retValue;
        }

        swap(originalPos, lastNodePos);

        if (lastNode->parent->left == entryToBeRemoved->node)
        {
            lastNode->parent->left = nullptr;
        }
        else
        {
            lastNode->parent->right = nullptr;
        }

        delete entryToBeRemoved->node;
        delete entryToBeRemoved;
        nodeCount--;

        if (lastNodePos.isRoot())
        {
            downHeapBubble(lastNodePos);
        }
        else if (lastNodePos.getEntry()->key <
                 lastNodePos.parent().getEntry()->key)
        {
            upHeapBubble(lastNodePos);
        }
        else
        {
            downHeapBubble(lastNodePos);
        }
        return retValue;
    }

    /**
     * @brief Removes and returns the minimum element
     * 
     * Convenience method that removes the root element (minimum key).
     * 
     * @return Value of the minimum element
     * @throws std::runtime_error if heap is empty
     * 
     * Time Complexity: O(log n) - delegated to remove()
     */
    Value removeMin()
    {
        if (!root)
            throw std::runtime_error("Heap empty");
        return remove(Position(root->entry));
    }

    /**
     * @brief Updates the key of an entry and restores heap property
     * 
     * @param p Position of entry to update
     * @param newKey New key value
     * @return Position of the entry (may have moved)
     * 
     * Time Complexity: O(log n) for bubble operation
     */
    Position replaceKey(const Position &p, Key newKey)
    {
        p.getEntry()->key = newKey;

        if (p.isRoot())
        {
            downHeapBubble(p);
        }
        else if (p.getEntry()->key < p.parent().getEntry()->key)
        {
            upHeapBubble(p);
        }
        else
        {
            downHeapBubble(p);
        }

        return p;
    }

    /**
     * @brief Returns position of the minimum element (root)
     * @return Position of root entry
     * 
     * Time Complexity: O(1)
     */
    Position min() const
    {
        return Position(root->entry);
    }

    /**
     * @brief Prints the heap structure to console
     * 
     * Uses inorder traversal to display tree structure with
     * visual connection lines.
     * 
     * @param rt Starting position (usually root)
     * @param prefix String prefix for indentation
     * @param isLeft Whether this is a left child
     */
    void prettyPrint(const Position &rt, std::string prefix = "",
                     bool isLeft = true)
    {
        if (rt.right().getEntry())
        {

            prettyPrint(rt.right(), prefix + (isLeft ? "|   " : "    "),
                        false);
        }

        std::cout << prefix;
        std::cout << (isLeft ? "+-- " : "+-- ");
        std::cout << rt << std::endl;

        if (rt.left().getEntry())
        {
            prettyPrint(rt.left(), prefix + (isLeft ? "    " : "|   "),
                        true);
        }
    }

    /**
     * @brief Swaps entries between two positions
     * 
     * Exchanges the entries of two nodes without changing
     * the tree structure. Used during removal operations.
     * 
     * @param p1 First position
     * @param p2 Second position
     * 
     * Time Complexity: O(1)
     */
    void swap(const Position &p1, const Position &p2)
    {
        Entry *p1OriginalEntry = p1.getEntry();
        Node *p1OriginalNode   = p1.getNode();
        Entry *p2OriginalEntry = p2.getEntry();
        Node *p2OriginalNode   = p2.getNode();

        p1OriginalNode->entry = p2OriginalEntry;
        p2OriginalNode->entry = p1OriginalEntry;

        p1OriginalNode->entry->node = p1OriginalNode;
        p2OriginalNode->entry->node = p2OriginalNode;
    }

protected:
    /**
     * @brief Finds node at given index using bit manipulation
     * 
     * Uses the binary representation of the index to navigate
     * the tree. Each bit (after the leading 1) represents a
     * left (0) or right (1) decision.
     * 
     * @param index 1-based index of the node
     * @return Pointer to the node at that index
     * 
     * Time Complexity: O(log n) - depth of tree
     * 
     * Algorithm:
     * - Index 1 = root
     * - Index 2 = root->left, Index 3 = root->right
     * - For index i: parent is at i/2
     * - Bit pattern determines path: 0=left, 1=right
     */
    Node *findNode(int index)
    {
        Node *current = root;
        int mask      = 1;

        while (mask <= index)
            mask <<= 1;
        mask >>= 2;

        while (mask > 0)
        {
            if ((index & mask) == 0)
                current = current->left;
            else
                current = current->right;
            mask >>= 1;
        }

        return current;
    }

    /**
     * @brief Bubbles an entry up to restore heap property
     * 
     * Recursively swaps entry with parent while entry's key
     * is smaller than parent's key.
     * 
     * @param currentPos Position to bubble up
     * 
     * Time Complexity: O(log n) - at most tree height swaps
     */
    void upHeapBubble(const Position &currentPos)
    {
        if (!currentPos.isRoot())
        {
            Position parent = currentPos.parent();
            if (currentPos.getEntry()->key < parent.getEntry()->key)
            {
                swap(currentPos, parent);
                upHeapBubble(currentPos);
            }
        }
    }

    /**
     * @brief Bubbles an entry down to restore heap property
     * 
     * Recursively swaps entry with smaller child while entry's
     * key is larger than that child's key.
     * 
     * @param currentPos Position to bubble down
     * 
     * Time Complexity: O(log n) - at most tree height swaps
     */
    void downHeapBubble(const Position &currentPos)
    {
        if (currentPos.isLeaf())
            return;

        Position left  = currentPos.left();
        Position right = currentPos.right();

        Position smallerChild;

        if (!right.getEntry())
            smallerChild = left;
        else if (!left.getEntry())
            smallerChild = right;
        else
        {
            if (left.getEntry()->key < right.getEntry()->key)
                smallerChild = left;
            else
                smallerChild = right;
        }

        if (smallerChild.getEntry()->key < currentPos.getEntry()->key)
        {
            swap(currentPos, smallerChild);
            downHeapBubble(currentPos);
        }
    }
};

#endif
