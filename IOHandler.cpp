#include "IOHandler.h"
#include "TileCodes.h"
#include "Constants.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

bool IOHandler::gameRunning = false;
bool IOHandler::takingInput = false;

void IOHandler::beginGame() {
    cout << "Welcome to Qwirkle!" << endl;
    cout << "-------------------" << endl;
    cout << " " << endl;
    selection();
}

void IOHandler::prompt() { cout << "> "; }

void IOHandler::selection() {

    cout << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;

    bool flag = true;
    while (!cin.eof() && flag) {
        int option = 0;
        cout << " " << endl;
        prompt();
        cin >> option;
        try {
            if (option == 1) {
                newGame();
                flag = false;
            } else if (option == 2) {
                loadGame();
                flag = false;
            } else if (option == 3) {
                credits();
                flag = false;
            } else if (option == 4 && !cin.eof()) {
                quit();
                flag = false;
            } else if (cin.eof()) {
                quit();
            } else {
                cin.clear();
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                cout << " " << endl;
                throw std::runtime_error("Invalid input.");
            }
        } catch (const std::runtime_error& e) {
            cerr << e.what() << endl;
            cout << endl;
        }
    }
}

void IOHandler::newGame() {
    string player1Name;
    string player2Name;

    cout << "Starting a New Game" << endl;
    cout << endl;
    bool nameCheck1 = true;
    bool nameCheck2 = true;
    string message = "Must enter a name in CAPS for Player and name must "
                     "not contain numbers or symbols or duplicate names";
    while (!cin.eof() && nameCheck1) {
        cout << "Enter a name for player 1 (uppercase characters only)" << endl;
        prompt();
        cin >> player1Name;
        if (validateName(player1Name)) {
            cout << message << endl;
        } else {
            nameCheck1 = false;
        }
    }
    while (!cin.eof() && nameCheck2) {
        cout << "Enter a name for player 2 (uppercase characters only)" << endl;
        prompt();
        cin >> player2Name;
        if (validateName(player2Name) || player1Name == player2Name) {
            cout << message << endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            nameCheck2 = false;
        }
    }
    if (cin.eof()) {
        quit();
    }
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Let's Play!" << endl;
    GameManager::beginGame(player1Name, player2Name);
    gameRunning = true;
}

bool IOHandler::validateName(const string& name) {

    bool state = false;
    int counter = 0;
    for (char c : name) {
        if (isupper(c) < 1) {
            counter++;
        }
        if (counter >= 1) {
            state = true;
        }
    }
    return state;
}

void IOHandler::playRound() {
    cout << endl;
    cout << GameManager::currentPlayer->getName() << ", it's your turn "
         << endl;
    cout << "Score for " << GameManager::player1->getName() << ": "
         << GameManager::player1->getScore() << endl;
    cout << "Score for " << GameManager::player2->getName() << ": "
         << GameManager::player2->getScore() << endl;
    cout << endl;
    cout << *GameManager::board << endl;
    cout << "Your hand is " << endl;
    cout << *GameManager::currentPlayer->getHand() << endl;

    takingInput = true;
    while (takingInput) {
        prompt();
        string temp, operation, tile, keywordAT, pos, saveName;
        getline(cin, temp);
        std::istringstream command(temp);
        if (cin.eof()) {
            quit();
            cin.clear();
        }
        command >> operation >> tile >> keywordAT >> pos;

        saveName = tile;

        transform(
            operation.begin(), operation.end(), operation.begin(), ::tolower);
        transform(tile.begin(), tile.end(), tile.begin(), ::toupper);
        transform(
            keywordAT.begin(), keywordAT.end(), keywordAT.begin(), ::tolower);
        transform(pos.begin(), pos.end(), pos.begin(), ::toupper);

        if(operation == "save"){
            takingInput = logicHandler(operation, saveName, keywordAT, pos);
        } else {
            takingInput = logicHandler(operation, tile, keywordAT, pos);
        }
    }
}

bool IOHandler::logicHandler(const string& operation, const string& tile,
    const string& keywordAT, const string& pos) {
    string gameFileName = tile;

    if (operation == "place" && keywordAT == "at") {
        if (checkTile(tile) && checkTilePosition(pos)) {
            placeTile(tile, pos);
        } else {
            takingInput = true;
        }

    } else if (operation == "replace" && keywordAT.empty() && pos.empty()) {
        if (checkTile(tile)) {
            replaceTile(tile);
        } else {
            takingInput = true;
        }

    } else if (operation == "save") {
        gameFileName = gameFileName + ".save";
        std::ofstream file(gameFileName);
        file << GameManager::player1->getName() << endl;
        file << GameManager::player1->getScore() << endl;
        file << *GameManager::player1->getHand()->getTiles() << endl;
        file << GameManager::player2->getName() << endl;
        file << GameManager::player2->getScore() << endl;
        file << *GameManager::player2->getHand()->getTiles() << endl;
        file << BOARD_LENGTH << "," << BOARD_LENGTH << endl;
        file << *GameManager::board << endl;
        file << *GameManager::bag->getTiles() << endl;
        file << GameManager::currentPlayer->getName() << endl;
        file.close();
        cout << endl;
        cout << "Game successfully saved" << endl;
        cout << endl;
        takingInput = true;
    } else if (operation == "quit") {
        quit();
        takingInput = false;
    } else {
        cout << ERROR_MESSAGE << "Not a valid command." << endl;
    }
    return takingInput;
}

void IOHandler::credits() {
    cout << "----------------------------------" << endl;
    cout << "Name : Shihab Sami" << endl;
    cout << "Student ID : S3823710" << endl;
    cout << "Email : S3823710@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Harrison Lorraway" << endl;
    cout << "Student ID : S3856166" << endl;
    cout << "Email : S3856166@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Yeu Haw Teh" << endl;
    cout << "Student ID : S3813866" << endl;
    cout << "Email : S3813866@student.rmit.edu.au" << endl;

    cout << endl;

    cout << "Name : Andrew Nhan Trong Tran" << endl;
    cout << "Student ID : S3785952" << endl;
    cout << "Email : S3785952@student.rmit.edu.au" << endl;
    cout << "----------------------------------" << endl;
    selection();
}

void IOHandler::loadGame() {
    bool fileCheck = true;
    while(fileCheck) {
        try {
    string filename;
    cout << "Enter the filename from which to load a game." << endl;
    prompt();
    cin >> filename;
    std::ifstream file(filename);

        if (cin.eof()) {
            quit();
        }

        if (!file) {
            throw::std::invalid_argument("File does not exits.");
        }
        if (is_empty(file)){
            throw::std::invalid_argument("File Is Empty!.");
        }

        cin.clear();
        cin.ignore();

        shared_ptr<PlayerHand> p1Hand = make_shared<PlayerHand>();
        shared_ptr<PlayerHand> p2Hand = make_shared<PlayerHand>();
        shared_ptr<Player> p1 = make_shared<Player>("Player1", p1Hand);
        shared_ptr<Player> p2 = make_shared<Player>("Player2", p2Hand);
        shared_ptr<TileBag> tileBag = make_shared<TileBag>();
        shared_ptr<GameBoard> board = make_shared<GameBoard>();
        shared_ptr<Player> currentPlayer = nullptr;

        int count = 0;
        string text;
            while (getline(file, text)) {
                text.erase(
                    std::remove(text.begin(), text.end(), '\r'), text.end());
                text.erase(
                    std::remove(text.begin(), text.end(), '\n'), text.end());
                if (count == 0 || count == 3 || count == 9) {
                    for (unsigned i = 0; i < text.length() - 1; i++) {
                        int ascii =  static_cast<unsigned char>(text[i]);
                        if (ascii < ASCII_ALPHABET_BEGIN - 1 ||
                            ascii > ASCII_ALPHABET_END + 1 || ascii == 0) {
                            throw std::invalid_argument(
                                "Name format is not part of ASCII text.");
                        }
                    }

                    if (count == 0) {
                        p1->setName(text);
                    } else if (count == 3) {
                        p2->setName(text);
                    } else {
                        if (p1->getName() == text) {
                            currentPlayer = p1;
                        } else {
                            currentPlayer = p2;
                        }
                    }

                    count++;
                } else if (count == 1 || count == 4) {
                    int number = stoi(text);
                    if (number < 0) {
                        throw std::invalid_argument(
                            "The number should be positive.");
                    }

                    if (count == 1) {
                        p1->setScore(number);
                    } else {
                        p2->setScore(number);
                    }

                    count++;
                } else if (count == 2 || count == 5 || count == 8) {
                    std::stringstream ss(text);
                    string c = "\0";
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        if (text != c) {
                            if (substr.length() != 2) {
                                throw std::invalid_argument(
                                    "Wrong tile list format.");
                            }
                        }
                        if (count == 2) {
                            p1Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        }
                        else if (count == 5) {
                            p2Hand->addTile(
                                make_shared<Tile>(substr[0], substr[1] - '0'));
                        }
                        else {
                            if (!substr.empty()) {
                                tileBag->getTiles()->addBack(make_shared<Tile>(
                                    substr[0], substr[1] - '0'));
                            }
                        }
                    }
                    count++;
                } else if (count == 6) {
                    std::stringstream ss(text);
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ',');
                        int number = std::stoi(substr);
                        if (number < 0 || (number > 26)) {
                            throw std::invalid_argument(
                                "The grid should be more than 0 and less that "
                                "26.");
                        }
                    }
                    count++;
                } else if (count == 7) {
                    std::stringstream ss(text);
                    string c = "\0";
                    while (ss.good()) {
                        string substr;
                        getline(ss, substr, ' ');
                        const char at = '@';
                        if (substr != c) {
                            if (substr[2] != at) {
                                throw std::invalid_argument(
                                    "The board should appear as a list of "
                                    "tile@postion.");
                            }
                        }
                        char last = substr.length() < 6 ? ',' : substr[5];
                        char tile[2] = {substr[0], substr[1]};
                        char pos[3] = {substr[3], substr[4], last};

                        int row = pos[0] - ASCII_ALPHABET_BEGIN;
                        int column =
                            (pos[2] == 44 || pos[2] == '\r' || pos[2] == '\n')
                            ? pos[1] - '0'
                            : (int)(pos[1] - '0') * 10 + (int)(pos[2] - '0');
                        if (!substr.empty()) {
                            board->placeTile(
                                make_shared<Tile>(tile[0], tile[1] - '0'), row,
                                column);
                        }
                    }
                    count++;
                }
            }
            cout << "Qwirkle game successfully loaded." << endl;
            file.close();
            GameManager::loadGame(p1, p2, tileBag, board, currentPlayer);
            gameRunning = true;
            fileCheck = false;

        } catch (const std::invalid_argument& e) {
            cerr << "The error is " << e.what() << endl;
        }
    }
}

bool IOHandler::is_empty(std::ifstream& file){
    return file.peek() == std::ifstream::traits_type::eof();
}

bool IOHandler::checkTile(const string& tile) {
    bool condition = false;
    bool boolLetter = false;
    bool boolNumber = false;
    try {
        string appended;
        if (tile.size() == STRING_SIZE_2) {
            char letter = tile.at(0);
            char num1 = tile.at(1);

            appended.append(1, num1);
            int combinedNumber = stoi(appended);
            for (char i : COLOURS) {
                if (letter == i) {
                    boolLetter = true;
                }
            }
            for (int i : SHAPES) {
                if (combinedNumber == i) {
                    boolNumber = true;
                }
            }
        }
        if (boolLetter && boolNumber) {
            condition = true;
        } else {
            cout << ERROR_MESSAGE "Not a valid tile." << endl;
        }
    } catch (const std::invalid_argument& e) {
        cout << ERROR_MESSAGE "Not a valid tile." << endl;
        condition = false;
    }
    return condition;
}

bool IOHandler::checkTilePosition(const string& position) {
    string appended;
    bool condition = false, boolLetter = false, boolNumber = false;

    try {
        if (position.size() == STRING_SIZE_2) {
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            int asciiLetter = static_cast<unsigned char>(int(letter));
            if (asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            appended.append(1, num1);
            if (stoi(appended) <= MAX_TILE_RANGE) {
                boolNumber = true;
            } else {
                boolNumber = false;
            }
        } else if (position.size() == STRING_SIZE_3) {
            char letter = position.at(FIRST_POSITION);
            char num1 = position.at(SECOND_POSITION);
            char num2 = position.at(THIRD_POSITION);

            int asciiLetter = static_cast<unsigned char>(int(letter));
            if (asciiLetter >= ASCII_ALPHABET_BEGIN &&
                asciiLetter <= ASCII_ALPHABET_END) {
                boolLetter = true;
            } else {
                boolLetter = false;
            }
            appended.append(1, num1);
            appended.append(1, num2);
            int combinedNumber = stoi(appended);
            if (combinedNumber > MAX_BOARD_INDEX ||
                combinedNumber < MIN_TILE_RANGE) {
                boolNumber = false;
            } else {
                boolNumber = true;
            }
        }
        if (boolNumber && boolLetter) {
            condition = true;
        } else {
            cout << ERROR_MESSAGE "Not a valid position." << endl;
        }
        return condition;
    } catch (const std::invalid_argument& e) {
        cout << ERROR_MESSAGE "Not a valid position." << endl;
        return false;
    }
}

void IOHandler::placeTile(const string& tile, const string& position) {
    string appended;
    Colour colour = tile.at(0);
    Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
    int row = 0;
    int col = 0;

    if (position.size() == STRING_SIZE_2) {
        char asciiLetter = position.at(FIRST_POSITION);
        int tempRow = static_cast<unsigned char>(int(asciiLetter));
        row = tempRow - ASCII_ALPHABET_BEGIN;
        char tempCol = position.at(SECOND_POSITION);
        appended.append(1, tempCol);
        col = stoi(appended);

    } else if (position.size() == STRING_SIZE_3) {
        char asciiLetter = position.at(FIRST_POSITION);
        char num1 = position.at(SECOND_POSITION);
        char num2 = position.at(THIRD_POSITION);
        int tempRow = static_cast<unsigned char>(int(asciiLetter));
        row = tempRow - ASCII_ALPHABET_BEGIN;
        appended.append(1, num1);
        appended.append(1, num2);
        col = stoi(appended);
    }
    GameManager::placeTile(colour, shape, row, col);
}

void IOHandler::replaceTile(const string& tile) {
    Colour colour = tile.at(0);
    Shape shape = static_cast<int>(tile.at(1)) - ASCII_NUMERICAL_BEGIN;
    GameManager::replaceTile(colour, shape);
}

void IOHandler::notify(const string& message, State state) {
    if (state == PLACE_SUCCESS) {
        cout << message << endl;
        takingInput = false;
    } else if (state == PLACE_FAILURE) {
        cout << ERROR_MESSAGE << message << endl;
    } else if (state == REPLACE_SUCCESS) {
        cout << message << endl;
        takingInput = false;
    } else if (state == REPLACE_FAILURE) {
        cout << ERROR_MESSAGE << message << endl;
    } else if (state == QWIRKLE) {
        cout << message << endl;
        takingInput = false;
    } else if (state == GAME_OVER) {
        cout << endl;
        cout << *GameManager::board << endl;
        cout << "Game Over" << endl;
        cout << "Score for " << GameManager::player1->getName() << " : "
             << GameManager::player1->getScore() << endl;
        cout << "Score for " << GameManager::player2->getName() << " : "
             << GameManager::player2->getScore() << endl;
        if (GameManager::player1->getScore() >
            GameManager::player2->getScore()) {
            cout << "Player " << GameManager::player1->getName() << " won!"
                 << endl;
        } else {
            cout << "Player " << GameManager::player2->getName() << " won!"
                 << endl;
        }
        cout << "Goodbye" << endl;
        takingInput = false;
        gameRunning = false;
        GameManager::resetGame();
    }
}

void IOHandler::quit() {
    GameManager::resetGame();
    cout << "Goodbye" << endl;
    exit(EXIT_SUCCESS);
}
