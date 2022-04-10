#pragma once	//GUDANR

#ifndef __CONFIG_H__
#define  __CONFIG_H__

#ifdef GUDANR
#include <glib.h>
#else
#include "_config_.h"
#endif

typedef char   gchar;
typedef short  gshort;
typedef long   glong;
typedef int    gint;
typedef gint   gboolean;

typedef unsigned char   guchar;
typedef unsigned short  gushort;
typedef unsigned long   gulong;
typedef unsigned int    guint;

typedef float   gfloat;
typedef double  gdouble;

typedef signed int gint32;
typedef unsigned int guint32;

typedef signed char gint8;
typedef unsigned char guint8;

typedef signed short gint16;
typedef unsigned short guint16;

typedef signed int gssize;
typedef unsigned int gsize;

typedef void* gpointer;

typedef	long	ssize_t;

typedef const void* gconstpointer;

struct _GArray
{
	gchar* data;
	guint len;
};
typedef struct _GArray		GArray;

#define g_array_append_val(a,v)	  g_array_append_vals (a, &(v), 1)


GArray* g_array_sized_new(gboolean zero_terminated,	gboolean clear_, guint element_size, guint reserved_size);
GArray* g_array_new(gboolean zero_terminated, gboolean clear_, guint element_size);
gchar* g_array_free(GArray* array, gboolean free_segment);
GArray* g_array_append_vals(GArray* array, gconstpointer data, guint len);
GArray* g_array_remove_index(GArray* array,	guint index_);
GArray* g_array_remove_range(GArray* array,	guint index_, guint length);

struct _GString
{
	gchar* str;
	gsize len;
	gsize allocated_len;
};

typedef struct _GString         GString;

// gdk/gdkcolor.h
struct _GdkColor
{
  guint32 pixel;
  guint16 red;
  guint16 green;
  guint16 blue;
};

// gdktypes.h
typedef struct _GdkColor		GdkColor;



// gtype.h
#if     GLIB_SIZEOF_SIZE_T != GLIB_SIZEOF_LONG || !defined __cplusplus
typedef gsize                           GType;
#else   /* for historic reasons, C++ links against gulong GTypes */
typedef gulong                          GType;
#endif

struct _GTypeClass
{
	/*< private >*/
	GType g_type;
};

typedef struct _GTypeClass              GTypeClass;



// gtype.h
struct _GTypeInstance
{
	/*< private >*/
	GTypeClass* g_class;
};


typedef struct _GData           GData;

typedef struct _GTypeInstance           GTypeInstance;

// gobject.h
struct  _GObject
{
	GTypeInstance  g_type_instance;

	/*< private >*/
	guint          ref_count;  /* (atomic) */
	GData* qdata;
};

typedef struct _GObject                  GObject;

#ifdef GUDANR
// gdkdrawable.h
struct _GdkDrawable
{
	GObject parent_instance;
};

// gdktypes.h
typedef struct _GdkDrawable		GdkPixmap
#endif

#define _G_NEW(struct_type, n_structs, func) \
        ((struct_type *) func ((n_structs), sizeof (struct_type)))
#define _G_RENEW(struct_type, mem, n_structs, func) \
        ((struct_type *) g_##func##_n (mem, (n_structs), sizeof (struct_type)))

#define FALSE	0
#define TRUE	1

#define g_new0(struct_type, n_structs)			_G_NEW (struct_type, n_structs, calloc)

#define g_new(struct_type, n_structs)			_G_NEW (struct_type, n_structs, calloc)

#define g_renew(struct_type, mem, n_structs)		_G_RENEW (struct_type, mem, n_structs, realloc)

#define g_array_index(a,t,i)      (((t*) (void *) (a)->data) [(i)])

#include <stdarg.h>

gchar* g_strdup_printf(const gchar* format, ...);
gchar* strdup_vprintf(const gchar* format, va_list  args);
gchar* g_strstr_len(const gchar* haystack,	gssize haystack_len, const gchar* needle);

#define G_DIR_SEPARATOR '\\'

#define        __S_IFMT        0170000        /* These bits determine file type.  */
/* File types.  */
#define        __S_IFDIR        0040000        /* Directory.  */
#define        __S_IFCHR        0020000        /* Character device.  */
#define        __S_IFBLK        0060000        /* Block device.  */
#define        __S_IFREG        0100000        /* Regular file.  */
#define        __S_IFIFO        0010000        /* FIFO.  */
#define        __S_IFLNK        0120000        /* Symbolic link.  */
#define        __S_IFSOCK        0140000        /* Socket.  */

#define __S_ISTYPE(mode, mask)  (((mode) & __S_IFMT) == (mask))
#define S_ISREG(mode)    __S_ISTYPE((mode), __S_IFREG)


/* access function */
#define	F_OK		0	/* test for existence of file */
#define	X_OK		0x01	/* test for execute or search permission */
#define	W_OK		0x02	/* test for write permission */
#define	R_OK		0x04	/* test for read permission */


#define GLIB_SIZEOF_SIZE_T 8

#ifndef UINT_WIDTH
#define UINT_WIDTH 32
#endif

/* Provide simple macro statement wrappers:
 *   G_STMT_START { statements; } G_STMT_END;
 * This can be used as a single statement, like:
 *   if (x) G_STMT_START { ... } G_STMT_END; else ...
 * This intentionally does not use compiler extensions like GCC's '({...})' to
 * avoid portability issue or side effects when compiled with different compilers.
 * MSVC complains about "while(0)": C4127: "Conditional expression is constant",
 * so we use __pragma to avoid the warning since the use here is intentional.
 */
#if !(defined (G_STMT_START) && defined (G_STMT_END))
#define G_STMT_START  do
#if defined (_MSC_VER) && (_MSC_VER >= 1500)
#define G_STMT_END \
    __pragma(warning(push)) \
    __pragma(warning(disable:4127)) \
    while(0) \
    __pragma(warning(pop))
#else
#define G_STMT_END    while (0)
#endif
#endif

#define g_return_val_if_fail(expr,val) G_STMT_START{ (void)0; }G_STMT_END



#define G_MINFLOAT	FLT_MIN
#define G_MAXFLOAT	FLT_MAX
#define G_MINDOUBLE	DBL_MIN
#define G_MAXDOUBLE	DBL_MAX
#define G_MINSHORT	SHRT_MIN
#define G_MAXSHORT	SHRT_MAX
#define G_MAXUSHORT	USHRT_MAX
#define G_MININT	INT_MIN
#define G_MAXINT	INT_MAX
#define G_MAXUINT	UINT_MAX
#define G_MINLONG	LONG_MIN
#define G_MAXLONG	LONG_MAX
#define G_MAXULONG	ULONG_MAX

#define G_MAXSIZE	G_MAXUINT
#define G_MINSSIZE	G_MININT
#define G_MAXSSIZE	G_MAXINT

#define G_MAXUINT16	((guint16) 0xffff)


#define G_LIKELY(expr) (expr)
#define G_UNLIKELY(expr) (expr)

extern gboolean g_mem_gc_friendly;

#ifndef _GLIB_EXTERN
#define _GLIB_EXTERN extern
#endif

#define GLIB_AVAILABLE_IN_ALL                   _GLIB_EXTERN


#ifdef GUDANR

#ifdef __has_attribute
#define g_macro__has_attribute __has_attribute
#else

/*
 * Fallback for GCC < 5 and other compilers not supporting __has_attribute.
 */
#define g_macro__has_attribute(x) g_macro__has_attribute_##x

#define g_macro__has_attribute___pure__ G_GNUC_CHECK_VERSION (2, 96)
#define g_macro__has_attribute___malloc__ G_GNUC_CHECK_VERSION (2, 96)
#define g_macro__has_attribute___noinline__ G_GNUC_CHECK_VERSION (2, 96)
#define g_macro__has_attribute___sentinel__ G_GNUC_CHECK_VERSION (4, 0)
#define g_macro__has_attribute___alloc_size__ G_GNUC_CHECK_VERSION (4, 3)
#define g_macro__has_attribute___format__ G_GNUC_CHECK_VERSION (2, 4)
#define g_macro__has_attribute___format_arg__ G_GNUC_CHECK_VERSION (2, 4)
#define g_macro__has_attribute___noreturn__ (G_GNUC_CHECK_VERSION (2, 8) || (0x5110 <= __SUNPRO_C))
#define g_macro__has_attribute___const__ G_GNUC_CHECK_VERSION (2, 4)
#define g_macro__has_attribute___unused__ G_GNUC_CHECK_VERSION (2, 4)
#define g_macro__has_attribute___no_instrument_function__ G_GNUC_CHECK_VERSION (2, 4)
#define g_macro__has_attribute_fallthrough G_GNUC_CHECK_VERSION (6, 0)
#define g_macro__has_attribute___deprecated__ G_GNUC_CHECK_VERSION (3, 1)
#define g_macro__has_attribute_may_alias G_GNUC_CHECK_VERSION (3, 3)
#define g_macro__has_attribute_warn_unused_result G_GNUC_CHECK_VERSION (3, 4)
#define g_macro__has_attribute_cleanup G_GNUC_CHECK_VERSION (3, 3)

#endif

/* Note: We can¡¯t annotate this with GLIB_AVAILABLE_MACRO_IN_2_58 because it¡¯s
 * used within the GLib headers in function declarations which are always
 * evaluated when a header is included. This results in warnings in third party
 * code which includes glib.h, even if the third party code doesn¡¯t use the new
 * macro itself. */

#if g_macro__has_attribute(__pure__)
#define G_GNUC_PURE __attribute__((__pure__))
#else
#define G_GNUC_PURE
#endif

#if g_macro__has_attribute(__malloc__)
#define G_GNUC_MALLOC __attribute__ ((__malloc__))
#else
#define G_GNUC_MALLOC
#endif

#if g_macro__has_attribute(__noinline__)
#define G_GNUC_NO_INLINE __attribute__ ((__noinline__))
#else
#define G_GNUC_NO_INLINE
#endif

#if g_macro__has_attribute(__alloc_size__)
#define G_GNUC_ALLOC_SIZE(x) __attribute__((__alloc_size__(x)))
#define G_GNUC_ALLOC_SIZE2(x,y) __attribute__((__alloc_size__(x,y)))
#else
#define G_GNUC_ALLOC_SIZE(x)
#define G_GNUC_ALLOC_SIZE2(x,y)
#endif

/* slices - fast allocation/release of small memory blocks
 */
GLIB_AVAILABLE_IN_ALL
gpointer g_slice_alloc          	(gsize	       block_size) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
GLIB_AVAILABLE_IN_ALL
gpointer g_slice_alloc0         	(gsize         block_size) G_GNUC_MALLOC G_GNUC_ALLOC_SIZE(1);
GLIB_AVAILABLE_IN_ALL
gpointer g_slice_copy               (gsize         block_size, gconstpointer mem_block) G_GNUC_ALLOC_SIZE(1);
GLIB_AVAILABLE_IN_ALL
void     g_slice_free1          	(gsize         block_size, gpointer      mem_block);
GLIB_AVAILABLE_IN_ALL
void     g_slice_free_chain_with_offset (gsize     block_size, gpointer      mem_chain,	gsize next_offset);
#define  g_slice_new(type)			((type*)	   g_slice_alloc (sizeof (type)))

/* Allow the compiler to inline memset(). Since the size is a constant, this
 * can significantly improve performance. */
#if defined (__GNUC__) && (__GNUC__ >= 2) && defined (__OPTIMIZE__)
#  define g_slice_new0(type)                                    \
  (type *) (G_GNUC_EXTENSION ({                                 \
    gsize __s = sizeof (type);                                  \
    gpointer __p;                                               \
    __p = g_slice_alloc (__s);                                  \
    memset (__p, 0, __s);                                       \
    __p;                                                        \
  }))
#else
#  define g_slice_new0(type)    ((type*) g_slice_alloc0 (sizeof (type)))
#endif

/* MemoryBlockType *
 *       g_slice_dup                    (MemoryBlockType,
 *	                                 MemoryBlockType *mem_block);
 *       g_slice_free                   (MemoryBlockType,
 *	                                 MemoryBlockType *mem_block);
 *       g_slice_free_chain             (MemoryBlockType,
 *                                       MemoryBlockType *first_chain_block,
 *                                       memory_block_next_field);
 * pseudo prototypes for the macro
 * definitions following below.
 */

/* we go through extra hoops to ensure type safety */
#define g_slice_dup(type, mem)                                  \
  (1 ? (type*) g_slice_copy (sizeof (type), (mem))              \
     : ((void) ((type*) 0 == (mem)), (type*) 0))
#define g_slice_free(type, mem)                                 \
G_STMT_START {                                                  \
  if (1) g_slice_free1 (sizeof (type), (mem));			\
  else   (void) ((type*) 0 == (mem)); 				\
} G_STMT_END
#define g_slice_free_chain(type, mem_chain, next)               \
G_STMT_START {                                                  \
  if (1) g_slice_free_chain_with_offset (sizeof (type),		\
                 (mem_chain), G_STRUCT_OFFSET (type, next)); 	\
  else   (void) ((type*) 0 == (mem_chain));			\
} G_STMT_END

#endif // GUDANR

/* Functions like the ones in <ctype.h> that are not affected by locale. */
typedef enum {
	G_ASCII_ALNUM = 1 << 0,
	G_ASCII_ALPHA = 1 << 1,
	G_ASCII_CNTRL = 1 << 2,
	G_ASCII_DIGIT = 1 << 3,
	G_ASCII_GRAPH = 1 << 4,
	G_ASCII_LOWER = 1 << 5,
	G_ASCII_PRINT = 1 << 6,
	G_ASCII_PUNCT = 1 << 7,
	G_ASCII_SPACE = 1 << 8,
	G_ASCII_UPPER = 1 << 9,
	G_ASCII_XDIGIT = 1 << 10
} GAsciiType;


extern const guint16* const g_ascii_table;

#define G_IS_DIR_SEPARATOR(c) ((c) == G_DIR_SEPARATOR || (c) == '/')

GLIB_AVAILABLE_IN_ALL
gboolean     g_path_is_absolute(const gchar* file_name);

GLIB_AVAILABLE_IN_ALL
gchar* g_build_filename(const gchar* first_element,	...);

GLIB_AVAILABLE_IN_ALL
gchar* g_get_current_dir(void);

#define G_GSIZE_FORMAT "lu"

GLIB_AVAILABLE_IN_ALL
GString* g_string_new(const gchar* init);

GLIB_AVAILABLE_IN_ALL
GString* g_string_sized_new(gsize            dfl_size);

GLIB_AVAILABLE_IN_ALL
GString* g_string_insert_len(GString* string,
	gssize           pos,
	const gchar* val,
	gssize           len);

GLIB_AVAILABLE_IN_ALL
GString* g_string_append(GString* string,
	const gchar* val);

GLIB_AVAILABLE_IN_ALL
GString* g_string_append_len(GString* string,
	const gchar* val,
	gssize           len);

GLIB_AVAILABLE_IN_ALL
gchar* g_string_free(GString* string, gboolean free_segment);

#define g_ascii_isalpha(c) \
  ((g_ascii_table[(guchar) (c)] & G_ASCII_ALPHA) != 0)

GLIB_AVAILABLE_IN_ALL
int     g_strcmp0(const char* str1,
	const char* str2);


gchar* g_path_get_dirname(const gchar* file_name);

gboolean g_utf8_validate(const gchar* str, gssize max_len,	const gchar** end);

gboolean g_utf8_validate_len(const char* str, gsize max_len, const gchar** end);

#ifdef GUDANR
#define g_warning(...)  g_log (G_LOG_DOMAIN,         \
                               G_LOG_LEVEL_WARNING,  \
                               __VA_ARGS__)
#else
#define g_warning(...) 
#endif

gchar* g_stpcpy(gchar* dest,
	const char* src);

gchar* g_strconcat(const gchar* string1, ...);

gint g_vasprintf(gchar** string, gchar const* format, va_list args);

gchar* g_strdup_vprintf(const gchar* format, va_list args);

gchar* g_strdup_printf(const gchar* format,	...);

gchar* g_path_get_basename(const gchar* file_name);

gsize	g_printf_string_upper_bound(const gchar* format, va_list args);

#define _g_vsprintf  vsprintf
#define _g_vsnprintf vsnprintf

/* Define G_VA_COPY() to do the right thing for copying va_list variables.
 * glibconfig.h may have already defined G_VA_COPY as va_copy or __va_copy.
 */
#if !defined (G_VA_COPY)
#  if defined (__GNUC__) && defined (__PPC__) && (defined (_CALL_SYSV) || defined (_WIN32))
#    define G_VA_COPY(ap1, ap2)	  (*(ap1) = *(ap2))
#  elif defined (G_VA_COPY_AS_ARRAY)
#    define G_VA_COPY(ap1, ap2)	  memmove ((ap1), (ap2), sizeof (va_list))
#  else /* va_list is a pointer */
#    define G_VA_COPY(ap1, ap2)	  ((ap1) = (ap2))
#  endif /* va_list is a pointer */
#endif /* !G_VA_COPY */

gpointer g_realloc(gpointer	 mem, gsize	 n_bytes);

gpointer g_realloc_n(gpointer mem, gsize n_blocks, gsize n_block_bytes);

typedef struct _GError GError;

typedef guint32 GQuark;

struct _GError
{
	GQuark       domain;
	gint         code;
	gchar* message;
};


typedef struct _GIConv* GIConv;
gchar* g_convert(const gchar* str,	gssize        len,	const gchar* to_codeset,	const gchar* from_codeset,	gsize* bytes_read,	gsize* bytes_written,	GError** error);

const gchar* g_basename(const gchar* file_name);


gchar* g_strrstr(const gchar* haystack,	const gchar* needle);

#define G_DIR_SEPARATOR_S		"\\"
#define GERBV_PATH_DELIMETER    ";"
#define GERBV_DIR_SEPARATOR_S   "\\"
#define GERBV_DIR_SEPARATOR_C   '\\'
#define BINDIR                  "."
#define BACKEND_DIR             ""

#define BINDIR_TO_EXECPREFIX    ""
#define BINDIR_TO_PKGDATADIR    "share\\gerbv"
#define SCMSUBDIR               "scheme"
#define G_SEARCHPATH_SEPARATOR_S "="




#include <string.h>

#define g_free				free
#define g_strdup			_strdup
#define	strdup				_strdup
#define g_malloc(x)			calloc(x, 1)
#define stricmp				_stricmp
#define strlwr				_strlwr

#include <stdio.h>
#include <io.h>

#define g_fopen				fopen
#include <float.h>

#define fileno				_fileno
#define access				_access

#include <assert.h>
#define g_assert			assert

#include <stdlib.h>
#define g_getenv			getenv
#define g_setenv			_putenv


gchar* g_build_filename(const gchar* first_element,	...) ;

gchar* g_build_filenamev(gchar** args) ;

gchar* g_build_filename_valist(const gchar* first_element,	va_list* args) ;


#endif	 // #ifndef __CONFIG_H__