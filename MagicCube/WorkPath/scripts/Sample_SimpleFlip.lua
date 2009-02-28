--[[
	Sample_SimpleFlip.lua

	Copyright (c) 2008 K.L.<xxxk.l.xxx@gmail.com>
	A MagicCube lua script sample.
--]]


-- 'print' will display these information in console window.
--	This text will display once script loaded.
--	Tell who is the script's author, and you can say something about the purpose of the script.
--	Text between '[[' and ']]' is treated as a string.
print [[
[Sample_SimpleFlip.lua]:
    Author:    K.L.<xxxk.l.xxx@gmail.com>
]]


-- the reference to the cube in player
g_Cube = nil

-- the current step index
g_Steps = 0

-- the fixed manipulations route to play
--	Modify the list as your will, then run the player and see what happened.
--	Available manipulations symbols include: L, iL, L2, R, iR, R2, U, iU, U2, D, iD, D2, F, iF, F2, B, iB, B2.
g_Route =
{
	L2,
	R2,
	U2,
	D2,
	F2,
	B2,
}


function initialize(cube)
	-- save the cube in player
	g_Cube = cube

	-- print series code of the initial MagicCube state.
	print("[Sample_SimpleFlip.lua]	initialize: " .. StateArchive(g_Cube):getSerializationCode())
end


function step()
	g_Steps = g_Steps + 1

	print("[Sample_SimpleFlip.lua]	step:", g_Steps)

	-- when g_Steps out of g_Route's size, g_Route[g_Steps] is nil
	if g_Route[g_Steps] == nil then
		print("[Sample_SimpleFlip.lua]	the end.")

		-- return 0 to end
		return 0
	end

	-- return the current manipulation
	return g_Route[g_Steps]
end
