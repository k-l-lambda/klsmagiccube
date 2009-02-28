#
#	Sample_SimpleFlip.py
#
#	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
#	A MagicCube python script sample.
#


# 'print' will display these information in console window.
#	This text will display once script loaded.
#	Tell who is the script's author, and you can say something about the purpose of the script.
#	Text between '[[' and ']]' is treated as a string.
print '[Sample_SimpleFlip.py]:'
print '    Author:    K.L.<xxxk.l.xxx@gmail.com>'


# the reference to the cube in player
g_Cube = None

# the current step index
g_Steps = 0

# the fixed manipulations route to play
#	Modify the list as your will, then run the player and see what happened.
#	Available manipulations symbols include: L, iL, L2, R, iR, R2, U, iU, U2, D, iD, D2, F, iF, F2, B, iB, B2.
g_Route = [L2, R2, U2, D2, F2, B2]


def initialize(cube):
	global g_Cube
	# save the cube in player
	g_Cube = cube

	# print series code of the initial MagicCube state.
	print "[Sample_SimpleFlip.py]	initialize: ", StateArchive(g_Cube).getSerializationCode()


def step():
	global g_Steps, g_Cube
	g_Steps = g_Steps + 1

	print "[Sample_SimpleFlip.py]	step:", g_Steps

	if g_Steps > len(g_Route):
		print "[Sample_SimpleFlip.py]	the end."

		# return 0 to end
		return 0

	# return the current manipulation
	return g_Route[g_Steps - 1]
