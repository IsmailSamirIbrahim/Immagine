
#ifndef IMMAGINE_EXPORT_H
#define IMMAGINE_EXPORT_H

#ifdef IMMAGINE_STATIC_DEFINE
#  define IMMAGINE_EXPORT
#  define IMMAGINE_NO_EXPORT
#else
#  ifndef IMMAGINE_EXPORT
#    ifdef immagine_EXPORTS
        /* We are building this library */
#      define IMMAGINE_EXPORT 
#    else
        /* We are using this library */
#      define IMMAGINE_EXPORT 
#    endif
#  endif

#  ifndef IMMAGINE_NO_EXPORT
#    define IMMAGINE_NO_EXPORT 
#  endif
#endif

#ifndef IMMAGINE_DEPRECATED
#  define IMMAGINE_DEPRECATED __declspec(deprecated)
#endif

#ifndef IMMAGINE_DEPRECATED_EXPORT
#  define IMMAGINE_DEPRECATED_EXPORT IMMAGINE_EXPORT IMMAGINE_DEPRECATED
#endif

#ifndef IMMAGINE_DEPRECATED_NO_EXPORT
#  define IMMAGINE_DEPRECATED_NO_EXPORT IMMAGINE_NO_EXPORT IMMAGINE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef IMMAGINE_NO_DEPRECATED
#    define IMMAGINE_NO_DEPRECATED
#  endif
#endif

#endif /* IMMAGINE_EXPORT_H */
