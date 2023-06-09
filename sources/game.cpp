#include "game.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <random>

using namespace std;

ariel::Game::Game(Player& player1, Player& player2) : player1_(player1), player2_(player2), num_of_rounds_(0) {
    if (player1 != player2 && !player1.isInGame() && !player2.isInGame()) {
        game_status_ = GameStatus::STARTED;
        player1_.startGame();
        player2_.startGame();
    }

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
    num_of_rounds_++;
    if (game_status_ != GameStatus::STARTED) {
        throw std::runtime_error("game not started or already finished");
    }

    Card card1 = player1_.drawCard();
    Card card2 = player2_.drawCard();

    turn_log_ = "";
    turn_log_ += player1_.getName() + " played " + card1.toString() + ", ";
    turn_log_ += player2_.getName() + " played " + card2.toString() + ". ";

    if (card1 == card2) {
        war(card1, card2);
        deck_.clear();
    } else if (card1 > card2) {
        turn_log_ += player1_.getName() + " wins the round.";
        player1_.wins(2);
    } else {
        turn_log_ += player2_.getName() + " wins the round.";
        player2_.wins(2);
    }

    // check if game is finished
    if (player1_.stacksize() == 0 && game_status_ != GameStatus::FINISHED) {
        if (player2_.stacksize() != 0) {
            throw std::runtime_error("game didn't run as expected");
        }

        if (player1_.cardesTaken() == player2_.cardesTaken()) {
            endGame(Winner::TIE);
        } else if (player1_.cardesTaken() > player2_.cardesTaken()) {
            endGame(Winner::PLAYER_1);
        } else {
            endGame(Winner::PLAYER_2);
        }
    }
    log_.push_back(turn_log_);
}

void ariel::Game::war(Card& card1, Card& card2) {
    if (game_status_ != GameStatus::STARTED) {
        return;
    }
    player1_.tie();
    player2_.tie();
    turn_log_ += " WAR! ";
    // if it's not the first war, cards are already added to the deck.
    if (deck_.size() == 0) {
        deck_.push_back(card1);
        deck_.push_back(card2);
    }

    if (player1_.stacksize() == 0 && deck_.size() == 2) {
        // player1 and player2 had no more cards during a war.
        // if e.g. 5, 5 -> they take back the 5, 5 and check who has more cards won (cards taken)
        // if they have the same amount of cards taken, its a TIE
        if (player1_.cardesTaken() == player2_.cardesTaken()) {
            player1_.incCardsTaken();
            player2_.incCardsTaken();
            // player1_.tie();
            // player2_.tie();
            endGame(Winner::TIE);
        } else if (player1_.cardesTaken() > player2_.cardesTaken()) {
            player1_.wins(2);
            endGame(Winner::PLAYER_1);
        } else {
            player2_.wins(2);
            endGame(Winner::PLAYER_2);
        }
        return;
    } else if (player1_.stacksize() == 0 && deck_.size() > 2) {
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

        deck_.clear();
        war(card1, card2);
        return;
    } else if (player1_.stacksize() == 0) {
        std::cout << "deck size:" << deck_.size() << " turn_log_" << std::endl;
        std::cout << turn_log_ << std::endl;
        cout << "p1 cards taken: " << player1_.cardesTaken() << endl;  // prints the amount of cards this player has won.
        cout << "p2 cards taken: " << player2_.cardesTaken() << endl;
        cout << "p1 cards stacksize: " << player1_.stacksize() << endl;  // prints the amount of cards left. should be 21 but can be less if a draw was played
        cout << "p1 cards stacksize: " << player2_.stacksize() << endl;
    }

    // play cards flipped
    card1 = player1_.drawCard();
    deck_.push_back(card1);

    card2 = player2_.drawCard();
    deck_.push_back(card2);

    if (player1_.stacksize() > 0) {
        card1 = player1_.drawCard();
        turn_log_ += player1_.getName() + " played " + card1.toString() + ", ";
        deck_.push_back(card1);

        card2 = player2_.drawCard();
        turn_log_ += player2_.getName() + " played " + card2.toString() + ", ";
        deck_.push_back(card2);
    } else {
        // player1 and player2 have no more cards for the war.
        if (deck_.size() == 4 && allCardsSameRank(deck_)) {
            // if there are 4 cards left with same rank (e.g. 4 Twos) we dont want to enter an infinite game.
            std::cout << "cards left are same rank, ending game" << std::endl;
            player1_.incCardsTaken();
            player1_.incCardsTaken();
            player2_.incCardsTaken();
            player2_.incCardsTaken();
            player1_.tie();
            player2_.tie();
            if (player1_.cardesTaken() == player2_.cardesTaken()) {
                endGame(Winner::TIE);
            } else if (player1_.cardesTaken() > player2_.cardesTaken()) {
                endGame(Winner::PLAYER_1);
            } else {
                endGame(Winner::PLAYER_2);
            }
        } else {
            // shuffle and split thrown cards.
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

            turn_log_ += " no more cards, reshuffling, ";
            deck_.clear();
            playTurn();  // should play as they were in a normal turn because they reshuffled (they're still in a war).
        }
        return;
    }

    if (card1 == card2) {
        war(card1, card2);
    } else if (card1 > card2) {
        turn_log_ += player1_.getName() + " wins the round. won " + std::to_string(deck_.size()) + " cards!";
        player1_.wins(deck_.size());
    } else {
        turn_log_ += player2_.getName() + " wins the round. won " + std::to_string(deck_.size()) + " cards!";
        player2_.wins(deck_.size());
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
    if (game_status_ == GameStatus::FINISHED) {
        std::cout << "Player " << player1_.getName() << " stats:\n";
        std::cout << "Cards taken: " << player1_.cardesTaken() << std::endl;
        std::cout << "Win rate: " << std::fixed << std::setprecision(2) << (double)player1_.getNumOfRoundsWon() / num_of_rounds_ << "%" << std::endl;

        std::cout << "\nPlayer " << player2_.getName() << " stats:\n";
        std::cout << "Cards taken: " << player2_.cardesTaken() << std::endl;
        std::cout << "Win rate: " << std::fixed << std::setprecision(2) << (double)player2_.getNumOfRoundsWon() / num_of_rounds_ << "%" << std::endl;

        std::cout << "\nNumber of ties: " << player1_.getNumOfTies() << std::endl;
        std::cout << "number of rounds: " << num_of_rounds_ << std::endl;
    } else {
        std::cout << "game not finished, can't print stats." << std::endl;
    }
}

bool ariel::Game::allCardsSameRank(const std::vector<Card>& deck) {
    Card first_card = deck_[0];

    for (std::size_t i = 1; i < deck_.size(); ++i) {
        if (deck_[i] != first_card) {
            // if the rank of any card does not match the rank of the first card, then return false
            return false;
        }
    }

    return true;
}

void ariel::Game::endGame(Winner winner) {
    if (winner == Winner::PLAYER_1) {
        name_of_winner_ = player1_.getName();
    } else if (winner == Winner::PLAYER_2) {
        name_of_winner_ = player2_.getName();
    }  // else it's a tie, name_of_winner_ should stay empty.

    game_status_ = GameStatus::FINISHED;
}
