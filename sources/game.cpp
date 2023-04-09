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
        player1_.incCardsTaken();
        player1_.incCardsTaken();
    } else {
        turn_log_ += player2_.getName() + " wins the round.";
        player2_.incCardsTaken();
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

    if (deck_.size() == 2) {
        // player1 and player2 had no more cards.
        // if e.g. 5, 5 -> they take back the 5, 5 and check who has more cards won (cards taken)
        // if they have the same amount of cards taken, its a TIE
        if (player1_.stacksize() == 0 && player2_.stacksize() == 0) {
            if (player1_.cardesTaken() == player2_.cardesTaken()) {
                player1_.incCardsTaken();
                player2_.incCardsTaken();
                endGame(Winner::TIE);
            } else if (player1_.cardesTaken() > player2_.cardesTaken()) {
                player1_.incCardsTaken();
                player1_.incCardsTaken();
                endGame(Winner::PLAYER_1);
            } else {
                player2_.incCardsTaken();
                player2_.incCardsTaken();
                endGame(Winner::PLAYER_2);
            }
        } else {
            // shuffle and split the cards played in this turn between the players. always even number
            auto rd = std::random_device{};
            auto rng = std::default_random_engine{rd()};
            std::shuffle(deck_.begin(), deck_.end(), rng);

            // split the deck between the players
            for (unsigned int i = 0; i < deck_.size() / 2; ++i) {
                player1_.addCard(deck_[i]);
            }
            for (unsigned int i = deck_.size() / 2; i < deck_.size(); ++i) {
                player2_.addCard(deck_[i]);
            }
        }
    } else if (deck_.size() < 4) { 
        // if one of the players doesn't have enough cards for war, the other player wins
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

    if (deck_.size() < 6) { 
        // if one of the players doesn't have enough cards, the other player wins
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
            player1_.incCardsTaken();
        }
    } else {
        turn_log_ += player2_.getName() + " wins the round.";
        for (const auto& card : deck_) {
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
    while (game_status_ != GameStatus::FINISHED) {
        playTurn();
    }
}

void ariel::Game::printWiner() {
    if (game_status_ != GameStatus::FINISHED) {
        std::cout << "the game didn't finish yet!" << std::endl;
        return;
    }

    if (name_of_winner_.empty()) {
        std::cout << "the game ended in a tie!" << std::endl;
        return;
    }

    std::cout << "the winner is " + name_of_winner_ << std::endl;
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
    if (winner == Winner::PLAYER_1) {
        name_of_winner_ = player1_.getName();
    } else if (winner == Winner::PLAYER_2) {
        name_of_winner_ = player2_.getName();
    } else {
        name_of_winner_ = nullptr;
    }

    game_status_ = GameStatus::FINISHED;
}
