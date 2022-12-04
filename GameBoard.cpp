#include "GameBoard.h"
#include "Constants.h"

#include <iomanip>
#include <iostream>
#include <string>

using std::left;
using std::right;
using std::setw;
using std::endl;
using std::string;
using std::invalid_argument;
using std::out_of_range;

GameBoard::GameBoard() : tileCount(0) {
    for (size_t i = 0; i < BOARD_LENGTH; ++i) {
        board.push_back(vector<shared_ptr<Tile>>());
        board.at(i).resize(BOARD_LENGTH, nullptr);
    }
}

GameBoard::~GameBoard() {
    for (size_t i = 0; i < BOARD_LENGTH; ++i)
        board.at(i).clear();
}

void GameBoard::placeTile(
    const shared_ptr<Tile>& tile, size_t row, size_t column) {
    if (row >= BOARD_LENGTH || column >= BOARD_LENGTH)
        throw out_of_range("invalid grid location for GameBoard::placeTile");

    board.at(row).at(column) = tile;
    ++tileCount;
}

shared_ptr<Tile> GameBoard::at(size_t row, size_t column) const {
    if (row >= BOARD_LENGTH || column >= BOARD_LENGTH)
        throw out_of_range("invalid grid location for GameBoard::at");

    return board.at(row).at(column);
}

bool GameBoard::isEmpty() const { return tileCount == 0; }

ostream& operator<<(ostream& os, const GameBoard& gameBoard) {
    // print the column header
    os << left << setw(3) << " ";
    for (int i = 0; i < BOARD_LENGTH; ++i) os << setw(3) << i;
    os << endl;

    // print the line under the column header
    os << setw(2) << " ";
    for (int i = 0; i < BOARD_LENGTH * 3 + 1; ++i) os << "-";
    os << endl;

    // print each row
    for (int i = 0; i < BOARD_LENGTH; ++i) {
        os << (char)(i + ASCII_ALPHABET_BEGIN) << " |";

        for (int j = 0; j < BOARD_LENGTH; ++j) {
            if (gameBoard.board.at(i).at(j)) os << *gameBoard.board.at(i).at(j);
            else os << "  ";
            os << "|";
        }

        os << endl;
    }

    return os;
}

ofstream& operator<<(ofstream& ofs, const GameBoard& gameBoard) {
    bool first = true;
    for (unsigned int i = 0; i < BOARD_LENGTH; ++i) {
        for (unsigned int j = 0; j < BOARD_LENGTH; ++j) {
            shared_ptr<Tile> tile = gameBoard.board.at(i).at(j);
            if (tile != nullptr) {
                // print each row and column in specified "Tile@Location" format
                ofs << (first ? "" : ", ") << *tile << "@"
                    << (char)(i + ASCII_ALPHABET_BEGIN) << j;

                first = false;
            }
        }
    }

    return ofs;
}
