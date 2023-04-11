#include "player.hpp"

#include <stdexcept>

using namespace ariel;

Player::Player(std::string name) : player_name_(name), player_in_game_(false), cards_taken_(0), rounds_won_(0), num_of_ties_(0) {
    reset();
}

void Player::addCard(const Card& card) {
    stack_.push(card);
}

void Player::incCardsTaken() {
    cards_taken_++;
}

void ariel::Player::reset() {
    cards_taken_ = 0;
    stack_ = std::stack<Card>();
    player_in_game_ = false;
    rounds_won_ = 0;
    num_of_ties_ = 0;
}

Card Player::drawCard() {
    if (stack_.empty()) {
        throw std::out_of_range("player " + player_name_ + " stack's is empty");
    } else {
        Card top_card = stack_.top();
        stack_.pop();
        return top_card;
    }
}

int Player::stacksize() const {
    return static_cast<int>(stack_.size());
}

int Player::cardesTaken() {
    return cards_taken_;
}

void Player::startGame() {
    reset();
    player_in_game_ = true;
}

// TOOD: maybe delete this reset is enough
void Player::finishGame() {
    player_in_game_ = false;
}

bool Player::isInGame() {
    return player_in_game_;
}

std::string Player::getName() {
    return player_name_;
}

void Player::wins(int cards_taken) {
    cards_taken_ += cards_taken;
    rounds_won_++;
}

void Player::tie() {
    // cards_taken_++;
    num_of_ties_++;
}

int Player::getNumOfTies() {
    return num_of_ties_;
}

int Player::getNumOfRoundsWon() {
    return rounds_won_;
}