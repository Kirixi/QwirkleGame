
#ifndef ASSIGN2_NODE_H
#define ASSIGN2_NODE_H

#include "Tile.h"

#include <memory>

using std::shared_ptr;
using std::make_shared;

class Node {
public:
    /**
     * Construct a node holding a pointer to a tile.
     */
    explicit Node(const shared_ptr<Tile>& tile);

    /**
     * Construct with reference to next and previous nodes besides the tile.
     */
    Node(
        const shared_ptr<Tile>& tile,
        const shared_ptr<Node>& next,
        const shared_ptr<Node>& previous);

    /**
     * Destructor to free up the owning tile.
     */
    ~Node();

    /**
     * Check for equality of two nodes based on the underlying tiles.
     */
    friend bool operator==(const Node& node1, const Node& node2);

    shared_ptr<Tile> tile;
    shared_ptr<Node> next;
    shared_ptr<Node> previous;
};

#endif // ASSIGN2_NODE_H
