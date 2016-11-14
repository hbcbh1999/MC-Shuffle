/*
  @File		shuffle.h
  @Author	Zed
  @Date    	8 May 2016
*/
#ifndef __CARD_SHUFFLE_H__
#define __CARD_SHUFFLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
// Mac: 'Python' is a framework.
#include <Python/Python.h>
// #include <Python.h>

// ---------- Marco: N-elements, Get the # of elements in an array-like object ----------
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

typedef int bool;
#define true 1
#define false 0

// ---------- Utility functions ----------
void disp_array(int *a, int size, bool with_order);
void swap(int *x, int *y);
void reverse(int *a, int begin, int end);
long factorial(int n);
unsigned long long lexicographic_order_of_permutation(int *array, int size);

// ---------- Deck class ----------
typedef struct
{
	int* array_;
	int size_;
	unsigned long long type_; 
	/* 
	 * 'Type' of the deck is its lexicographic order in full permutation.
	 *
	 * 0: not initialized.
	 *
	 * This quantity is NOT needed for intermediated decks within the shuffle steps,
	 * only required for the final output.
	 */
} deck;

void disp_deck(deck *d);
void deallocate_deck(deck *d);
bool is_identical_deck(deck *d1, deck *d2);
deck *make_sequential_deck(int size);
deck *copy_deck_from(deck* from);
void permute_deck_in_lexicographic_order(deck* d);

// ---------- Probability mass function class ----------
typedef struct
{
	double* p_;
	unsigned long sample_space_cardinality_;
} prob_mass_function;

void deallocate_prob_mass_function(prob_mass_function *pmf);
prob_mass_function *make_uniform_probability_mass(unsigned long num_outcomes);
prob_mass_function *make_empirical_probability_mass(int *counts, 
	unsigned long num_samples, unsigned long num_outcomes);
prob_mass_function *copy_probability_mass_from(prob_mass_function *pmf_from);
double total_variation_norm(prob_mass_function *pmf1, prob_mass_function *pmf2);

// ---------- Main functionals ----------
deck* riffle_shuffle(deck *initial_deck, int num_shuffles, bool dispaly_shuffle_process);
void sample_riffle_shuffle(int num_samples, int num_shuffles, deck* initial_deck);

// ---------- Main functionals Python interface ----------
PyObject *do_sample_riffle_shuffle(PyObject *initial_deck_py, int num_shuffles, int num_samples, unsigned int seed);
static PyObject *py_sample_riffle_shuffle(PyObject *self, PyObject *args);


#endif