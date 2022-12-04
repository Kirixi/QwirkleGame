
#include "Tile.h"

using std::invalid_argument;

Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

Colour Tile::getColour() const { return colour; }

Shape Tile::getShape() const { return shape; }

bool Tile::hasSameColour(const Tile& other) const {
    return colour == other.colour;
}

bool Tile::hasSameShape(const Tile& other) const {
    return shape == other.shape;
}

bool Tile::operator==(const Tile& other) const {
    return hasSameColour(other) && hasSameShape(other);
}

ostream& operator<<(ostream& os, const Tile& tile) {
    os << tile.colour << tile.shape;
    return os;
}
