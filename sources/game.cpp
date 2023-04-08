#include "game.hpp"

#include <algorithm>
#include <iostream>
#include <random>

ariel::Game::Game(Player& p1, Player& p2) : player1_(p1), player2_(p2) {
    if (p1 != p2 && !p1.isInGame() && !p2.isInGame()) {
        game_status_ = GameStatus::STARTED;
    }
    p1.startGame();
    p2.startGame();

    deck_.clear();
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

    deck_.clear();
}

void ariel::Game::playTurn() {
    Card card1 = player1_.drawCard();
    Card card2 = player2_.drawCard();
    turn_log_ = "";
    turn_log_ += player1_.getName() + " played " + card1.toString() + ", ";
    turn_log_ += player2_.getName() + " played " + card2.toString() + ". ";

    if (card1 == card2) {
        deck_.push_back(card1);
        deck_.push_back(card2);
        war(card1, card2);
        deck_.clear();
    } else if (card1 > card2) {
        turn_log_ += player1_.getName() + " wins the round.";
        player1_.addCard(card1);
        player1_.incCardsTaken();  // TODO: should i remove this?
        player1_.addCard(card2);
        player1_.incCardsTaken();
    } else {
        turn_log_ += player2_.getName() + " wins the round.";
        player2_.addCard(card1);
        player1_.incCardsTaken();  // TODO: should i remove this?
        player2_.addCard(card2);
        player2_.incCardsTaken();
    }

    // check if game is finished
    if (player1_.stacksize() == 0 && game_status_ != GameStatus::FINISHED) {
        endGame(Winner::PLAYER_2);
    } else if (player2_.stacksize() == 0 && game_status_ != GameStatus::FINISHED) {
        endGame(Winner::PLAYER_1);
    }
    log_.push_back(turn_log_);
}

// TODO: how to deal with the case where both players has 1 card left with the same rank?
void ariel::Game::war(Card& card1, Card& card2) {
    if (game_status_ == GameStatus::FINISHED) {
        return;
    }

    turn_log_ += " WAR! ";

    if (player1_.stacksize() > 0) {
        card1 = player1_.drawCard();
        deck_.push_back(card1);
    }
    if (player2_.stacksize() > 0) {
        card2 = player2_.drawCard();
        deck_.push_back(card2);
    }

    // player1 and player2 had no more cards.
    if (deck_.size() == 2) {
        // TODO: implement this.

    } else if (deck_.size() < 4) {  // if one of the players doesn't have enough cards for war, the other player wins
        if (player1_.stacksize() == 0) {
            turn_log_ += " player " + player2_.getName() + " wins.";
            endGame(Winner::PLAYER_2);
        } else {
            turn_log_ += " player " + player1_.getName() + " wins.";
            endGame(Winner::PLAYER_1);
        }
        return;
    }

    if (player1_.stacksize() > 0) {
        card1 = player1_.drawCard();
        turn_log_ += player1_.getName() + " played " + card1.toString() + ", ";
        deck_.push_back(card1);
    }
    if (player2_.stacksize() > 0) {
        card2 = player2_.drawCard();
        turn_log_ += player2_.getName() + " played " + card2.toString() + ", ";
        deck_.push_back(card2);
    }

    if (deck_.size() < 6) {  // if one of the players doesn't have enough cards, the other player wins
        if (player1_.stacksize() == 0) {
            turn_log_ += " player " + player2_.getName() + " wins.";
            endGame(Winner::PLAYER_2);
        } else {
            turn_log_ += " player " + player1_.getName() + " wins.";
            endGame(Winner::PLAYER_1);
        }
        return;
    }

    if (card1 == card2) {
        war(card1, card2);
    } else if (card1 > card2) {
        turn_log_ += player1_.getName() + " wins the round.";
        for (const auto& card : deck_) {
            player1_.addCard(card);
            player1_.incCardsTaken();
        }
    } else {
        turn_log_ += player2_.getName() + " wins the round.";
        for (const auto& card : deck_) {
            player2_.addCard(card);
            player2_.incCardsTaken();
        }
    }
}

void ariel::Game::printLastTurn() {
    if (log_.size() != 0) {
        std::cout << log_.back() << std::endl;
    }
}

void ariel::Game::playAll() {
}

void ariel::Game::printWiner() {
}

void ariel::Game::printLog() {
    int i = 1;
    for (const auto& turn : log_) {
        std::cout << std::to_string(i) << " " << turn << std::endl;
        i++;
    }
}

void ariel::Game::printStats() {
}

void ariel::Game::endGame(Winner winner) {
}
