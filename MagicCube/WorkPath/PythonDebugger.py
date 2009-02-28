#
#	PythonDebugger.py
#
#	This is MagicCube's python script console debugger toolkit.
#

print '-------------------------------------------------------------------------------'
print '                             K.L.\'s MagicCube'
print '                               Python Debugger'
print ''
print '    Welcome use MagicCube python script!'
print '    Have fun!'
print ''
print '    Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>'
print '    K.L.\'s MagicCube forum: http://groups.google.com/group/k_l_s-magiccube'
print '    If your scripts encountered any hard problem, just tell me :)'
print '-------------------------------------------------------------------------------'

# modify TRACER_ON 'True' to turn on tracer
TRACER_ON = False


def go():
	import string

	# import all symbols of MagicCube to local dictionary
	#	'import *' is not allowed out of module level
	import MagicCube
	for k in MagicCube.__dict__.keys():
		globals()[k] = MagicCube.__dict__.get(k)
	print '[PythonDebugger]	MagicCube symbols registered.'


	# Input the MaigcCube script file name to debug.
	filename = raw_input('[PythonDebugger]	Input script name: ')



	# Parse the file
	tryagain = True
	while tryagain:
		failed = False
		try:
			execfile(filename, globals(), globals())
		except IOError:
			try:
				execfile(filename + '.py')
			except IOError:
				try:
					execfile('scripts\\' + filename, globals(), globals())
				except IOError:
					try:
						execfile('scripts\\' + filename + '.py', globals(), globals())
					except IOError:
						failed = True
						print '[PythonDebugger]	cannot open file \'', filename, '\' or \'', filename + '.py', '\' or ', 'scripts\\' + filename, '\' or ', 'scripts\\' + filename + '.py', '\'.'

		if not failed:
			# Parse passed, start MagicCube player simulation
			print '[PythonDebugger]	Simulation begin.'

			# Input initial MagicCube state. If ignore this, CUBE0 will be used.
			valid = False
			while not valid:
				series = raw_input('[PythonDebugger]	Initial MagicCube state series code:(optional) ')
				if series == '':
					series = StateArchive(CUBE0).getSerializationCode()
				state = StateArchive(series)
				valid = state.valid()

			cube = MagicCube.MagicCube(state)

			try:
				initialize(cube)
			except:
				print '[PythonDebugger]	Failed calling \'initialize\'.'
			else:
				# Repeat calling 'step', till it return 0.
				end = False
				result = 0
				steps = 0
				runsteps = 0
				while not end:
					# Call 'step' in script file.
					try:
						result = step()
					except:
						print '[PythonDebugger]	Failed calling \'step\'.'
						end = True
					else:
						if result == None:
							print '[PythonDebugger]	Error: step function returned None, that is invalid.'
							end = True
						else:
							if (result != 0) and (type(result) != type(L)):
								print '[PythonDebugger]	Warning: step returned value(type:', type(result), ') is not of valid type.'

							if (type(result) == type(L)) and (not isValidManipulation(result)):
								print '[PythonDebugger]	Warning: step returned value(', result, ') is not valid manipulation value.'

							# Report current state. 
							steps = steps + 1
							print '[PythonDebugger]	Step:', steps
							print '	Current state:', StateArchive(cube).getSerializationCode()
							print '	Next manipulation:', result

							# Pause every step, wait for input durative executing step count.
							#	Ignore this simply, it will pause every step.
							if runsteps <= 0:
								try:
									runsteps = int(raw_input())
								except:
									runsteps = 0
							else:
								runsteps = runsteps - 1

							# Update inner cube.
							try:
								cube.doManipulation(result)
							except:
								print '[PythonDebugger]	player failed updating MagicCube.'

							end = (result == 0)

		print '[PythonDebugger]	Simulation end.'

		tryagain = (string.lower(raw_input('[PythonDebugger]	Try again?(Y/N): ')) == 'y')


import os
import sys
import trace


try:
	if TRACER_ON:
		tracer = trace.Trace(ignoredirs = [sys.prefix, os.path.dirname(__file__)])
		tracer.run('go()')
	else:
		go()
except StandardError, e:
	print '[PythonDebugger]	standard error occured:', e
except:
	print '[PythonDebugger]	debugger inner error.'
	print '	Sorry, please report this error detail to me <xxxk.l.xxx@gmail.com>.'

os.system('pause')
