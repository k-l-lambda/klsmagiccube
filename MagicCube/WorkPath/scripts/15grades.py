#
#	15grades.py
#
#	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
#

print '[15grades.py]:'
print '    Author:    K.L.<xxxk.l.xxx@gmail.com>'
print '    Tenet:     Restore MagicCube to zero state.'
print '    Principle: Define 15 grades by how ordered the state is, and transit'
print '               between grades by method of exhaustion.'
print '    Issues:    Distance of some abutting grades is too far, and method of'
print '               exhaustion is too unefficient, the required search deep too'
print '               large.'
print '               The result of python version seems wrong in current, while'
print '               the result of lua version(15grades.lua) seems right. I guess'
print '               there is some logic error in code. If you find it, tell me.'

SEARCH_DEEP = 4

MANIP_SET = [L, iL, L2, R, iR, R2, U, iU, U2, D, iD, D2, F, iF, F2, B, iB, B2]


def vector3plusVector2(v3, v2):
	result = Position(v3)
	if result.x == PositionScalar.Zero:
		result.x = v2[0]
		if result.y == PositionScalar.Zero:
			result.y = v2[1]
		else:
			result.z = v2[1]
	else:
		result.y = v2[0]
		result.z = v2[1]

	return result

def evaluateFaceCorner(cube, face):
	ordered = []
	for x in [PositionScalar.Positive, PositionScalar.Negative]:
		for y in [PositionScalar.Positive, PositionScalar.Negative]:
			pos = vector3plusVector2(faceToPosition(face), (x, y))
			f = mul(face, cube.getUnit(pos).orientation)
			if f == face:
				ordered.append(pos)

	qcounts = {}
	for p in ordered:
		qid = cube.getUnit(p).orientation.validId()
		qcounts[qid] = qcounts.get(qid, 0) + 1

	max_c = 0
	max_k = None
	for k, count in enumerate(qcounts):
		if count > max_c:
			max_c = count
			max_k = k

	return max_c, max_k

def evaluateFaceEdge(cube, face, qid):
	count = 0
	for xy in [
		(PositionScalar.Positive, PositionScalar.Zero),
		(PositionScalar.Negative, PositionScalar.Zero),
		(PositionScalar.Zero, PositionScalar.Positive),
		(PositionScalar.Zero, PositionScalar.Negative),
	]:
		pos = vector3plusVector2(faceToPosition(face), xy)
		orient = cube.getUnit(pos).orientation
		if orient.validId() == qid:
			f = mul(face, orient)
			if f == face:
				count += 1

	return count

# evaluate ordered edge blocks in face: y = PositionScalar.Zero
def evaluateMiddleEdges(cube):
	count = 0
	for pos in [
		Position(PositionScalar.Positive, PositionScalar.Zero, PositionScalar.Positive),
		Position(PositionScalar.Negative, PositionScalar.Zero, PositionScalar.Positive),
		Position(PositionScalar.Positive, PositionScalar.Zero, PositionScalar.Negative),
		Position(PositionScalar.Negative, PositionScalar.Zero, PositionScalar.Negative),
	]:
		index = MagicCube.ZERO.getUnitIndex(pos)
		uc0 = MagicCube.ZERO.getUnit(index)
		uc = cube.getUnit(index)
		if uc == uc0:
			count += 1

	return count


def evaluateGrade(cube):
	grade = 0

	if cube.logicEqual(MagicCube.ZERO):
		return grade

	grade += 1

	topcorners, topqid = evaluateFaceCorner(cube, Face.pY)
	bottomcorners, bottomqid = evaluateFaceCorner(cube, Face.nY)

	topedges = evaluateFaceEdge(cube, Face.pY, topqid)
	bottomedges = evaluateFaceEdge(cube, Face.nY, bottomqid)


	# Grade 1: 3 layer ordered self, but stagger each other.

	if topcorners + bottomcorners + topedges + bottomedges == 16:
		middleedges = evaluateMiddleEdges(cube)
		if middleedges == 4:
			return grade

	grade += 1

	# Grade 2: top and bottom layer ordered, but middle layer not.

	if topcorners + bottomcorners + topedges + bottomedges == 16:
		return grade

	grade += 1

	# Grade 3: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer ordered.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 4):
		return grade

	grade += 1

	# Grade 4: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 3 edges ordered 1 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 3):
		return grade

	grade += 1

	# Grade 5: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 2 edges ordered 2 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 2):
		return grade

	grade += 1

	# Grade 6: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 1 edges ordered 3 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 1):
		return grade

	grade += 1

	# Grade 7: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: all edges unordered.

	if (topcorners + bottomcorners == 8) and (topedges == 3):
		return grade

	grade += 1

	# Grade 8: all corners ordered, top layer: 2 edges ordered 2 not.

	if (topcorners + bottomcorners == 8) and (topedges == 2):
		return grade

	grade += 1

	# Grade 9: all corners ordered, top layer: 1 edges ordered 3 not.

	if (topcorners + bottomcorners == 8) and (topedges == 1):
		return grade

	grade += 1

	# Grade 10: all corners ordered, all top layer edges unordered.

	if topcorners + bottomcorners == 8:
		return grade

	grade += 1

	# Grade 11: all top corners ordered, but bottom corners not all ordered.

	if topcorners == 4:
		return grade

	grade += 1

	# Grade 12: 3 top corners ordered.

	if topcorners == 3:
		return grade

	grade += 1

	# Grade 13: 2 top corners ordered.

	if topcorners == 2:
		return grade

	grade += 1

	# Grade 14: 1 top corner ordered.

	if topcorners == 1:
		return grade

	grade += 1

	# Grade 15: no top corner ordered.

	return grade

def advance(cube, grade, deep, face):
	global MANIP_SET

	if deep == 0:
		return None
	else:
		for m in MANIP_SET:
			if face != manipulationToFace(m):
				cube.doManipulation(m)

				g = evaluateGrade(cube)
				if g < grade:
					return [m]
				else:
					route = advance(cube, grade, deep - 1, manipulationToFace(m))
					if route:
						route = [m] + route
						return route

				cube.manipulationBackOff()

		return None

def searchRoute(cube, deep):
	grade = evaluateGrade(cube)
	print '[15grades.py]	Current grade:', grade
	if grade == 0:
		return [0], 0

	c = MagicCube(cube)
	route = advance(c, grade, deep, None) or []

	return route, grade


g_Manips = []


def initialize(cube):
	global g_Cube

	g_Cube = cube



def step():
	global g_Cube, g_Manips, SEARCH_DEEP

	if len(g_Manips) == 0:
		print '[15grades.py]	Searching route...'

		g_Manips, grade = searchRoute(g_Cube, SEARCH_DEEP)

		if grade == 0:
			print '[15grades.py]	Target achieved!'
			return 0

	if (len(g_Manips) < 1) or (g_Manips[0] == 0):
		print '[15grades.py]	Failed searching route under deep: ', SEARCH_DEEP, '.'
		return 0

	m = g_Manips[0]
	g_Manips = g_Manips[1:]
	return m
