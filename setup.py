from distutils.core import setup, Extension

module = Extension('shuffle', sources=['shuffle.c'])
setup(
	name = 'shuffle',
	version = '0.1',
	description = 'Course project of Stochastic Process (2016 Spring) at SJTU.',
	author = 'Zed Yang & Hugh Wang',
	author_email = 'zed.yang@outlook.com',
	url = 'https://github.com/zedyang/Card-Shuffling-with-MC',
	ext_modules=[module]
)