--[[
	15grades.lua

	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
--]]

print [[
[15grades.lua]:
    Author:    K.L.<xxxk.l.xxx@gmail.com>
    Tenet:     Restore MagicCube to zero state.
    Principle: Define 15 grades by how ordered the state is, and transit
               between grades by method of exhaustion.
    Issue:     Distance of some abutting grades is too far, and method of
               exhaustion is too unefficient, the required search deep too
               large.
]]

SEARCH_DEEP = 4

MANIP_SET = {L, iL, L2, R, iR, R2, U, iU, U2, D, iD, D2, F, iF, F2, B, iB, B2}


vector3plusVector2 = function(v3, v2)
	local result = Position(v3)
	if result.x == PositionScalar.Zero then
		result.x = v2[1]
		if result.y == PositionScalar.Zero then
			result.y = v2[2]
		else
			result.z = v2[2]
		end
	else
		result.y = v2[1]
		result.z = v2[2]
	end

	return result
end

evaluateFaceCorner = function(cube, face)
	local ordered = {}
	local k, x, y
	for k, x in pairs({PositionScalar.Positive, PositionScalar.Negative}) do
		for k, y in pairs({PositionScalar.Positive, PositionScalar.Negative}) do
			local pos = vector3plusVector2(faceToPosition(face), {x, y})
			local f = mul(face, cube:getUnit(pos).orientation)
			if f == face then
				table.insert(ordered, pos)
			end
		end
	end

	local qcounts = {}
	local p
	for k, p in pairs(ordered) do
		local qid = cube:getUnit(p).orientation:validId()
		qcounts[qid] = (qcounts[qid] or 0) + 1
	end

	local max = 0
	local max_k
	local count
	for k, count in pairs(qcounts) do
		if (count or 0) > max then
			max = count
			max_k = k
		end
	end

	return max, max_k
end

evaluateFaceEdge = function(cube, face, qid)
	local count = 0
	local k, xy
	for k, xy in pairs({
		{PositionScalar.Positive, PositionScalar.Zero},
		{PositionScalar.Negative, PositionScalar.Zero},
		{PositionScalar.Zero, PositionScalar.Positive},
		{PositionScalar.Zero, PositionScalar.Negative},
	}) do
		local pos = vector3plusVector2(faceToPosition(face), xy)
		local orient = cube:getUnit(pos).orientation
		if orient:validId() == qid then
			local f = mul(face, orient)
			if f == face then
				count = count + 1
			end
		end
	end

	return count
end

-- evaluate ordered edge blocks in face: y = PositionScalar.Zero
evaluateMiddleEdges = function(cube)
	local count = 0
	local k, pos
	for k, pos in pairs({
		Position(PositionScalar.Positive, PositionScalar.Zero, PositionScalar.Positive),
		Position(PositionScalar.Negative, PositionScalar.Zero, PositionScalar.Positive),
		Position(PositionScalar.Positive, PositionScalar.Zero, PositionScalar.Negative),
		Position(PositionScalar.Negative, PositionScalar.Zero, PositionScalar.Negative),
	}) do
		local index = MagicCube.ZERO:getUnitIndex(pos)
		local uc0 = MagicCube.ZERO:getUnit(index)
		local uc = cube:getUnit(index)
		if uc == uc0 then
			count = count + 1
		end
	end

	return count
end


evaluateGrade = function(cube)
	local grade = 0

	if cube:logicEqual(MagicCube.ZERO) then
		return grade
	end

	grade = grade + 1

	local topcorners, topqid, bottomcorners, bottomqid
	topcorners, topqid = evaluateFaceCorner(cube, Face.pY)
	bottomcorners, bottomqid = evaluateFaceCorner(cube, Face.nY)

	local topedges = evaluateFaceEdge(cube, Face.pY, topqid)
	local bottomedges = evaluateFaceEdge(cube, Face.nY, bottomqid)


	-- Grade 1: 3 layer ordered self, but stagger each other.

	if topcorners + bottomcorners + topedges + bottomedges == 16 then
		local middleedges = evaluateMiddleEdges(cube)
		if middleedges == 4 then
			return grade
		end
	end

	grade = grade + 1

	-- Grade 2: top and bottom layer ordered, but middle layer not.

	if topcorners + bottomcorners + topedges + bottomedges == 16 then
		return grade
	end

	grade = grade + 1

	-- Grade 3: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer ordered.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 4) then
		return grade
	end

	grade = grade + 1

	-- Grade 4: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 3 edges ordered 1 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 3) then
		return grade
	end

	grade = grade + 1

	-- Grade 5: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 2 edges ordered 2 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 2) then
		return grade
	end

	grade = grade + 1

	-- Grade 6: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: 1 edges ordered 3 not.

	if (topcorners + bottomcorners == 8) and (topedges == 3) and (bottomedges == 1) then
		return grade
	end

	grade = grade + 1

	-- Grade 7: all corners ordered, top layer: 3 edges ordered but 1 not; bottom layer: all edges unordered.

	if (topcorners + bottomcorners == 8) and (topedges == 3) then
		return grade
	end

	grade = grade + 1

	-- Grade 8: all corners ordered, top layer: 2 edges ordered 2 not.

	if (topcorners + bottomcorners == 8) and (topedges == 2) then
		return grade
	end

	grade = grade + 1

	-- Grade 9: all corners ordered, top layer: 1 edges ordered 3 not.

	if (topcorners + bottomcorners == 8) and (topedges == 1) then
		return grade
	end

	grade = grade + 1

	-- Grade 10: all corners ordered, all top layer edges unordered.

	if topcorners + bottomcorners == 8 then
		return grade
	end

	grade = grade + 1

	-- Grade 11: all top corners ordered, but bottom corners not all ordered.

	if topcorners == 4 then
		return grade
	end

	grade = grade + 1

	-- Grade 12: 3 top corners ordered.

	if topcorners == 3 then
		return grade
	end

	grade = grade + 1

	-- Grade 13: 2 top corners ordered.

	if topcorners == 2 then
		return grade
	end

	grade = grade + 1

	-- Grade 14: 1 top corner ordered.

	if topcorners == 1 then
		return grade
	end

	grade = grade + 1

	-- Grade 15: no top corner ordered.

	return grade
end

advance = function(cube, grade, deep, face)
	if deep == 0 then
		return nil
	else
		local i
		for i = 1, 18 do
			local m = MANIP_SET[i]
			if face ~= manipulationToFace(m) then
				cube:doManipulation(m)

				local g = evaluateGrade(cube)
				if g < grade then
					return {m}
				else
					local route = advance(cube, grade, deep - 1, manipulationToFace(m))
					if route then
						table.insert(route, 1, m)
						return route
					end
				end

				cube:manipulationBackOff()
			end
		end

		return nil
	end
end

searchRoute = function(cube, deep)
	local grade = evaluateGrade(cube)
	print('[15grades.lua]	Current grade:', grade)
	if grade == 0 then
		return {0}, 0
	end

	local c = MagicCube(cube)
	local route = advance(c, grade, deep) or {}

	return route, grade
end


g_Manips = {}


function initialize(cube)
	g_Cube = cube
end



function step()
	if table.maxn(g_Manips) <= 0 then
		print('[15grades.lua]	Searching route...')

		local grade
		g_Manips, grade = searchRoute(g_Cube, SEARCH_DEEP)

		if grade == 0 then
			print('[15grades.lua]	Target achieved!')
		end
	end

	if table.maxn(g_Manips) <= 0 then
		print('[15grades.lua]	Failed searching route under deep: ' .. SEARCH_DEEP .. '.')
		return 0
	end

	local m = g_Manips[1]
	table.remove(g_Manips, 1)
	return m
end
