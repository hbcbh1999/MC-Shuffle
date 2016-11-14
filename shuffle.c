/*
  @File			shuffle.c
  @Author		Zed
  @Day 			8 May 2016
*/
#include "shuffle.h"

void disp_array(int *a, int size, bool with_order)
{
	for (int i=0; i<size; ++i) 
	{
		printf(" %d |", a[i]);
	}
	if(with_order)
	{ // also print the lexicographic order.
		printf("   [%llu]", lexicographic_order_of_permutation(a, size));
	}
	printf("\n");
}

void swap(int *x, int *y) 
{ // Swap values at two pointers.
  int temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

void reverse(int *a, int begin, int end)
{ // flip an array from a[begin] to a[end]
	while(begin < end) 
	{
		swap(&a[begin], &a[end]);
		begin++;
		end--;
	}
}

long factorial(int n)
{
  if (n == 0)
    return 1;
  else
    return(n * factorial(n-1));
}

unsigned long long lexicographic_order_of_permutation(int *array, int size) 
{ /*
	 * assign lexicogrphic index to a permutation
	 * the lowest order is defined as 1; 0 preserved for uninitialized condition.
	 *
	 */
	unsigned long long order = 1;	// 
	if(size == 1) return order;

	for(int j = 0; j < size; j++)
	{
		int count = 0;
		for(int i = j + 1; i < size; i++)
		{
			if(array[i] < array[j]) count++;
		}
		if(count > 0)
			order += count * factorial(size - j - 1);
	}
	return order;
}

void disp_deck(deck *d) 
{ // Display a deck.
	disp_array(d->array_, d->size_, false);
}

void deallocate_deck(deck *d) 
{ // deallocate the memory of a deck (pointer).
	free(d->array_);
	free(d);
}

bool is_identical_deck(deck *d1, deck *d2)
{ // Whether two decks are the same.

	if(d1->size_ != d2->size_)
	{
		return false;
	}
	for(int ix = 0; ix < d1->size_; ix++) 
	{
		if(d1->array_[ix] != d2->array_[ix])
		{
			return false;
		}
	}
	return true;
}

deck *make_sequential_deck(int size) 
{ /* 
	 * A constructor.
	 * Make a deck in sequential order, begin with integer 1.
	 * size: specified deck size.
	 */
	int *array = malloc(size * sizeof(int));
	for(int ix = 0; ix < size; ix++) 
	{
		array[ix] = ix + 1;
	}
	// pack the deck struct.
	deck *new_deck = malloc(sizeof(deck));
	new_deck->array_ = array;
	new_deck->size_ = size;
	new_deck->type_ = 0;

	return new_deck;
}

deck *copy_deck_from(deck* from)
{ /*
	 * copy constructor.
	 */
	int *array = malloc((from->size_) * sizeof(int));
	for(int ix = 0; ix < from->size_; ix++) 
	{
		array[ix] = from->array_[ix];
	}
	deck *new_deck = malloc(sizeof(deck));
	new_deck->array_ = array;
	new_deck->size_ = from->size_;
	new_deck->type_ = from->type_;

	return new_deck;
}

void permute_deck_in_lexicographic_order(deck* d) 
{ /*
	 * Permute a deck with indices in the lexicographic order.
	 *
	 */
	int n = d->size_;

	// create a sorted deck.
	deck *sorted_deck = make_sequential_deck(n);
	int *array = sorted_deck->array_;

	bool is_finished = false;
	while(!is_finished)
	{
		disp_array(array, n, true);	// display this permutation with order.
		/*
		 * find the rightmost element who is smaller than next one.
		 */
		int i;
		for(i = n-2; i >= 0; --i)
		{
			if(array[i] < array[i+1])
				break;
		}
		/* If there is no such one, we are done,
		 * the array is just in decreasing order.
		 */
		if(i==-1)
			is_finished = true;
		else
		{
			int ceil_index = i+1;
			// find the ceil of a[i] in the right of it.
			// i.e. ceil_index is the smallest element greater than it.
			for(int k = ceil_index+1; k < n; k++)
			{
				if(array[k] > array[i] && array[k] < array[ceil_index])
					ceil_index = k;
			}

			// swap these two elements.
			swap(&array[i], &array[ceil_index]);
			// reverse the array on the right of i.
			reverse(array, i+1, n-1);
		}
	}
	// cleanup.
	deallocate_deck(sorted_deck);
}

// ---------------------------------------------------

void deallocate_prob_mass_function(prob_mass_function *pmf) 
{ // deallocate the memory of a pmf (pointer).
	free(pmf->p_);
	free(pmf);
}

prob_mass_function *make_uniform_probability_mass(unsigned long num_outcomes)
{ /*
	 * construct a pmf for unifrom distribution.
	 */
	double u = 1.0 / num_outcomes;
	double *p = malloc(num_outcomes * sizeof(double));
	for(int ix = 0; ix < num_outcomes; ix++) 
	{
		p[ix] = u;
	}
	// pack the pmf struct.
	prob_mass_function *new_pmf = malloc(sizeof(prob_mass_function));
	new_pmf->p_ = p;
	new_pmf->sample_space_cardinality_ = num_outcomes;
	return new_pmf;
}

prob_mass_function *make_empirical_probability_mass(int *counts, 
	unsigned long num_samples, unsigned long num_outcomes)
{ /*
	 * construct a pmf out of an empirical distribution.
	 */
	double *p = malloc(num_outcomes * sizeof(double));
	for(int ix = 0; ix < num_outcomes; ix++) 
	{
		p[ix] = (double)(1.0 * counts[ix] / num_samples);
		// note here divided by num_samples.
	}
	// pack the pmf struct.
	prob_mass_function *new_pmf = malloc(sizeof(prob_mass_function));
	new_pmf->p_ = p;
	new_pmf->sample_space_cardinality_ = num_outcomes;
	return new_pmf;
}

prob_mass_function *copy_probability_mass_from(prob_mass_function *pmf_from)
{ /*
   * copy constructor.
   */
	double *p = malloc(pmf_from->sample_space_cardinality_ * sizeof(double));
	for(int ix = 0; ix < pmf_from->sample_space_cardinality_; ix++) 
	{
		p[ix] = pmf_from->p_[ix];
	}
	prob_mass_function *new_pmf = malloc(sizeof(prob_mass_function));
	new_pmf->p_ = p;
	new_pmf->sample_space_cardinality_ = pmf_from->sample_space_cardinality_;
	return new_pmf;
}

double total_variation_norm(prob_mass_function *pmf1, prob_mass_function *pmf2)
{ /* 
	 * Implementation of discrete https://en.wikipedia.org/wiki/Total_variation
	 */
	assert(pmf1->sample_space_cardinality_ == pmf2->sample_space_cardinality_);
	double tv_norm = 0;
	for(int i = 0; i < pmf1->sample_space_cardinality_; i++) {
		tv_norm += fabs(pmf1->p_[i] - pmf2->p_[i]);
	}
	return 0.5 * tv_norm;
}

// ---------------------------------------------------
deck* riffle_shuffle(deck *initial_deck, int num_shuffles, bool dispaly_shuffle_process)
{ 
	/* 
	 * Do the riffle shuffle.
	 * num_shuffles: number of shuffles.
	 *
	 */
	// << Some parameters in simulation >>
	// The number of times an item has been used consecutively from 
	// same half.
	// int const max_streak = 10;

	/*
	 * Find the starting point of the second half of the deck.
	 * TODO: should be a binomial dist?
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
	 * we randomly choose between d1 or d2, let (*current) equals our selection
	 * we let (*current)++
	 */
	int d, d1, d2, streak;
	// initialize the current pointer
	int *current = (rand() % 2) ? &d1 : &d2;

	if(dispaly_shuffle_process) 
	{
		printf("-------%d shuffles remaining.-------\n", num_shuffles);
	}

	for(d = 0, d1 = 0, d2 = last_half_start, streak = 0; 
		d < n && d2 < n && d1 < last_half_start; d++, *current += 1) 
	{
		// Switch between two halves. Select d1 w.p # cards in frist half / # cards.
		double proportion_left = (last_half_start - d1) / (double)(n - d);
		double test = rand() / (double)RAND_MAX; // Uniform[0, 1]
		
		current = (test < proportion_left ? &d1 : &d2);

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

	// deallocate memory taken by intermediate results.
	deallocate_deck(initial_deck);

	// do the shuffle recursively.
	num_shuffles--;
	if(num_shuffles > 0) 
	{
		shuffled_deck = riffle_shuffle(shuffled_deck, num_shuffles, dispaly_shuffle_process);
	} 
	return shuffled_deck;
}

// ---------------------------------------------------
void sample_riffle_shuffle(int num_samples, int num_shuffles, deck* initial_deck)
{
	/* Draw samples from shuffling distribution with specified
	 * # of samples, # of shuffles and initial deck.
	 * 
	 */
	srand((int)time(NULL));	// rng seeding.

	int counter = 0;
	long sample_space_cardinality = factorial(initial_deck->size_); // cardinality |Omega|, = n!

	// Initialize record book.
	int *frequency = malloc(sample_space_cardinality * sizeof(int));
	for(int i = 0; i < sample_space_cardinality; i++) {
		frequency[i] = 0;
	}

	while(counter < num_samples)
	{
		deck* shuffled_deck;
		/* make a copy of initial deck every time, since riffle_shuffle
		 * is designed to deallocate the initial deck as parameter.
		 *
		 * at the execution of riffle_shuffle, this cpy_initial_deck is deallocated automatically.
		 */
		deck* cpy_initial_deck;
		cpy_initial_deck = copy_deck_from(initial_deck);
		shuffled_deck = riffle_shuffle(cpy_initial_deck, num_shuffles ,false);

		// compute order for the output.
		shuffled_deck->type_ = lexicographic_order_of_permutation(
			shuffled_deck->array_, shuffled_deck->size_);

		/*
		 *
		 * Do some calculations inside.
		 *
		 * Example: calculate the proportion of type [1] deck.
		 *
		 */
		frequency[(shuffled_deck->type_ - 1)] += 1;

		++counter;
		// cleanup the simulation results.
		deallocate_deck(shuffled_deck);
	}
	/*
	 *
	 * Do some calculations outside.
	 *
	 * Example: calculate the total variation norm.
	 *
	 */
	// make theoratical stationary pmf and empirical pmf from simulation.
	prob_mass_function *stationary_pmf = make_uniform_probability_mass(sample_space_cardinality);
	prob_mass_function *empirical_pmf = make_empirical_probability_mass(frequency, num_samples, sample_space_cardinality);
	double tv_norm = total_variation_norm(stationary_pmf, empirical_pmf);

	// print result.
	for(int i = 0; i < sample_space_cardinality; i++) {
		printf("Prob{permutation [%d] arises} = %f\n", i+1, 1.0 * frequency[i] / num_samples);
	}
	printf("Prob{permutation [i] arises} (Expected) = %f\n", 1.0 / sample_space_cardinality);
	printf(">> Total variation norm = %f\n", tv_norm);

	// final cleanup
	free(frequency);
	deallocate_prob_mass_function(stationary_pmf);
	deallocate_prob_mass_function(empirical_pmf);
}

// ---------------------------------------------------
PyObject *do_sample_riffle_shuffle(PyObject *initial_deck_py, int num_shuffles, int num_samples, unsigned int seed)
{ /* Python interface.
	 * PyObject *initial_deck is a pointer to python list.
	 * unsigned int seed is rng seed variable.
	 */
	if(seed == 0) {
		srand((int)time(NULL));	// random seeding.
	} else {
		srand(seed);
	}

	int counter = 0;
	int n = (int)PyList_Size(initial_deck_py); // size of deck.
	long sample_space_cardinality = factorial(n); // cardinality |Omega|, = n!

	// Initialize results' placeholder.
	int *frequency = malloc(sample_space_cardinality * sizeof(int));
	for(int i = 0; i < sample_space_cardinality; i++) {
		frequency[i] = 0;
	}

	// copy python list into C deck.
	deck* initial_deck = malloc(sizeof(deck));
	int *array = malloc(n * sizeof(int));
	for(int i = 0; i < n; i++) {
		array[i] = (int)(PyList_GetItem(initial_deck_py, i));
	}
	initial_deck->array_ = array;
	initial_deck->size_ = n;
	initial_deck->type_ = 0;

	while(counter < num_samples)
	{
		deck* shuffled_deck;
		/* make a copy of initial deck every time, since riffle_shuffle
		 * is designed to deallocate the initial deck as parameter.
		 *
		 * at the execution of riffle_shuffle, this cpy_initial_deck is deallocated automatically.
		 */
		deck* cpy_initial_deck;
		cpy_initial_deck = copy_deck_from(initial_deck);
		shuffled_deck = riffle_shuffle(cpy_initial_deck, num_shuffles ,false);

		// compute order for the output.
		shuffled_deck->type_ = lexicographic_order_of_permutation(
			shuffled_deck->array_, shuffled_deck->size_);

		/*
		 *
		 * Do some calculations inside.
		 *
		 * Example: calculate the proportion of type [1] deck.
		 *
		 */
		frequency[(shuffled_deck->type_ - 1)] += 1;

		++counter;
		// cleanup the simulation results.
		deallocate_deck(shuffled_deck);
	}
	// Make python returning object.
	PyObject *frequency_py = PyList_New(sample_space_cardinality);
	for(int i = 0; i < sample_space_cardinality; i++) {
		// extract quantities from C array, make pyobject
		PyObject *temp = PyFloat_FromDouble(frequency[i]);

		PyList_SetItem(frequency_py, i, temp);
		Py_INCREF(temp);
	}

	// final cleanup
	deallocate_deck(initial_deck);
	free(frequency);

	return frequency_py;
}

static PyObject *py_sample_riffle_shuffle(PyObject *self, PyObject *args)
{
	PyObject *initial_deck_py;
	int num_shuffles;
	int num_samples;
	unsigned int seed = 0;

	// parse args to list
	if (! PyArg_ParseTuple( args, "Oii|I", &initial_deck_py, &num_shuffles, &num_samples, &seed) )
	{
		return NULL;
	}
	return do_sample_riffle_shuffle(initial_deck_py, num_shuffles, num_samples, seed);
}

static PyMethodDef Methods[] = {
	{"sample_riffle", py_sample_riffle_shuffle, METH_VARARGS, "Simulate a Riffle Shuffle on a List."},
	{NULL, NULL, 0, NULL}
};

void initshuffle(void){
	(void) Py_InitModule("shuffle", Methods);
}

/*
int main(int argc, char **argv)
{
	int n_sim = 10000000;
	int n_cards = 5;
	int num_shuffles;
	srand((int)time(NULL));
	while(1){
		deck* d1 = make_sequential_deck(n_cards);
		
		scanf("%d", &num_shuffles);

		printf("-------------------------------------------------------\n");
		printf(">> Statistics: \n");
		sample_riffle_shuffle(n_sim, num_shuffles, d1);
		printf("-------------------------------------------------------\n");

		deallocate_deck(d1);
	}
  return 0;
}
*/