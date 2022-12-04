
#ifndef ASSIGN1_TILECODES_H
#define ASSIGN1_TILECODES_H

#include "Tile.h"

#include <array>

using std::array;

// Colours
#define RED 'R'
#define ORANGE 'O'
#define YELLOW 'Y'
#define GREEN 'G'
#define BLUE 'B'
#define PURPLE 'P'

// Shapes
#define CIRCLE 1
#define STAR_4 2
#define DIAMOND 3
#define SQUARE 4
#define STAR_6 5
#define CLOVER 6

/**
 * @note
 * Global variables made static, as well as const according to the style guide.
 */
static const array<Colour, 6> COLOURS{
    RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};
static const array<Shape, 6> SHAPES{
    CIRCLE, STAR_4, DIAMOND, SQUARE, STAR_6, CLOVER};

#endif // ASSIGN1_TILECODES_H
