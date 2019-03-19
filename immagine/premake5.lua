immagine = path.getabsolute(".")

function use_immagine()
	includedirs "%{immagine}/include"
	links "immagine"
end

project "immagine"
	language "C++"
	kind "SharedLib"

	files
	{
		"include/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"include/",
		"%{stb_path}"
	}

	defines
	{
		"IMMAGINE_DLL"
	}

