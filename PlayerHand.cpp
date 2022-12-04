
#include "PlayerHand.h"

using std::invalid_argument;

PlayerHand::PlayerHand() : tiles(make_shared<LinkedList>()) {}

PlayerHand::PlayerHand(const shared_ptr<LinkedList>& tiles) : tiles(tiles) {}

PlayerHand::PlayerHand(const PlayerHand& hand) : tiles(hand.tiles) {}

PlayerHand::~PlayerHand() { tiles.reset(); }

void PlayerHand::addTile(shared_ptr<Tile> tile) { tiles->addBack(tile); }

shared_ptr<Tile> PlayerHand::playTile(const Tile& tile) {
    shared_ptr<Tile> toPlay = nullptr;

    try {
        toPlay = getTile(tile);
        tiles->remove(toPlay);
    } catch (invalid_argument& exception) {
        throw invalid_argument("invalid tile for PlayerHand::playTile");
    }

    return toPlay;
}

void PlayerHand::replaceTile(const Tile& tile, TileBag& bag) {
    try {
        shared_ptr<Tile> fromHand = getTile(tile);
        shared_ptr<Tile> fromBag = bag.replace(fromHand);
        tiles->remove(fromHand);
        tiles->addBack(fromBag);
    } catch (invalid_argument& exception) {
        throw invalid_argument("invalid tile for PlayerHand::replaceTile");
    }
}

shared_ptr<Tile> PlayerHand::getTile(const Tile& tile) {
    bool found = false;
    shared_ptr<Tile> currentTile = nullptr;
    shared_ptr<Tile> toReturn = nullptr;

    for (unsigned int i = 0; i < tiles->size() && !found; ++i) {
        currentTile = tiles->at(i);
        if (tile == *currentTile) {
            toReturn = currentTile;
            found = true;
        }
    }

    if (!found)
        throw invalid_argument("invalid tile for PlayerHand::getTile");

    return toReturn;
}

shared_ptr<LinkedList> PlayerHand::getTiles() { return tiles; }

ostream& operator<<(ostream& os, const PlayerHand& hand) {
    os << *hand.tiles;
    return os;
}
