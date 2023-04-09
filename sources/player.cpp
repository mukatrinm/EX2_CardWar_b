#include "player.hpp"

using namespace ariel;

size_t Player::next_id_ = 0;

Player::Player(std::string name) : player_id_(++next_id_), player_name_(name), player_in_game_(false), cards_taken_(0) {
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
}

Card Player::drawCard() {
    if (stack_.empty()) {
        throw std::out_of_range("player " + std::to_string(player_id_) + "stack is empty");
    }

    Card top_card = stack_.top();
    stack_.pop();
    return top_card;
}

int Player::stacksize() const {
    return static_cast<int>(stack_.size());
}

int Player::cardesTaken() {
    return cards_taken_;
}

void Player::startGame() {
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