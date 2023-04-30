#include <iostream>
#include <vector>
#include <memory>
#include <random>
#include <ctime>
#include <algorithm> // for "shuffle"

using namespace std;

enum Rank {
    ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING
};

enum Suit {
    HEARTS, DIAMONDS, CLUBS, SPADES
};

class Card {
public:
    Card(Rank rank, Suit suit) : rank_(rank), suit_(suit) {}

    Rank getRank() const { return rank_; }
    Suit getSuit() const { return suit_; }

private:
    Rank rank_;
    Suit suit_;
};

class Deck {
public:
    Deck() {
        for (int suit = HEARTS; suit <= SPADES; ++suit) {
            for (int rank = ACE; rank <= KING; ++rank) {
                cards_.push_back(make_unique<Card>(static_cast<Rank>(rank), static_cast<Suit>(suit)));
            }
        }
    }

    void shuffledeck() {
        random_device rd;
        mt19937 g(rd());
        shuffle(cards_.begin(), cards_.end(), g);
    }

    unique_ptr<Card> drawCard() {
        if (cards_.empty()) {
            return nullptr;
        }
        unique_ptr<Card> card = std::move(cards_.back());
        cards_.pop_back();
        return card;
    }

private:
    vector<unique_ptr<Card> > cards_;
};

class Player {
public:
    void addCard(unique_ptr<Card> card) {
        hand_.push_back(std::move(card));
    }

    int getHandValue() const {
        int value = 0;
        int aces = 0;
        for (const auto& card : hand_) {
            int cardValue = card->getRank();
            if (cardValue >= 10) {
                cardValue = 10;
            } else if (cardValue == ACE) {
                aces++;
                cardValue = 11;
            }
            value += cardValue;
        }

        while (value > 21 && aces > 0) {
            value -= 10;
            aces--;
        }
        return value;
    }

private:
    vector<unique_ptr<Card> > hand_;
};

int main() {
    Deck deck;
    deck.shuffledeck();

    Player player;
    Player dealer;

    player.addCard(deck.drawCard());
    player.addCard(deck.drawCard());

    dealer.addCard(deck.drawCard());
    dealer.addCard(deck.drawCard());

    cout << "Player hand value: " << player.getHandValue() << endl;
    cout << "Dealer hand value: " << dealer.getHandValue() << endl;

    return 0;
}
