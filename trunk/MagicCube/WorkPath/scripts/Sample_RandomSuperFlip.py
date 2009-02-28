#
#	Sample_RandomSuperFlip.py
#
#	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
#	A MagicCube python script sample.
#


print '[Sample_RandomSuperFlip.py]:'
print '    Author:    K.L.<xxxk.l.xxx@gmail.com>'
print '    Tenet:     Play a endless random super flip pattern sequence.'


import random


# the reference to the cube in player
g_Cube = None

# the current step index
g_Steps = 0

# Manipulations prepared for super flip.
g_Manips = [L, iR, iL, R, U, iD, iU, D, F, iB, iF, B]


def initialize(cube):
	global g_Cube
	# save the cube in player
	g_Cube = cube

	# print series code of the initial MagicCube state.
	print "[Sample_RandomSuperFlip.py]	initialize: ", StateArchive(g_Cube).getSerializationCode()

def step():
	global g_Steps, g_Cube, f
	g_Steps = g_Steps + 1

	print "[Sample_RandomSuperFlip.py]	step:", g_Steps

	if (g_Steps % 2) == 1:
		# when g_Steps is odd number, pick a random couple of manipulations in prepared list,
		#	and then return the first one in couple.
		f = random.randint(0, 5)
		m = g_Manips[f * 2]
	else:
		# when g_Steps is even, return the 2nd manipulation in couple.
		m = g_Manips[f * 2 + 1]

	return m
