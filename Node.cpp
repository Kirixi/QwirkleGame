
#include "Node.h"

Node::Node(const shared_ptr<Tile>& tile)
    : tile(tile), next(nullptr), previous(nullptr) {}

Node::~Node() {
    tile.reset();
    next.reset();
    previous.reset();
}

Node::Node(
    const shared_ptr<Tile>& tile,
    const shared_ptr<Node>& next,
    const shared_ptr<Node>& previous) {
    this->tile = tile;
    this->next = next;
    this->previous = previous;
}

bool operator==(const Node& node1, const Node& node2) {
    return *node1.tile == *node2.tile;
}
