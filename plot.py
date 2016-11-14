#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
plt.style.use('ggplot')
from shuffle import sample_riffle

"""
-------------------------------------------------------------------
params
------
@ num_cards: number of cards
@ num_shuffle: number of shuffles.
@ num_sample: number of samples to draw.
@ seed: unsigned integer; the random seed used in sampling, 0 means ref:system time
	other values: constant behavior in every simulation.
@ init_deck: the initial deck to be shuffled.
@ card_omega: the cardinality of sample space, namely len(init_deck)!.
@ prob_unif: theoretical probability in which every outcome arises.
-------------------------------------------------------------------
"""

num_cards = 6
num_shuffle = 6
num_sample = 1000000
seed = 0	# 0 for random behavior, other value for deterministic behavior.

init_deck = range(num_cards)
card_omega = np.math.factorial(len(init_deck))
prob_unif = 1.0 / card_omega

# -----------------------------------------------------------------

def plot_sample_prob_mass_function():
	X = range(card_omega)
	fig, axes = plt.subplots(num_shuffle, 1, figsize=(10,9), sharex=True)
	axes[0].set_title("Frequency Distribution, Sample size = {}, Deck size = {}".format(num_sample, num_cards))
	for k in range(num_shuffle):
		S = sample_riffle(init_deck, k+1, num_sample, seed)
		axes[k].set_xlim(0, card_omega)
		axes[k].set_ylim(0, max(S))
		
		#ax.plot(X, S, alpha=0.6)
		axes[k].bar(X, S, width=0.5)
	axes[num_shuffle-1].set_xlabel("$X(w)$, the lexicographic order of permutations; cardinality = {}".format(card_omega))
	axes[num_shuffle/2].set_ylabel("Number of Outcomes")
	plt.show()

def plot_sample_prob_mass_function_in_one_plot():
	X = range(card_omega)
	fig, ax = plt.subplots(figsize=(10,6))
	S0 = sample_riffle(init_deck, 1, num_sample, seed)
	ax.set_xlim(0, card_omega)
	ax.set_ylim(0, 5000)
	ax.set_title("Frequency Distribution, Sample size = {}, Deck size = {}".format(num_sample, num_cards))
	for k in range(1, 4 + num_shuffle):
		S = sample_riffle(init_deck, k, num_sample, seed)
		ax.plot(X, S, alpha=0.6)
		#ax.bar(X, S, alpha=0.6, color=plt.rcParams['axes.color_cycle'][k])
	ax.set_xlabel("$X(w)$, the lexicographic order of permutations; cardinality = {}".format(card_omega))
	ax.set_ylabel("Number of Outcomes")
	plt.show()


def plot_tv():
	x = range(1, 3 + num_shuffle)
	y = [sum([0.5 * abs((1.0 * s / num_sample) - prob_unif) for s in sample_riffle(init_deck, k, num_sample, seed)]) for k in x]
	fig, ax = plt.subplots(figsize=(6,6))
	ax.plot(x, y, '*-', linewidth=1.5)
	ax.set_xlabel("Number of Shuffles")
	ax.set_ylabel("$\Delta(t)$")
	print y
	#plt.show()


plot_tv()