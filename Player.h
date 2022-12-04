
#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerHand.h"

#include <string>

using std::string;

class Player {
public:
    /**
     * Construct a player with name and initial hand.
     *
     * @param name - the name
     * @param hand - the PlayerHand*/
    Player(const string& name, shared_ptr<PlayerHand> hand);

    /**
     * Destructor to free up the player's hand.
     */
    ~Player();

    /**
     * Get the player's name.
     *
     * @return the name
     */
    string getName();

    /**
     * Set the name of the player.
     *
     * @param name - the name
     */
    void setName(const string& name);

    /**
     * Get the current score for the player.
     *
     * @return the integer representing score
     */
    size_t getScore();

    /**
     * Set the player's score.
     *
     * @param score - the integer representing score
     */
    void setScore(size_t score);

    /**
     * Get the player's hand.
     *
     * @return a pointer to the PlayerHand
     */
    shared_ptr<PlayerHand> getHand();

    /**
     * Overloaded equality operator for comparing equality between two players.
     *
     * @param player1,player2 - the players to compare
     * @return true if the player's have the same name
     */
    friend bool operator==(const Player& player1, const Player& player2);

private:
    string name;
    size_t score;
    shared_ptr<PlayerHand> hand;
};

#endif // !PLAYER_H
