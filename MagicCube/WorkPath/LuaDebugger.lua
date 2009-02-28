--[[
	LuaDebugger.lua

	This is MagicCube's lua script console debugger toolkit.
	Drag this file and drop on lua standard interpreter(MagicCube\lua.exe) to run the debugger.
--]]

print [[
-------------------------------------------------------------------------------
                             K.L.'s MagicCube
                               Lua Debugger

    Welcome use MagicCube lua script!
    Have fun!

    Copyright (c) 2008	K.L.<xxxk.l.xxx@gmail.com>
    K.L.'s MagicCube forum: http://groups.google.com/group/k_l_s-magiccube
    If your scripts encountered any hard problem, just tell me :)
-------------------------------------------------------------------------------
]]


function go()
	--  Register MagicCube symbols.
	package.loadlib('MagicCube.dll', 'registerLuaSymbols')()
	print '[LuaDebugger]	MagicCube symbols registered.'


	-- Modify the default package path a bit for our scripts path.
	package.path = string.gsub(package.path, '\\lua\\%?.lua', '\\scripts\\%?.lua')


	-- Input the MaigcCube script file name to debug.
	--	It should ignore extend name ".lua".
	--	e.g.
	--		If you want to debug file "MagicCube\scripts\MyScript.lua", just input "MyScript", that's OK.
	io.write('[LuaDebugger]	Input script name: ')
	filename = io.read()


	-- Simulation fuction
	function runPlayer()
		-- Input initial MagicCube state. If ignore this, CUBE0 will be used.
		local state
		repeat
			io.write('[LuaDebugger]	Initial MagicCube state series code:(optional) ')
			local series = io.read()
			if series == '' then
				series = StateArchive(CUBE0):getSerializationCode()
			end
			state = StateArchive(series)
			local valid = state:valid()
		until valid

		local cube = MagicCube(state)

		-- Call 'initialize' in script file.
		local succeed, e
		succeed, e = pcall(initialize, cref(cube))
		if not succeed then
			print('[LuaDebugger]	Failed calling \'initialize\'. Error:', e)
			return
		end

		-- Repeat calling 'step', till it return 0.
		local result
		local steps = 0
		local runsteps = 0
		repeat
			-- Update inner cube.
			if result then
				local s, e
				s, e = pcall(function() cube:doManipulation(result) end)
				if not s then
					error('Player failed updating MagicCube. Error: '.. e)
				end
			end

			-- Call 'step' in script file.
			succeed, result = pcall(step)
			if not succeed then
				print('[LuaDebugger]	Failed calling \'step\'. Error:', result)
				return
			end

			if not result then
				print '[LuaDebugger]	Error: step function returned nil, that is invalid.'
				return
			end

			if type(result) ~= type(L) then
				print('[LuaDebugger]	Warning: step returned value(type:' .. type(result)
					.. ') is not of valid type.')
				result = 0
			elseif not isValidManipulation(result) then
				print('[LuaDebugger]	Warning: step returned value(' .. result
					.. ') is not valid manipulation value.')
				result = 0
			end

			-- Report current state.
			steps = steps + 1
			print('[LuaDebugger]	Step:', steps,
				'\n	Current state:', StateArchive(cube):getSerializationCode(),
				'\n	Next manipulation:', result)

			-- Pause every step, wait for input durative executing step count.
			--	Ignore this simply, it will pause every step.
			if runsteps <= 0 then
				runsteps = tonumber(io.read()) or 0
			else
				runsteps = runsteps - 1
			end
		until result == 0
	end


	-- Parse the file
	repeat
		local s, e
		s, e = pcall(require, filename)
		if s then
			-- Parse passed, start MagicCube player simulation
			print('[LuaDebugger]	Simulation begin.')
			runPlayer()
			print('[LuaDebugger]	Simulation end.')
		else
			-- Parse failed.
			print('[LuaDebugger]	Failed to parse file:', filename .. '.lua', '\n	Error:', e)
			if string.sub(filename, string.len(filename) - 3, string.len(filename)) == '.lua' then
				print '	Maybe you should remove the file extend name(.lua), try again.'
			end
		end

		--io.write('[LuaDebugger]	Try again?(Y/N): ')
		local tryagain = false --(string.lower(io.read()) == 'y')
	until not tryagain
end


-- Catch debugger inner error.
g_s, g_e = pcall(go)
if not g_s then
	print('[LuaDebugger]	debugger inner error:', g_e)
	print '	Sorry, please report this error info to me <xxxk.l.xxx@gmail.com>.'
end


-- Pause before quit debugger.
os.execute 'pause'
