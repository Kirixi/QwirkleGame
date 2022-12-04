
#include "Player.h"

Player::Player(const string& name, shared_ptr<PlayerHand> hand)
    : name(name), score(0), hand(hand) {}

Player::~Player() { hand.reset(); }

string Player::getName() { return name; }

void Player::setName(const string& name) { this->name = name; }

size_t Player::getScore() { return score; }

void Player::setScore(size_t score) { this->score = score; }

shared_ptr<PlayerHand> Player::getHand() { return hand; }

bool operator==(const Player& player1, const Player& player2) {
    return player1.name == player2.name;
}
