#include "game.hpp"

#include <algorithm>
#include <iostream>
#include <random>

ariel::Game::Game(Player &p1, Player &p2) : player1_{p1}, player2_{p2} {
    if (p1 != p2 && !p1.isInGame() && !p2.isInGame()) {
        game_status_ = GameStatus::STARTED;
    }
    p1.startGame();
    p2.startGame();

    for (int i = 0; i < 52; ++i) {
        // each suit has 13 cards, rank starts from 1 (ACE)
        Card card(static_cast<Rank>(i % 13 + 1), static_cast<Suit>(i / 13));
        deck_.push_back(card);
    }

    // shuffle the deck
    // https://stackoverflow.com/questions/6926433/how-to-shuffle-a-stdvector
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(deck_.begin(), deck_.end(), rng);

    // split the deck between the players
    for (unsigned int i = 0; i < 26; ++i) {
        player1_.addCard(deck_[i]);
    }
    for (unsigned int i = 26; i < 52; ++i) {
        player2_.addCard(deck_[i]);
    }
}

void ariel::Game::playTurn() {
}

void ariel::Game::printLastTurn() {
}

void ariel::Game::playAll() {
}

void ariel::Game::printWiner() {
}

void ariel::Game::printLog() {
}

void ariel::Game::printStats() {
}
