#pragma once

#include <string>

namespace ariel {

enum class Suit {
    SPADES,
    HEARTS,
    DIAMONDS,
    CLUBS
};

enum class Rank {
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

class Card {
   private:
    Rank rank_;
    Suit suit_;

   public:
    Card(Rank rank, Suit suit);
    std::string toString() const;

    bool operator==(const Card& other) const {
        return rank_ == other.rank_;
    }

    bool operator>(const Card& other) const {
        if (rank_ == Rank::ACE) {
            if (other.rank_ == Rank::TWO) {
                return false;
            }

            return rank_ < other.rank_;
        }

        return rank_ > other.rank_;
    }
};
}  // namespace ariel
