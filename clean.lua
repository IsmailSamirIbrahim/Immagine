premake.modules.lua = {}
local clean_mod = premake.modules.lua

function rmfile(path)
	if os.ishost("windows") then
		path = path:gsub("/", "\\")
		os.executef("del %s", path)
	else
		premake.error("OS not supported")
	end
end

function clean_bin()
	local children = os.matchdirs("bin/*")
	for k, v in pairs(children) do
			os.rmdir(v)
	end

	children = os.matchfiles("bin/*")
	for k, v in pairs(children) do
			rmfile(v)
	end
end

function clean_build()
	children = os.matchdirs("build/*")
	for k, v in pairs(children) do
			os.rmdir(v)
	end

	children = os.matchfiles("build/*")
	for k, v in pairs(children) do
			rmfile(v)
	end
end

newaction {
	trigger = "clean",
	description = "Cleans the bin and build folders",

	execute = function()
		clean_bin()
		clean_build()
	end,
}

return clean_mod