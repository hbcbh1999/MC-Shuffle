/*
  @file			shuffle.c
  @author		Zed
  @date     8 May 2016
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Marco: N-elements, Get the # of elements in an array-like object.
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

typedef int bool;
#define true 1
#define false 0

// Deck structure.
typedef struct
{
	int* array_;
	int size_;
} deck;

void swap(int *x, int *y) 
{ // Swap values at two pointers.
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

void disp_deck(deck *d) 
{ // Display a deck.

	for (int i=0; i<d->size_; ++i) {
		printf(" %d |", d->array_[i]);
	}
	printf("\n");
}

deck *make_sequential_deck(int size) 
{ /* 
	 * Make a deck in sequential order, begin with integer 1.
	 * size: specified deck size.
	 */
	int *array = malloc(size * sizeof(int));
	for(int ix = 0; ix < size; ix++) {
		array[ix] = ix + 1;
	}
	// pack the deck struct.
	deck *new_deck = malloc(sizeof(deck));
	new_deck->array_ = array;
	new_deck->size_ = size;
	return new_deck;
}

// ---------------------------------------------------
deck* riffle_shuffle(deck *initial_deck, int num_shuffles, bool dispaly_shuffle_process)
{ /* 
	 * Do the riffle shuffle.
	 * num_shuffles: number of shuffles.
	 *
	 */
	// << Some parameters in simulation >>
	// The number of times an item has been used consecutively from 
	// same half.
	int const max_streak = 10;
	srand((int)time(NULL));


	/*
	 * Find the starting point of the second half of the deck.
	 * TODO: should be a binomial dist.
	 */
	deck *shuffled_deck = malloc(sizeof(deck));
	int *shuffled_array = malloc(initial_deck->size_ * sizeof(int));

	int n = initial_deck->size_; // total # of cards
	int last_half_start = (int)(n / 2);

	/* Now proceed by interleave the two part.
	 * >> Specifier:
	 * d:  index of cards in the deck to proceed.
	 * d1: index of cards in the first half to proceed.
	 * d2: index of cards in the another half to proceed.
	 * streak: counter of how many times a card from same half has been
	 * consecutively used, for example: d1 | d2 | d2 | d1 | d1 | d1
	 * at this schedule， streak is 3, since 3 cards from d1 are put
	 * together.
	 * (*current): a pointer pointing to either d1 or d2. 
	 * Records the position of card to add to shuffled_deck from currect selected half.
	 *
	 * >> Main Loop:
	 * We add one card into the shuffled deck each time.
	 * we let d++
	 * we randomly choose between d1 or d2, let (*from_which) equals our selection
	 * we let (*from_which)++
	 */
	int d, d1, d2, streak;
	// initialize the current pointer
	int *current = (rand() % 2) ? &d1 : &d2;

	if(dispaly_shuffle_process) 
	{
		printf("-------%d shuffles remaining.-------\n", num_shuffles);
	}

	for(d = 0, d1 = 0, d2 = last_half_start, streak = 0; 
		d < n && d2 < n && d1 < last_half_start; 
		d++, *current += 1) 
	{
		// Switch between two halves. Select d1 w.p # cards in frist half / # cards.
		double proportion_left = (last_half_start - d1) / (double)(n - d);
		double test = rand() / (double)RAND_MAX; // Uniform[0, 1]
		
		if(test < proportion_left) {
			current = &d1;
		} else {
			current = &d2;
		}

		// Display the process
		if(dispaly_shuffle_process) 
		{
			printf("%f, %f, %d, %d, %d\n", test, proportion_left, 
				initial_deck->array_[d1], initial_deck->array_[d2], initial_deck->array_[(*current)]);
		}		

		// add item to new shuffed_list
		int temp = initial_deck->array_[(*current)];
		shuffled_array[d] = temp;
	}
	// Switch to the remaining hand i.e. the one that didn't cause for loop to exit.
	current = (current == &d1 ? &d2 : &d1);
	// Append the rest of cards.
	while(d < n) 
	{
		int temp = initial_deck->array_[(*current)];
		shuffled_array[d] = temp;
		d++;
		*current += 1;
	}

	shuffled_deck->array_ = shuffled_array;
	shuffled_deck->size_ = initial_deck->size_;

	// Do the shuffle recursively.
	num_shuffles--;
	if(num_shuffles > 0) 
	{
		shuffled_deck = riffle_shuffle(shuffled_deck, num_shuffles, dispaly_shuffle_process);
	}

	return shuffled_deck;
}



int main(int argc, char **argv)
{

	deck* new_deck;
	new_deck = make_sequential_deck(20);

	disp_deck(new_deck);
	deck* d2;
	d2 = riffle_shuffle(new_deck,7,true);
	disp_deck(d2);
	
	int x = rand();
  return 0;
}