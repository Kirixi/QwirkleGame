#include "GameManager.h"
#include "IOHandler.h"
#include "Constants.h"

using std::invalid_argument;
using std::out_of_range;

shared_ptr<TileBag> GameManager::bag = nullptr;
shared_ptr<Player> GameManager::player1 = nullptr;
shared_ptr<Player> GameManager::player2 = nullptr;
shared_ptr<Player> GameManager::currentPlayer = nullptr;
shared_ptr<GameBoard> GameManager::board = nullptr;

/**
 * Initialise the board, tilebag and the players for the game.
 *
 * @param player1Name - name of the first player
 * @param player2name - name of the second player
 */
void GameManager::beginGame(
    const string& player1Name, const string& player2Name) {
    bag = make_shared<TileBag>();
    bag->fill();
    bag->shuffle();

    player1 = make_shared<Player>(player1Name, bag->getHand());
    player2 = make_shared<Player>(player2Name, bag->getHand());
    currentPlayer = player1;

    board = make_shared<GameBoard>();
}

void GameManager::loadGame(const shared_ptr<Player>& player1,
    const shared_ptr<Player>& player2, const shared_ptr<TileBag>& loadedBag,
    const shared_ptr<GameBoard>& loadedBoard,
    const shared_ptr<Player>& currentPlayer) {

    bag = loadedBag;
    board = loadedBoard;

    GameManager::player1 = player1;
    GameManager::player2 = player2;
    GameManager::currentPlayer = currentPlayer;
}

/**
 * Place a tile on to the board from the current player's hand.
 *
 * @param colour,shape - the tile attributes
 * @param row,column - the specified grid location
 */
void GameManager::placeTile(Colour colour, Shape shape, size_t row, size_t column) {
    string message = "Tile placed successfully.";
    State state = PLACE_SUCCESS;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = "The specified tile is not present in hand.";
            throw invalid_argument("");
        } else if (!isGridLocationEmpty(row, column)) {
            message =
                "A tile is already present in the provided grid location.";
            throw invalid_argument("");
        }

        Lines lines = getAdjacentLines(tile, row, column);
        if (!board->isEmpty() && !hasAdjacentTile(tile, lines)) {
            message = "No adjacent tile to form line.";
            throw invalid_argument("");
        } else if (!isTileValidOnLine(tile, lines)) {
            message = "Tile violates line rules.";
            throw invalid_argument("");
        }

        // score for the first round
        if (board->isEmpty())
            currentPlayer->setScore(1);

        board->placeTile(currentPlayer->getHand()->playTile(tile), row, column);
        if (!bag->getTiles()->isEmpty()) {
            currentPlayer->getHand()->addTile(bag->getTiles()->at(FIRST_POSITION));
            bag->getTiles()->removeFront();
        }

        updateScore(lines);
        GameManager::switchPlayer();
    } catch (...) {
        state = PLACE_FAILURE;
    }

    IOHandler::notify(message, state);
    if (hasGameEnded())
        IOHandler::notify("", GAME_OVER);
}

/**
 * Replace the tile from the current player's hand.
 *
 * @param colour,shape - the tile attributes
 */
void GameManager::replaceTile(Colour colour, Shape shape) {
    string message = "Tile replaced successfully.";
    State state = REPLACE_SUCCESS;

    try {
        Tile tile(colour, shape);
        if (!isTileInHand(tile)) {
            message = "The specified tile is not present in hand.";
            throw invalid_argument("");
        } else if (board->isEmpty()) {
            message = "Must place a tile on the first move.";
            throw invalid_argument("");
        }

        if (!bag->getTiles()->isEmpty()) {
            currentPlayer->getHand()->replaceTile(tile, *bag);
            GameManager::switchPlayer();
        } else {
            message = "No more tiles remain to be replaced.";
            throw out_of_range("");
        }
    } catch (...) {
        state = REPLACE_FAILURE;
    }

    IOHandler::notify(message, state);
}

/**
 * Switch player at the end of a round.
 */
void GameManager::switchPlayer() {
    currentPlayer = (*currentPlayer == *player1) ? player2 : player1;
}

/**
 * Check if location in board is empty for tile being placed.
 *
 * @param row,column - the specified grid location
 */
bool GameManager::isGridLocationEmpty(size_t row, size_t column) {
    return board->at(row, column) == nullptr;
}

/**
 * Check if the player hand contains a tile.
 *
 * @param tile - the tile to be checked
 */
bool GameManager::isTileInHand(const Tile& tile) {
    return currentPlayer->getHand()->getTiles()->contains(tile);
}

/**
 * Gets the adjacent lines formed by the tile if placed at the specified
 * location.
 *
 * @return Lines (pair<LinkedList, LinkedList) for the horizontal and vertical
 * lines
 * */
Lines GameManager::getAdjacentLines(const Tile& tile, size_t row, size_t column) {
    Lines lines = make_pair(LinkedList{}, LinkedList{});

    size_t currentRow = row;
    size_t currentColumn = column;
    int currentDirection = UP;
    bool allDirectionTraversed = false;

    // only traverse as many times as there are tiles in all directions
    while (!allDirectionTraversed) {
        if (currentDirection == UP)
            --currentRow;
        else if (currentDirection == DOWN)
            ++currentRow;
        else if (currentDirection == LEFT)
            --currentColumn;
        else if (currentDirection == RIGHT)
            ++currentColumn;

        try {
            shared_ptr<Tile> other = board->at(currentRow, currentColumn);

            /*
             * if location is empty (nullptr), proceed to traverse a different
             * direction else keep track of traversed tiles
             */
            if (other != nullptr) {
                if (currentDirection == UP || currentDirection == DOWN)
                    lines.second.addBack(other);
                else
                    lines.first.addBack(other);
            } else {
                throw out_of_range("");
            }
        }
        // exception may be thrown by board if the coordinates are out of bounds
        catch (out_of_range& exception) {
            currentRow = row;
            currentColumn = column;
            ++currentDirection;
            allDirectionTraversed = currentDirection > RIGHT;
        }
    }

    return lines;
}

/**
 * Check the surrounding grid locations if an adjacent tile is present to be
 * able to form a line.
 *
 * @param tile - the tile to be checked
 * @param lines - the horizontal and vertical lines
 * @return boolean indicating whether the tile has adjacent neighbors
 */
bool GameManager::hasAdjacentTile(const Tile& tile, const Lines& lines) {
    return !lines.first.isEmpty() || !lines.second.isEmpty();
}

/**
 * Check if the tile being placed violate the following rules.
 * - No more than 6 tiles per line.
 * - No same tile more than once per line.
 * - Tile has either same colour or shape with both the horizontal and vertical
 *   lines.
 *
 * @param tile - the tile to be placed
 * @param lines - the horizontal and vertical lines
 * @return boolean indicating whether the tile can be placed on line
 */
bool GameManager::isTileValidOnLine(const Tile& tile, const Lines& lines) {
    const LinkedList& horizontalTiles = lines.first;
    const LinkedList& verticalTiles = lines.second;

    bool tileUnique =
        !horizontalTiles.contains(tile) && !verticalTiles.contains(tile);

    /*
     * check whether tile shares similarity with the other tiles on both the
     * horizontal and vertical lines, is only traversed as long as one of the
     * similarities remain, if more than 2 comparisons are made and conditions
     * still hold then no further iteration is made as the tile cannot have
     * dissimilarities any longer, the large number of conditions are there to
     * ensure that the algorithm is as efficient as could be
     */
    bool hasSameColourAsLine = true;
    bool hasSameShapeAsLine = true;

    for (size_t i = 0; i < horizontalTiles.size() &&
         (hasSameColourAsLine || hasSameShapeAsLine) && i < 2;
         ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*horizontalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*horizontalTiles.at(i));
    }

    bool matchHorizontalLine = hasSameColourAsLine || hasSameShapeAsLine;
    hasSameColourAsLine = true;
    hasSameShapeAsLine = true;

    // only iterate if the tile matched the horizontal line above
    for (size_t i = 0; i < verticalTiles.size() && matchHorizontalLine &&
         (hasSameColourAsLine || hasSameShapeAsLine) && i < 2;
         ++i) {
        hasSameColourAsLine &= tile.hasSameColour(*verticalTiles.at(i));
        hasSameShapeAsLine &= tile.hasSameShape(*verticalTiles.at(i));
    }

    bool matchVerticalLine = hasSameColourAsLine || hasSameShapeAsLine;

    return tileUnique && matchHorizontalLine && matchVerticalLine &&
        horizontalTiles.size() < MAX_LINE_SIZE &&
        verticalTiles.size() < MAX_LINE_SIZE;
}

/**
 * Update the score for a round based on the formed lines.
 *
 * @param lines - the horizontal and vertical lines
 */
void GameManager::updateScore(const Lines& lines) {
    size_t horizontalScore =
        lines.first.isEmpty() ? 0 : lines.first.size() + 1;
    size_t verticalScore =
        lines.second.isEmpty() ? 0 : lines.second.size() + 1;

    // qwirkle is printed twice if it happens twice on the same move
    if (horizontalScore == MAX_LINE_SIZE) {
        IOHandler::notify("QWIRKLE!!!", QWIRKLE);
    }
    if (verticalScore == MAX_LINE_SIZE) {
        IOHandler::notify("QWIRKLE!!!", QWIRKLE);
    }

    // bonus points for emptying hand or scoring qwirkle
    int bonusCount = 0;
    if (player1->getHand()->getTiles()->isEmpty())
        ++bonusCount;
    if (horizontalScore == MAX_LINE_SIZE)
        ++bonusCount;
    if (verticalScore == MAX_LINE_SIZE)
        ++bonusCount;

    currentPlayer->setScore(currentPlayer->getScore()
        + horizontalScore + verticalScore + SCORE_BONUS * bonusCount);
}

/**
 * Check if conditions hold for game ending.
 */
bool GameManager::hasGameEnded() {
    return (player1->getHand()->getTiles()->isEmpty() ||
       player2->getHand()->getTiles()->isEmpty()) && bag->getTiles()->isEmpty();
}

/**
 * Resets the data structures for a new game.
 */
void GameManager::resetGame() {
    board.reset();
    bag.reset();
    player1.reset();
    player2.reset();
    currentPlayer.reset();
}
