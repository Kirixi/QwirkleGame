
#include "LinkedList.h"

using std::out_of_range;

LinkedList::LinkedList() : length(0), head(nullptr), tail(nullptr) {}

LinkedList::~LinkedList() { while (length != 0) removeFront(); }

void LinkedList::addFront(const shared_ptr<Tile>& tile) {
    shared_ptr<Node> toBeAdded = make_shared<Node>(tile);
    if (length == 0) {
        head = toBeAdded;
        tail = toBeAdded;
    } else {
        head->previous = toBeAdded;
        toBeAdded->next = head;
        head = toBeAdded;
    }

    ++length;
}

void LinkedList::addBack(const shared_ptr<Tile>& tile) {
    shared_ptr<Node> toBeAdded = make_shared<Node>(tile);
    if (length == 0) {
        head = toBeAdded;
        tail = toBeAdded;
    } else {
        tail->next = toBeAdded;
        toBeAdded->previous = tail;
        tail = toBeAdded;
    }

    ++length;
}

void LinkedList::removeFront() {
    if (length == 1) {
        head.reset();
        tail.reset();
    } else {
        head = head->next;
        head->previous.reset();
    }

    --length;
}

void LinkedList::removeBack() {
    if (length == 1) {
        head.reset();
        tail.reset();
    } else {
        tail = tail->previous;
        tail->next.reset();
    }

    --length;
}

void LinkedList::insert(
    const shared_ptr<Tile>& tile, size_t index, bool replace) {
    if (index >= length)
        throw out_of_range("invalid index for LinkedList::insert");

    shared_ptr<Node> toBeInserted = make_shared<Node>(tile);
    shared_ptr<Node> nodeAtIndex = head;
    for (size_t i = 0; i < index; ++i)
        nodeAtIndex = nodeAtIndex->next;

    if (replace) {
        /*
        when replacing, update new node's references to point to current node's
        next and previous nodes, also update the next and previous nodes'
        references to point to the new node
        */
        toBeInserted->next = nodeAtIndex->next;
        toBeInserted->previous = nodeAtIndex->previous;

        // if replacing head or tail, update references of head and tail
        if (index == 0) {
            nodeAtIndex->next->previous = toBeInserted;
            head = toBeInserted;
        } else if (index == length - 1) {
            nodeAtIndex->previous->next = toBeInserted;
            tail = toBeInserted;
        } else {
            nodeAtIndex->next->previous = toBeInserted;
            nodeAtIndex->previous->next = toBeInserted;
        }

        nodeAtIndex.reset();
    } else {
        if (index == 0) {
            toBeInserted->next = nodeAtIndex;
            nodeAtIndex->previous = toBeInserted;
            head = toBeInserted;
        } else if (index == length - 1) {
            toBeInserted->previous = nodeAtIndex;
            nodeAtIndex->next = toBeInserted;
            tail = toBeInserted;
        } else {
            toBeInserted->next = nodeAtIndex;
            toBeInserted->previous = nodeAtIndex->previous;
            nodeAtIndex->previous->next = toBeInserted;
            nodeAtIndex->previous = toBeInserted;
        }

        ++length;
    }
}

void LinkedList::remove(const shared_ptr<Tile>& tile) {
    shared_ptr<Node> current = head;
    bool removed = false;
    size_t i = 0;
    while (i < length && !removed) {
        if (*current->tile == *tile) {
            remove(i);
            removed = true;
        } else {
            current = current->next;
            ++i;
        }
    }
}

void LinkedList::remove(size_t index) {
    if (index >= length)
        throw out_of_range("invalid index for LinkedList::remove");

    if (index == 0)
        removeFront();
    else if (index == length - 1)
        removeBack();
    else {
        shared_ptr<Node> toBeDeleted = head;
        for (size_t i = 0; i < index; ++i)
            toBeDeleted = toBeDeleted->next;

        toBeDeleted->next->previous = toBeDeleted->previous;
        toBeDeleted->previous->next = toBeDeleted->next;
        --length;
    }
}

size_t LinkedList::size() const { return length; }

shared_ptr<Tile> LinkedList::at(size_t index) const {
    if (index >= length)
        throw out_of_range("invalid index for LinkedList::at");

    shared_ptr<Node> toReturn = head;
    for (size_t i = 0; i < index; ++i)
        toReturn = toReturn->next;

    return toReturn->tile;
}

bool LinkedList::contains(const Tile& tile) const {
    shared_ptr<Node> current = head;
    bool found = false;
    size_t i = 0;
    while (i < length && !found) {
        if (*current->tile == tile) {
            found = true;
        } else {
            current = current->next;
            ++i;
        }
    }

    return found;
}

bool LinkedList::isEmpty() const { return length == 0; }

ostream& operator<<(ostream& os, const LinkedList& list) {
    shared_ptr<Node> current = list.head;
    for (size_t i = 0; i < list.length; ++i) {
        os << *current->tile << (i < list.length - 1 ? "," : "");
        current = current->next;
    }

    return os;
}
