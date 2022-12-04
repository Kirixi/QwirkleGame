
#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include "Tile.h"

#include <fstream>
#include <memory>
#include <vector>

using std::vector;
using std::shared_ptr;
using std::make_shared;
using std::ofstream;

class GameBoard {
public:
    /**
     * Construct the board with a 2d vector of default values.
     */
    GameBoard();

    /**
     * Destructor to free up all the tiles placed.
     */
    ~GameBoard();

    /**
     * Place a tile on the board at the given location.
     *
     * @param tile - the tile to be placed
     * @param row,column - the location
     */
    void placeTile(const shared_ptr<Tile>& tile, size_t row, size_t column);

    /**
     * Query the tile at the given location.
     *
     * @param row,column - the location
     * @return a pointer to the tile at the location
     */
    shared_ptr<Tile> at(size_t row, size_t column) const;

    /**
     * Query whether the board is empty.
     *
     * @return true if board is empty
     */
    bool isEmpty() const;

    /**
     * Overloaded output stream operator for easy printing.
     *
     * @param os - a reference to the output stream
     * @param board - a reference to the board to be printed
     */
    friend ostream& operator<<(ostream& os, const GameBoard& board);

    /**
     * Overloaded file output stream operator for printing to a file.
     *
     * @param ofs - a reference to the file output stream
     * @param board - a reference to the board to be printed
     */
    friend ofstream& operator<<(ofstream& ofs, const GameBoard& board);

private:
    size_t tileCount;
    vector<vector<shared_ptr<Tile>>> board;
};

#endif // !GAME_BOARD_H
