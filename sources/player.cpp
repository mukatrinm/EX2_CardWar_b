#include "player.hpp"

ariel::Player::Player(std::string name) : player_name_(name) {
}

void ariel::Player::addCard(const Card& card) {
    stack_.push(card);
}

int ariel::Player::stacksize() const {
    return stack_.size();
}

int ariel::Player::cardesTaken() {
    return 0;
}

void ariel::Player::startGame() {
    player_in_game_ = true;
}

void ariel::Player::finishGame() {
    player_in_game_ = false;
}

bool ariel::Player::isInGame() {
    return player_in_game_;
}
