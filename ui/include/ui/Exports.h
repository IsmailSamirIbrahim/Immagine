
#ifndef UI_EXPORT_H
#define UI_EXPORT_H

#ifdef UI_STATIC_DEFINE
#  define UI_EXPORT
#  define UI_NO_EXPORT
#else
#  ifndef UI_EXPORT
#    ifdef ui_EXPORTS
        /* We are building this library */
#      define UI_EXPORT 
#    else
        /* We are using this library */
#      define UI_EXPORT 
#    endif
#  endif

#  ifndef UI_NO_EXPORT
#    define UI_NO_EXPORT 
#  endif
#endif

#ifndef UI_DEPRECATED
#  define UI_DEPRECATED __declspec(deprecated)
#endif

#ifndef UI_DEPRECATED_EXPORT
#  define UI_DEPRECATED_EXPORT UI_EXPORT UI_DEPRECATED
#endif

#ifndef UI_DEPRECATED_NO_EXPORT
#  define UI_DEPRECATED_NO_EXPORT UI_NO_EXPORT UI_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef UI_NO_DEPRECATED
#    define UI_NO_DEPRECATED
#  endif
#endif

#endif /* UI_EXPORT_H */
