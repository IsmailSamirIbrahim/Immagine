project "playground"
	language "C++"
	kind "ConsoleApp"

	files
	{
		"include/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"include/"
	}

	links
	{
		"immagine"
	}

	use_immagine()
