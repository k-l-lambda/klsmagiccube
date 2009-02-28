--[[
	Sample_RandomSuperFlip.lua

	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
	A MagicCube lua script sample.
--]]


print [[
[Sample_RandomSuperFlip.lua]:
    Author:    K.L.<xxxk.l.xxx@gmail.com>
    Tenet:     Play a endless random super flip pattern sequence.
]]



-- the reference to the cube in player
g_Cube = nil

-- the current step index
g_Steps = 0

-- Manipulations prepared for super flip.
g_Manips = {L, iR, iL, R, U, iD, iU, D, F, iB, iF, B}


function initialize(cube)
	-- save the cube in player
	g_Cube = cube

	-- print series code of the initial MagicCube state.
	print("[Sample_RandomSuperFlip.lua]	initialize: " .. StateArchive(g_Cube):getSerializationCode())
end

function step()
	g_Steps = g_Steps + 1

	print("[Sample_RandomSuperFlip.lua]	step:", g_Steps)

	local m
	if (g_Steps % 2) == 1 then
		-- when g_Steps is odd number, pick a random couple of manipulations in prepared list,
		--	and then return the first one in couple.

		f = math.random(6)
		m = g_Manips[f * 2]
	else
		-- when g_Steps is even, return the 2nd manipulation in couple.

		m = g_Manips[f * 2 - 1]
	end

	return m
end
