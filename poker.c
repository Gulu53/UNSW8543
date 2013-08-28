/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: - When run with -get_one_hand as first command line argument,  *
 *                  . uses the number provided as second command line argument *
 *                    as a seed to the random number generator,                *
 *                  . randomly generates a hand of 5 cards from a deck of 52,  *
 *                  . sorts the cards by rank, and for a given rank sorts the  *
 *                    cards by suit in the order hearts, clubs, diamonds and   *
 *                    spades, and                                              *
 *                  . determines whether the hand is                           *
 *                      * a straight flush                                     *
 *                      * a four of a kind                                     *
 *                      * a full house                                         *
 *                      * a flush                                              *
 *                      * a straight                                           *
 *                      * a three of a kind                                    *
 *                      * a two pair                                           *
 *                      * a one pair                                           *
 *                      * a high card                                          *
 *              - When run with -simulate as first command line argument,      *
 *                  . uses the number provided as second command line argument *
 *                    as a seed to the random number generator,                *
 *                  . randomly generates a hand of 5 cards from a deck of 52,  *
 *                    as many times as given by the number provided as third   *
 *                    command line argument, and                               *
 *                  . outputs the frequencies of the kinds of hands that have  *
 *                    been obtained.                                           *
 *                                                                             *
 * Written by Eric Martin and NilH for COMP9021                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h> 
#include <locale.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h> 

#define HEARTS L'\u2665'
#define CLUBS L'\u2663'
#define DIAMONDS L'\u2666'
#define SPADES L'\u2660'
#define ACE "A"
#define TWO "2"
#define THREE "3"
#define FOUR "4"
#define FIVE "5"
#define SIX "6"
#define SEVEN "7"
#define EIGHT "8"
#define NINE "9"
#define TEN "10"
#define JACK "J"
#define QUEEN "Q"
#define KING "K"
#define NB_OF_RANKS 13
#define NB_OF_SUITS 4
#define DECK_SIZE (NB_OF_RANKS * NB_OF_SUITS)
#define HAND_SIZE 5
#define STRAIGHT_FLUSH "straight flush"
#define FOUR_OF_A_KIND "four of a kind"
#define FULL_HOUSE "full house"
#define FLUSH "flush"
#define STRAIGHT "straight"
#define THREE_OF_A_KIND "three of a kind"
#define TWO_PAIR "two pair"
#define ONE_PAIR "one pair"
#define HIGH_CARD "high card"

#define RANK(X) (X % NB_OF_RANKS)
#define SUIT(X) (X / NB_OF_RANKS)

const char *const RANKS[] = {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
const wchar_t const SUITS[] = {HEARTS, CLUBS, DIAMONDS, SPADES};

void get_hand(void);
void order_cards(void);
int cmp(const int, const int);
void display_cards(void);
char *kind_of_hand(void);
bool straight(void);
bool flush(void);
bool four_of_a_kind(void);
bool three_of_a_kind(void);
int nb_of_ranks(void);
void simulate(int);
void strcmp_loc (int*, char**);

/* The sequence of numbers between 0 and 51 represent the cards in the order:
 *   - ace, two, three, ..., ten, jack, queen, king of hearts,
 *   - ace, two, three, ..., ten, jack, queen, king of clubs,
 *   - ace, two, three, ..., ten, jack, queen, king of diamonds,
 *   - ace, two, three, ..., ten, jack, queen, king of spades.
 *   
 * The array "hand" stores five distinct numbers between 0 and 51,
 * hence represents five cards.
 * 
 * RANK(hand[0]), RANK(hand[1]), ..., RANK[hand[4]) represent the ranks
 * of those 5 cards, respectively, with
 *   - 0 representing ace
 *   - 1 representing two
 *   ...
 *   - 12 representating king.
 *   
 * SUIT(hand[0]), SUIT(hand[1]), ..., SUIT[hand[4]) represent the suits
 * of those 5 cards, respectively, with
 *   - 0 representing hearts
 *   - 1 representing clubs
 *   - 2 representing diamonds
 *   - 3 representing spades.
 *   (Actually, it suffices to know that each suit is represented by a unique number...) */
int hand[HAND_SIZE];

int main(int argc, char **argv) {
    if (argc < 3 || argc > 4) {
        printf("I expect two or three command line arguments, "
               "try again.\n");
        return EXIT_FAILURE;
    }
    srand(strtoul(argv[2], NULL, 10));
    if (argc == 3) {
        if (strcmp(argv[1], "-get_one_hand")) {
            printf("With two command line arguments, "
                   "I expect the first one to be \"-get_one_hand\", "
                   "try again.\n");
            return EXIT_FAILURE;
        }
        get_hand();
        order_cards();
        display_cards();
        printf("   is a %s.\n", kind_of_hand());
        return EXIT_SUCCESS;
    }
    if (strcmp(argv[1], "-simulate")) {
        printf("With three command line arguments, "
               "I expect the first one to be \"-simulate\", "
               "try again.\n");
        return EXIT_FAILURE;
    }
    simulate(strtoul(argv[3], NULL, 10));
    return EXIT_SUCCESS;
}

/* Randomly assigns five distinct values between 0 and 51
 * to the elements of the array "hand". */
void get_hand(void) {
    int deck[DECK_SIZE];
    for (int i = 0; i < DECK_SIZE; ++i)
        deck[i] = i;
    for (int i = 0; i < HAND_SIZE; ++i) {
        int j = rand() % (DECK_SIZE - i) + i;
        int temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
    for (int i = 0; i < HAND_SIZE; ++i)
        hand[i] = deck[i];
}

/* Orders the numbers stored in "hand" so that the cards they represent are ordered */
/* as stated in the program description. */
void order_cards(void) {
    for (int i = 1; i < HAND_SIZE; ++i)
        for (int j = i; j > 0 && cmp(hand[j - 1], hand[j]) > 0; --j) {
            int temp = hand[j - 1];
            hand[j - 1] = hand[j];
            hand[j] = temp;
        }
}

int cmp(const int a, const int b) {
    int rank_difference = RANK(a) - RANK(b);
    if (rank_difference)
        return rank_difference;
    return SUIT(a) - SUIT(b);
}

void display_cards(void) {
    setlocale(LC_CTYPE, "");
    for (int i = 0; i < HAND_SIZE; ++i) {
        printf("%4s", RANKS[RANK(hand[i])]);
        putwchar(SUITS[SUIT(hand[i])]);
    }
}

char *kind_of_hand(void) {
    if (straight() && flush())
        return STRAIGHT_FLUSH;
    if (four_of_a_kind())
        return FOUR_OF_A_KIND;
    int nb_of_ranks_in_hand = nb_of_ranks();
    if (nb_of_ranks_in_hand == 2)
        return FULL_HOUSE;
    if (flush())
        return FLUSH;
    if (straight())
        return STRAIGHT;
    if (three_of_a_kind())
        return THREE_OF_A_KIND;
    if (nb_of_ranks_in_hand == 3)
        return TWO_PAIR;
    if (nb_of_ranks_in_hand == 4)
        return ONE_PAIR;
    return HIGH_CARD;
}

/* Implement this function; 8 lines suffice. */
bool straight(void) {
    for (int i = 1; i < HAND_SIZE; i++ ) {
        if (RANK(hand[i]) - RANK(hand[i - 1]) != 1)
            return false;
    } 
    return true; 
}

/* Implement this function; 4 lines suffice. */
bool flush(void) {
    for (int i = 1; i < HAND_SIZE; i++) { 
        if (SUIT(hand[0]) != SUIT(hand[i]))
            return false;
    }
    return true;
}
    
/* Implement this function; 5 lines suffice. */
bool four_of_a_kind(void) {
    int off_set = (RANK(hand[0]) == RANK(hand[1])) ? 0 : 1, sum = 0, number_of_addition = 4;
    for (int i = 0; i < number_of_addition; i++ ) {
        sum += RANK(hand[i + off_set]);
    }
    return (sum == RANK(hand[0 + off_set]) * 4) ? true : false;   
}

/* Implement this function; 7 lines suffice. */
bool three_of_a_kind(void) {
    // 3n + 13(x) is the sum of the three of a kind, where x can be either 0, 1, 2 or 3
    for (int i = 0; i < 3; i++) {
        int sum = 0;
        for (int j = 0; j < 3; j++) {
            sum += RANK(hand[i + j]);
        }
        if (sum == RANK(hand[i]) * 3)
            return true;
    }
    return false;
}

/* Implement this function; 5 lines suffice. */
int nb_of_ranks(void) {
    int ranks_nb = 1;
    for (int i = 1; i < HAND_SIZE; i++)
        ranks_nb += (RANK(hand[i]) != RANK(hand[i - 1])) ? 1 : 0;
    return ranks_nb;
}
    
/* Implement this function; only requires to adapt kind_of_hand()
 * and add printf() statements; use "%7.4f%%" to print out the percentages.
 * You will probably use an expression involving a multiplication by 100.0
 * (not 100, so that the result be a floating point number). */
void simulate(int N) {
    int seq_count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    char *keys[] = {HIGH_CARD, ONE_PAIR, TWO_PAIR, THREE_OF_A_KIND, STRAIGHT, FLUSH, FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH};
    char *output[] = {"High card:", "One pair:", "Two pair:", "Three of a kind:", "Straight:", "Flush:", "Full house:", "Four of a kind:","Straight flush:"};
    for (int i = 1; i <= N; i++) { 
        get_hand();
        order_cards();
        strcmp_loc(seq_count, keys);
    }
    printf("Results of the simulation:\n");
    for (int i = 8; i >= 0; i--) {
        printf("  %-16s %7.4f%%\n", output[i], (seq_count[i] / (float)N) * 100.0);
    }
}

void strcmp_loc (int *seq_count, char **keys) {
    for (int i = 0; i < 9; i++) {
        char *ans = kind_of_hand();
        if (!(strcmp(ans, *(keys + i)))) {
            *(seq_count + i) = *(seq_count + i) + 1;
            break;
        }
    } 
}

