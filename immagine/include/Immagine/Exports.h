#pragma once

#if defined(OS_WINDOWS)
	#if defined(IMMAGINE_DLL)
		#define API_IMMAGINE __declspec(dllexport)
	#else
		#define API_IMMAGINE __declspec(dllimport)
	#endif
#endif