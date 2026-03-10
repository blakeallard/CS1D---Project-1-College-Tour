#ifndef LINKED_HEAP_TREE
#define LINKED_HEAP_TREE
#include <cstddef>
#include <iostream>
#include <list>
#include <ostream>
#include <queue>

// Min at top
template <typename Key, typename Value> class LinkedHeapTree
{

    public:
        struct Node;
        struct Entry
        {
                Entry(Key aKey, Value aValue)
                    : key(aKey), element(aValue), node(nullptr)
                {
                }

                Key key;
                Value element;
                Node *node;

                friend std::ostream &operator<<(std::ostream &os,
                                                const Entry &e)
                {
                    os << "<" << e.element << ":" << e.key << ">";
                    return os;
                }
        };

        struct Node
        {

                Node()
                    : entry(nullptr), parent(nullptr), left(nullptr),
                      right(nullptr)
                {
                }
                Entry *entry;
                Node *parent;
                Node *left;
                Node *right;
        };

        class Position
        {
            public:
                Position() : entry(nullptr)
                {
                }
                Position(Entry *e) : entry(e)
                {
                }

                Entry &operator*()
                {
                    return *entry;
                }
                const Entry &operator*() const
                {
                    return *entry;
                }

                Entry *getEntry() const
                {
                    return entry;
                }

                Node *getNode() const
                {
                    if (entry == nullptr)
                        return nullptr;
                    return entry->node;
                }

                Position left() const
                {
                    if (!entry->node->left)
                        return Position();
                    return Position(entry->node->left->entry);
                }
                Position right() const
                {
                    if (!entry->node->right)
                        return Position();
                    return Position(entry->node->right->entry);
                }
                Position parent() const
                {
                    if (!entry->node->parent)
                        return Position();
                    return Position(entry->node->parent->entry);
                }
                bool isRoot() const
                {
                    return !entry->node->parent;
                }
                bool isLeaf() const
                {
                    return !left().entry && !right().entry;
                }

                friend std::ostream &operator<<(std::ostream &os,
                                                const Position &p)
                {
                    os << *p;
                    return os;
                }

            private:
                Entry *entry = nullptr;
        };

        typedef std::list<Position> PositionList;

    private:
        Node *root;
        int nodeCount;

    public:
        LinkedHeapTree<Key, Value>() : nodeCount(0), root(NULL)
        {
        }

        int size() const
        {
            return nodeCount;
        }

        bool empty() const
        {
            return !root;
        }

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
            // We shift right once to find the path to the indice
            // just above the final indice.
            int parentIndex = lastIndex >> 1;

            Node *parent    = findNode(parentIndex);
            newNode->parent = parent;
            // If last index ends with 0 then it goes left
            if ((lastIndex & 1) == 0)
                parent->left = newNode;
            else
                parent->right = newNode;

            upHeapBubble(Position(newEntry));
            return Position(newEntry);
        }

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

            // Checks if the node we want to remove is also the last
            // node
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

            // Now the original position is the last node and the
            // last node is now the original position

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

        Value removeMin()
        {
            if (!root)
                throw std::runtime_error("Heap empty");
            return remove(Position(root->entry));
        }

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

        Position min() const
        {
            return Position(root->entry);
        }

        // Based on inorder traversal
        void prettyPrint(const Position &rt, std::string prefix = "",
                         bool isLeft = true)
        {
            if (rt.right().getEntry())
            {

                prettyPrint(rt.right(), prefix + (isLeft ? "│   " : "    "),
                            false);
            }

            std::cout << prefix;
            std::cout << (isLeft ? "└── " : "┌── ");
            std::cout << rt << std::endl;

            if (rt.left().getEntry())
            {
                prettyPrint(rt.left(), prefix + (isLeft ? "    " : "│   "),
                            true);
            }
        }

        void swap(const Position &p1, const Position &p2)
        {
            Entry *p1OriginalEntry = p1.getEntry();
            Node *p1OriginalNode   = p1.getNode();
            Entry *p2OriginalEntry = p2.getEntry();
            Node *p2OriginalNode   = p2.getNode();

            // p1 has p2's entry but not its correct associated node
            p1OriginalNode->entry = p2OriginalEntry;
            p2OriginalNode->entry = p1OriginalEntry;

            // Goes to old node through the new entry to set it from
            // the old to it to itself because the entries node has
            // still not been updated
            p1OriginalNode->entry->node = p1OriginalNode;
            p2OriginalNode->entry->node = p2OriginalNode;
        }

    protected:
        // Uses a bitmask to determine where in the binary tree the last
        // node is. This is done because the binary tree in itself is a
        // representation of binary numbers, thus every bit can be
        // resolved as a decision between left (0) and right (1)
        Node *findNode(int index)
        {
            Node *current = root;
            int mask      = 1;

            // Mask begins at 0000... 1, this the left 1 until it
            // matches the magnitude of the index
            while (mask <= index)
                mask <<= 1;
            // The function above overshoots by 1 order of
            // magnitude, but we shift right twice to also get rid
            // of the leading 1 which represents the root itself
            mask >>= 2;

            // We continously check which branch to take to go by
            // bitwise anding against the index. Then shifting right
            // to switch the position of the and. This will tell us
            // wether the bit we are on will resolve to a left or
            // right decision.
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
