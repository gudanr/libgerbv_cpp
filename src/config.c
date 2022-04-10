
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <assert.h>

gboolean g_mem_gc_friendly = TRUE;

static const guint16 ascii_table_data[256] = {
  0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
  0x004, 0x104, 0x104, 0x004, 0x104, 0x104, 0x004, 0x004,
  0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
  0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004, 0x004,
  0x140, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
  0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
  0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459, 0x459,
  0x459, 0x459, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
  0x0d0, 0x653, 0x653, 0x653, 0x653, 0x653, 0x653, 0x253,
  0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
  0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253, 0x253,
  0x253, 0x253, 0x253, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x0d0,
  0x0d0, 0x473, 0x473, 0x473, 0x473, 0x473, 0x473, 0x073,
  0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
  0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073, 0x073,
  0x073, 0x073, 0x073, 0x0d0, 0x0d0, 0x0d0, 0x0d0, 0x004
  /* the upper 128 are all zeroes */
};

const guint16* const g_ascii_table = ascii_table_data;


gchar*
strdup_vprintf(const gchar* format,   va_list      args)
{
    gchar* string = NULL;

    const int len = vsnprintf(string, 0, format, args) + 1;

    if (len > 0) {
        string = (gchar*)malloc(len);
        vsnprintf(string, len, format, args);
    }

    return string;
}

/**
 * g_strdup_printf:
 * @format: (not nullable): a standard printf() format string, but notice
 *     [string precision pitfalls][string-precision]
 * @...: the parameters to insert into the format string
 *
 * Similar to the standard C sprintf() function but safer, since it
 * calculates the maximum space required and allocates memory to hold
 * the result. The returned string should be freed with g_free() when no
 * longer needed.
 *
 * The returned string is guaranteed to be non-NULL, unless @format
 * contains `%lc` or `%ls` conversions, which can fail if no multibyte
 * representation is available for the given character.
 *
 * Returns: a newly-allocated string holding the result
 */
#if(0)
gchar*
g_strdup_printf(const gchar* format, ...)
{
    gchar* buffer;
    va_list args;

    va_start(args, format);
    buffer = strdup_vprintf(format, args);
    va_end(args);

    return buffer;
}
#endif

/**
 * g_strstr_len:
 * @haystack: a nul-terminated string
 * @haystack_len: the maximum length of @haystack in bytes. A length of -1
 *     can be used to mean "search the entire string", like `strstr()`.
 * @needle: the string to search for
 *
 * Searches the string @haystack for the first occurrence
 * of the string @needle, limiting the length of the search
 * to @haystack_len.
 *
 * Returns: a pointer to the found occurrence, or
 *    %NULL if not found.
 */
gchar*
g_strstr_len(const gchar* haystack,
    gssize       haystack_len,
    const gchar* needle)
{
    if(haystack == NULL) NULL;
    if(needle == NULL) NULL;

    if (haystack_len < 0)
        return strstr(haystack, needle);
    else
    {
        const gchar* p = haystack;
        gsize needle_len = (gsize)strlen(needle);
        gsize haystack_len_unsigned = haystack_len;
        const gchar* end;
        gsize i;

        if (needle_len == 0)
            return (gchar*)haystack;

        if (haystack_len_unsigned < needle_len)
            return NULL;

        end = haystack + haystack_len - needle_len;

        while (p <= end && *p)
        {
            for (i = 0; i < needle_len; i++)
                if (p[i] != needle[i])
                    goto next;

            return (gchar*)p;

        next:
            p++;
        }

        return NULL;
    }
}

#define MIN_ARRAY_SIZE  16

typedef struct _GRealArray  GRealArray;

/**
 * GArray:
 * @data: a pointer to the element data. The data may be moved as
 *     elements are added to the #GArray.
 * @len: the number of elements in the #GArray not including the
 *     possible terminating zero element.
 *
 * Contains the public fields of a GArray.
 */
struct _GRealArray
{
    guint8* data;
    guint   len;
    guint   elt_capacity;
    guint   elt_size;
    guint   zero_terminated : 1;
    guint   clear : 1;
    int     ref_count;
    void*   clear_func;
};

#define g_array_elt_len(array,i) ((gsize)(array)->elt_size * (i))
#define g_array_elt_pos(array,i) ((array)->data + g_array_elt_len((array),(i)))
#define g_array_elt_zero(array, pos, len)                               \
  (memset (g_array_elt_pos ((array), pos), 0,  g_array_elt_len ((array), len)))
#define g_array_zero_terminate(array) G_STMT_START{                     \
  if ((array)->zero_terminated)                                         \
    g_array_elt_zero ((array), (array)->len, 1);                        \
}G_STMT_END

static gint
g_nearest_pow(gint num)
{
    gint n = 1;

    while (n < num)
        n <<= 1;

    return n;
}


GArray*
g_array_new(gboolean zero_terminated, gboolean clear, guint elt_size)
{
    g_return_val_if_fail(elt_size > 0, NULL);
#if (UINT_WIDTH / 8) >= GLIB_SIZEOF_SIZE_T
    g_return_val_if_fail(elt_size <= G_MAXSIZE / 2 - 1, NULL);
#endif

    return g_array_sized_new(zero_terminated, clear, elt_size, 0);
}



typedef enum
{
    FREE_SEGMENT = 1 << 0,
    PRESERVE_WRAPPER = 1 << 1
} ArrayFreeFlags;

static gchar* array_free(GRealArray*, ArrayFreeFlags);

gchar* g_array_free(GArray* farray, gboolean  free_segment)
{
    GRealArray* array = (GRealArray*)farray;
    ArrayFreeFlags flags;

    g_return_val_if_fail(array, NULL);

    flags = (free_segment ? FREE_SEGMENT : 0);

    /* if others are holding a reference, preserve the wrapper but do free/return the data */
    if (!(--array->ref_count))
        flags |= PRESERVE_WRAPPER;

    return array_free(array, flags);
}

static gchar* array_free(GRealArray* array, ArrayFreeFlags  flags)
{
    gchar* segment;

    if (flags & FREE_SEGMENT)
    {
        if (array->clear_func != NULL)
        {
            guint i;

            for (i = 0; i < array->len; i++)
#ifdef GUDANR
                array->clear_func(g_array_elt_pos(array, i));
#else
                ;
#endif
        }

        free(array->data);
        segment = NULL;
    }
    else
        segment = (gchar*)array->data;

    if (flags & PRESERVE_WRAPPER)
    {
        array->data = NULL;
        array->len = 0;
        array->elt_capacity = 0;
    }
    else
    {
        free(array);
    }

    return segment;
}

static void
g_array_maybe_expand(GRealArray* array, guint len);

GArray*
g_array_append_vals(GArray* farray, gconstpointer data, guint len)
{
    GRealArray* array = (GRealArray*)farray;

    g_return_val_if_fail(array, NULL);

    if (len == 0)
        return farray;

    g_array_maybe_expand(array, len);

    memcpy(g_array_elt_pos(array, array->len), data,
        g_array_elt_len(array, len));

    array->len += len;

    g_array_zero_terminate(array);

    return farray;
}

static void
g_array_maybe_expand(GRealArray* array,
    guint       len)
{
    guint max_len, want_len;

    /* The maximum array length is derived from following constraints:
     * - The number of bytes must fit into a gsize / 2.
     * - The number of elements must fit into guint.
     * - zero terminated arrays must leave space for the terminating element
     */
    max_len = min(G_MAXSIZE / 2 / array->elt_size, G_MAXUINT) - array->zero_terminated;

#ifdef GUDANR
    /* Detect potential overflow */
    if G_UNLIKELY((max_len - array->len) < len)
        g_error("adding %u to array would overflow", len);
#endif

    want_len = array->len + len + array->zero_terminated;
    if (want_len > array->elt_capacity)
    {
        gsize want_alloc = g_nearest_pow(g_array_elt_len(array, want_len));
        want_alloc = max(want_alloc, MIN_ARRAY_SIZE);

        array->data = realloc(array->data, want_alloc);

        if (G_UNLIKELY(g_mem_gc_friendly))
            memset(g_array_elt_pos(array, array->elt_capacity), 0,
                g_array_elt_len(array, want_len - array->elt_capacity));

        array->elt_capacity = min(want_alloc / array->elt_size, G_MAXUINT);
    }
}


/**
 * g_array_sized_new:
 * @zero_terminated: %TRUE if the array should have an extra element at
 *     the end with all bits cleared
 * @clear_: %TRUE if all bits in the array should be cleared to 0 on
 *     allocation
 * @element_size: size of each element in the array
 * @reserved_size: number of elements preallocated
 *
 * Creates a new #GArray with @reserved_size elements preallocated and
 * a reference count of 1. This avoids frequent reallocation, if you
 * are going to add many elements to the array. Note however that the
 * size of the array is still 0.
 *
 * Returns: the new #GArray
 */
GArray* g_array_sized_new(gboolean zero_terminated, gboolean clear, guint elt_size, guint reserved_size)
{
    GRealArray* array;

    g_return_val_if_fail(elt_size > 0, NULL);
#if (UINT_WIDTH / 8) >= GLIB_SIZEOF_SIZE_T
    g_return_val_if_fail(elt_size <= G_MAXSIZE / 2 - 1, NULL);
#endif

#ifdef GUDANR
    array = g_slice_new(GRealArray);
#else
    array = malloc(sizeof(GRealArray));
#endif

    array->data = NULL;
    array->len = 0;
    array->elt_capacity = 0;
    array->zero_terminated = (zero_terminated ? 1 : 0);
    array->clear = (clear ? 1 : 0);
    array->elt_size = elt_size;
    array->clear_func = NULL;

    array->ref_count = 0;

    if (array->zero_terminated || reserved_size != 0)
    {
        g_array_maybe_expand(array, reserved_size);
        g_array_zero_terminate(array);
    }

    return (GArray*)array;
}


GArray*
g_array_remove_index(GArray* farray,
    guint   index_)
{
    GRealArray* array = (GRealArray*)farray;

    g_return_val_if_fail(array, NULL);

    g_return_val_if_fail(index_ < array->len, NULL);

#ifdef GUDANR
    if (array->clear_func != NULL)
        array->clear_func(g_array_elt_pos(array, index_));
#endif

    if (index_ != array->len - 1)
        memmove(g_array_elt_pos(array, index_),
            g_array_elt_pos(array, index_ + 1),
            g_array_elt_len(array, array->len - index_ - 1));

    array->len -= 1;

    if (G_UNLIKELY(g_mem_gc_friendly))
        g_array_elt_zero(array, array->len, 1);
    else
        g_array_zero_terminate(array);

    return farray;
}

GArray*
g_array_remove_range(GArray* farray,
    guint   index_,
    guint   length)
{
    GRealArray* array = (GRealArray*)farray;

    g_return_val_if_fail(array, NULL);
    g_return_val_if_fail(index_ <= array->len, NULL);
    g_return_val_if_fail(index_ + length <= array->len, NULL);

    if (array->clear_func != NULL)
    {
#ifdef GUDANR
        guint i;

        for (i = 0; i < length; i++)
            array->clear_func(g_array_elt_pos(array, index_ + i));
#endif
    }

    if (index_ + length != array->len)
        memmove(g_array_elt_pos(array, index_),
            g_array_elt_pos(array, index_ + length),
            (array->len - (index_ + length)) * array->elt_size);

    array->len -= length;
    if (G_UNLIKELY(g_mem_gc_friendly))
        g_array_elt_zero(array, array->len, length);
    else
        g_array_zero_terminate(array);

    return farray;
}

GString*
g_string_append(GString* string,
    const gchar* val)
{
    return g_string_insert_len(string, -1, val, -1);
}


GString*
g_string_append_len(GString* string,
    const gchar* val,
    gssize       len)
{
    return g_string_insert_len(string, -1, val, len);
}


static void
g_string_maybe_expand(GString* string,
    gsize    len)
{
#ifdef GUDANR
    /* Detect potential overflow */
    if G_UNLIKELY((G_MAXSIZE - string->len - 1) < len)
        g_error("adding %" G_GSIZE_FORMAT " to string would overflow", len);
#endif

    if (string->len + len >= string->allocated_len)
    {
        string->allocated_len = g_nearest_pow(string->len + len + 1);
        /* If the new size is bigger than G_MAXSIZE / 2, only allocate enough
         * memory for this string and don't over-allocate. */
        if (string->allocated_len == 0)
            string->allocated_len = string->len + len + 1;
        string->str = realloc(string->str, string->allocated_len);
    }
}


GString*
g_string_insert_len(GString* string,
    gssize       pos,
    const gchar* val,
    gssize       len)
{
    gsize len_unsigned, pos_unsigned;

    g_return_val_if_fail(string != NULL, NULL);
    g_return_val_if_fail(len == 0 || val != NULL, string);

    if (len == 0)
        return string;

    if (len < 0)
        len = (int)strlen(val);
    len_unsigned = len;

    if (pos < 0)
        pos_unsigned = string->len;
    else
    {
        pos_unsigned = pos;
        g_return_val_if_fail(pos_unsigned <= string->len, string);
    }

    /* Check whether val represents a substring of string.
     * This test probably violates chapter and verse of the C standards,
     * since ">=" and "<=" are only valid when val really is a substring.
     * In practice, it will work on modern archs.
     */
    if (G_UNLIKELY(val >= string->str && val <= string->str + string->len))
    {
        gsize offset = (gsize)(val - string->str);
        gsize precount = 0;

        g_string_maybe_expand(string, len_unsigned);
        val = string->str + offset;
        /* At this point, val is valid again.  */

        /* Open up space where we are going to insert.  */
        if (pos_unsigned < string->len)
            memmove(string->str + pos_unsigned + len_unsigned,
                string->str + pos_unsigned, string->len - pos_unsigned);

        /* Move the source part before the gap, if any.  */
        if (offset < pos_unsigned)
        {
            precount = min(len_unsigned, pos_unsigned - offset);
            memcpy(string->str + pos_unsigned, val, precount);
        }

        /* Move the source part after the gap, if any.  */
        if (len_unsigned > precount)
            memcpy(string->str + pos_unsigned + precount,
                val + /* Already moved: */ precount +
                /* Space opened up: */ len_unsigned,
                len_unsigned - precount);
    }
    else
    {
        g_string_maybe_expand(string, len_unsigned);

        /* If we aren't appending at the end, move a hunk
         * of the old string to the end, opening up space
         */
        if (pos_unsigned < string->len)
            memmove(string->str + pos_unsigned + len_unsigned,
                string->str + pos_unsigned, string->len - pos_unsigned);

        /* insert the new string */
        if (len_unsigned == 1)
            string->str[pos_unsigned] = *val;
        else
            memcpy(string->str + pos_unsigned, val, len_unsigned);
    }

    string->len += len_unsigned;

    string->str[string->len] = 0;

    return string;
}


gboolean
g_path_is_absolute(const gchar* file_name)
{
    g_return_val_if_fail(file_name != NULL, FALSE);

    if (G_IS_DIR_SEPARATOR(file_name[0]))
        return TRUE;

//#ifdef G_OS_WIN32
    /* Recognize drive letter on native Windows */
    if (g_ascii_isalpha(file_name[0]) &&
        file_name[1] == ':' && G_IS_DIR_SEPARATOR(file_name[2]))
        return TRUE;
//#endif

    return FALSE;
}

static gchar*
g_build_pathname_va(const gchar* first_element,
    va_list* args,
    gchar** str_array)
{
    /* Code copied from g_build_pathv(), and modified to use two
     * alternative single-character separators.
     */
    GString* result;
    gboolean is_first = TRUE;
    gboolean have_leading = FALSE;
    const gchar* single_element = NULL;
    const gchar* next_element;
    const gchar* last_trailing = NULL;
    gchar current_separator = '\\';
    gint i = 0;

    result = g_string_new(NULL);

    if (str_array)
        next_element = str_array[i++];
    else
        next_element = first_element;

    while (TRUE)
    {
        const gchar* element;
        const gchar* start;
        const gchar* end;

        if (next_element)
        {
            element = next_element;
            if (str_array)
                next_element = str_array[i++];
            else
                next_element = va_arg(*args, gchar*);
        }
        else
            break;

        /* Ignore empty elements */
        if (!*element)
            continue;

        start = element;

        if (TRUE)
        {
            while (start &&
                (*start == '\\' || *start == '/'))
            {
                current_separator = *start;
                start++;
            }
        }

        end = start + strlen(start);

        if (TRUE)
        {
            while (end >= start + 1 &&
                (end[-1] == '\\' || end[-1] == '/'))
            {
                current_separator = end[-1];
                end--;
            }

            last_trailing = end;
            while (last_trailing >= element + 1 &&
                (last_trailing[-1] == '\\' || last_trailing[-1] == '/'))
                last_trailing--;

            if (!have_leading)
            {
                /* If the leading and trailing separator strings are in the
                 * same element and overlap, the result is exactly that element
                 */
                if (last_trailing <= start)
                    single_element = element;

                g_string_append_len(result, element, (gsize)(start - element));
                have_leading = TRUE;
            }
            else
                single_element = NULL;
        }

        if (end == start)
            continue;

        if (!is_first)
            g_string_append_len(result, &current_separator, 1);

        g_string_append_len(result, start, (gsize)(end - start));
        is_first = FALSE;
    }

    if (single_element)
    {
        g_string_free(result, TRUE);
        return _strdup(single_element);
    }
    else
    {
        if (last_trailing)
            g_string_append(result, last_trailing);

        return g_string_free(result, FALSE);
    }
}

static gchar*
g_build_filename_va(const gchar* first_argument,
    va_list* args,
    gchar** str_array)
{
    gchar* str;

//#ifndef G_OS_WIN32
//    str = g_build_path_va(G_DIR_SEPARATOR_S, first_argument, args, str_array);
//#else
    str = g_build_pathname_va(first_argument, args, str_array);
//#endif

    return str;
}


gchar*
g_build_filename(const gchar* first_element, ...)
{
    gchar* str;
    va_list args;

    va_start(args, first_element);
    str = g_build_filename_va(first_element, &args, NULL);
    va_end(args);

    return str;
}


gchar*
g_get_current_dir(void)
{
#ifdef _WIN32

    gchar* dir = NULL;
    gchar dummy[2];
    DWORD len;

    len = GetCurrentDirectory(2, dummy);
    dir = g_new(char, len);

    if (GetCurrentDirectory(len, dir) == len - 1)

    free(dir);

    if (dir == NULL)
        dir = _strdup("\\");

    return dir;

#else
    const gchar* pwd;
    gchar* buffer = NULL;
    gchar* dir = NULL;
    static gulong max_len = 0;
    struct stat pwdbuf, dotbuf;

    pwd = g_getenv("PWD");
    if (pwd != NULL &&
        g_stat(".", &dotbuf) == 0 && g_stat(pwd, &pwdbuf) == 0 &&
        dotbuf.st_dev == pwdbuf.st_dev && dotbuf.st_ino == pwdbuf.st_ino)
        return g_strdup(pwd);

    if (max_len == 0)
        max_len = (G_PATH_LENGTH == -1) ? 2048 : G_PATH_LENGTH;

    while (max_len < G_MAXULONG / 2)
    {
        g_free(buffer);
        buffer = g_new(gchar, max_len + 1);
        *buffer = 0;
        dir = getcwd(buffer, max_len);

        if (dir || errno != ERANGE)
            break;

        max_len *= 2;
    }

    if (!dir || !*buffer)
    {
        /* hm, should we g_error() out here?
         * this can happen if e.g. "./" has mode \0000
         */
        buffer[0] = G_DIR_SEPARATOR;
        buffer[1] = 0;
    }

    dir = g_strdup(buffer);
    g_free(buffer);

    return dir;

#endif /* !G_OS_WIN32 */
}



GString*
g_string_new(const gchar* init)
{
    GString* string;

    if (init == NULL || *init == '\0')
        string = g_string_sized_new(2);
    else
    {
        gint len;

        len = (gint)strlen(init);
        string = g_string_sized_new(len + 2);

        g_string_append_len(string, init, len);
    }

    return string;
}

GString*
g_string_sized_new(gsize dfl_size)
{
#ifdef GUDANR
    GString* string = g_slice_new(GString);
#else
    GString* string = malloc(sizeof(GString));
#endif

    string->allocated_len = 0;
    string->len = 0;
    string->str = NULL;

    g_string_maybe_expand(string, max(dfl_size, 64));
    string->str[0] = 0;

    return string;
}


gchar*
g_string_free(GString* string, gboolean  free_segment)
{
    gchar* segment;

    g_return_val_if_fail(string != NULL, NULL);

    if (free_segment)
    {
        free(string->str);
        segment = NULL;
    }
    else
        segment = string->str;

#ifdef GUDANR
    g_slice_free(GString, string);
#else
    free(string);
#endif

    return segment;
}


int
g_strcmp0(const char* str1,
    const char* str2)
{
    if (!str1)
        return -(str1 != str2);
    if (!str2)
        return str1 != str2;
    return strcmp(str1, str2);
}

gchar*
g_path_get_dirname(const gchar* file_name)
{
    gchar* base;
    gsize len;

    g_return_val_if_fail(file_name != NULL, NULL);

    base = strrchr(file_name, G_DIR_SEPARATOR);

#ifdef G_OS_WIN32
    {
        gchar* q;
        q = strrchr(file_name, '/');
        if (base == NULL || (q != NULL && q > base))
            base = q;
    }
#endif

    if (!base)
    {
#ifdef G_OS_WIN32
        if (g_ascii_isalpha(file_name[0]) && file_name[1] == ':')
        {
            gchar drive_colon_dot[4];

            drive_colon_dot[0] = file_name[0];
            drive_colon_dot[1] = ':';
            drive_colon_dot[2] = '.';
            drive_colon_dot[3] = '\0';

            return g_strdup(drive_colon_dot);
        }
#endif
        return _strdup(".");
    }

    while (base > file_name && G_IS_DIR_SEPARATOR(*base))
        base--;

#ifdef G_OS_WIN32
    /* base points to the char before the last slash.
     *
     * In case file_name is the root of a drive (X:\) or a child of the
     * root of a drive (X:\foo), include the slash.
     *
     * In case file_name is the root share of an UNC path
     * (\\server\share), add a slash, returning \\server\share\ .
     *
     * In case file_name is a direct child of a share in an UNC path
     * (\\server\share\foo), include the slash after the share name,
     * returning \\server\share\ .
     */
    if (base == file_name + 1 &&
        g_ascii_isalpha(file_name[0]) &&
        file_name[1] == ':')
        base++;
    else if (G_IS_DIR_SEPARATOR(file_name[0]) &&
        G_IS_DIR_SEPARATOR(file_name[1]) &&
        file_name[2] &&
        !G_IS_DIR_SEPARATOR(file_name[2]) &&
        base >= file_name + 2)
    {
        const gchar* p = file_name + 2;
        while (*p && !G_IS_DIR_SEPARATOR(*p))
            p++;
        if (p == base + 1)
        {
            len = (guint)strlen(file_name) + 1;
            base = g_new(gchar, len + 1);
            strcpy(base, file_name);
            base[len - 1] = G_DIR_SEPARATOR;
            base[len] = 0;
            return base;
        }
        if (G_IS_DIR_SEPARATOR(*p))
        {
            p++;
            while (*p && !G_IS_DIR_SEPARATOR(*p))
                p++;
            if (p == base + 1)
                base++;
        }
    }
#endif

    len = (guint)(1 + base - file_name);
    base = g_new(gchar, len + 1);
    memmove(base, file_name, len);
    base[len] = 0;

    return base;
}


#define VALIDATE_BYTE(mask, expect)                      \
  G_STMT_START {                                         \
    if (G_UNLIKELY((*(guchar *)p & (mask)) != (expect))) \
      goto error;                                        \
  } G_STMT_END


static const gchar*
fast_validate_len(const char* str,
    gssize      max_len)

{
    const gchar* p;

    assert(max_len >= 0);

    for (p = str; ((p - str) < max_len) && *p; p++)
    {
        if (*(guchar*)p < 128)
            /* done */;
        else
        {
            const gchar* last;

            last = p;
            if (*(guchar*)p < 0xe0) /* 110xxxxx */
            {
                if (G_UNLIKELY(max_len - (p - str) < 2))
                    goto error;

                if (G_UNLIKELY(*(guchar*)p < 0xc2))
                    goto error;
            }
            else
            {
                if (*(guchar*)p < 0xf0) /* 1110xxxx */
                {
                    if (G_UNLIKELY(max_len - (p - str) < 3))
                        goto error;

                    switch (*(guchar*)p++ & 0x0f)
                    {
                    case 0:
                        VALIDATE_BYTE(0xe0, 0xa0); /* 0xa0 ... 0xbf */
                        break;
                    case 0x0d:
                        VALIDATE_BYTE(0xe0, 0x80); /* 0x80 ... 0x9f */
                        break;
                    default:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                    }
                }
                else if (*(guchar*)p < 0xf5) /* 11110xxx excluding out-of-range */
                {
                    if (G_UNLIKELY(max_len - (p - str) < 4))
                        goto error;

                    switch (*(guchar*)p++ & 0x07)
                    {
                    case 0:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                        if (G_UNLIKELY((*(guchar*)p & 0x30) == 0))
                            goto error;
                        break;
                    case 4:
                        VALIDATE_BYTE(0xf0, 0x80); /* 0x80 ... 0x8f */
                        break;
                    default:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                    }
                    p++;
                    VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                }
                else
                    goto error;
            }

            p++;
            VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */

            continue;

        error:
            return last;
        }
    }

    return p;
}


static const gchar*
fast_validate(const char* str)

{
    const gchar* p;

    for (p = str; *p; p++)
    {
        if (*(guchar*)p < 128)
            /* done */;
        else
        {
            const gchar* last;

            last = p;
            if (*(guchar*)p < 0xe0) /* 110xxxxx */
            {
                if (G_UNLIKELY(*(guchar*)p < 0xc2))
                    goto error;
            }
            else
            {
                if (*(guchar*)p < 0xf0) /* 1110xxxx */
                {
                    switch (*(guchar*)p++ & 0x0f)
                    {
                    case 0:
                        VALIDATE_BYTE(0xe0, 0xa0); /* 0xa0 ... 0xbf */
                        break;
                    case 0x0d:
                        VALIDATE_BYTE(0xe0, 0x80); /* 0x80 ... 0x9f */
                        break;
                    default:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                    }
                }
                else if (*(guchar*)p < 0xf5) /* 11110xxx excluding out-of-range */
                {
                    switch (*(guchar*)p++ & 0x07)
                    {
                    case 0:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                        if (G_UNLIKELY((*(guchar*)p & 0x30) == 0))
                            goto error;
                        break;
                    case 4:
                        VALIDATE_BYTE(0xf0, 0x80); /* 0x80 ... 0x8f */
                        break;
                    default:
                        VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                    }
                    p++;
                    VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */
                }
                else
                    goto error;
            }

            p++;
            VALIDATE_BYTE(0xc0, 0x80); /* 10xxxxxx */

            continue;

        error:
            return last;
        }
    }

    return p;
}


gboolean
g_utf8_validate(const char* str,
    gssize        max_len,
    const gchar** end)

{
    const gchar* p;

    if (max_len >= 0)
        return g_utf8_validate_len(str, max_len, end);

    p = fast_validate(str);

    if (end)
        *end = p;

    if (*p != '\0')
        return FALSE;
    else
        return TRUE;
}

gboolean
g_utf8_validate_len(const char* str,
    gsize         max_len,
    const gchar** end)

{
    const gchar* p;

    p = fast_validate_len(str, max_len);

    if (end)
        *end = p;

    if (p != str + max_len)
        return FALSE;
    else
        return TRUE;
}

gchar*
g_stpcpy(gchar* dest,
    const gchar* src)
{
#ifdef HAVE_STPCPY
    g_return_val_if_fail(dest != NULL, NULL);
    g_return_val_if_fail(src != NULL, NULL);
    return stpcpy(dest, src);
#else
    gchar* d = dest;
    const gchar* s = src;

    g_return_val_if_fail(dest != NULL, NULL);
    g_return_val_if_fail(src != NULL, NULL);
    do
        *d++ = *s;
    while (*s++ != '\0');

    return d - 1;
#endif
}



gchar*
g_strconcat(const gchar* string1, ...)
{
    gsize   l;
    va_list args;
    gchar* s;
    gchar* concat;
    gchar* ptr;

    if (!string1)
        return NULL;

    l = 1 + strlen(string1);
    va_start(args, string1);
    s = va_arg(args, gchar*);
    while (s)
    {
        l += strlen(s);
        s = va_arg(args, gchar*);
    }
    va_end(args);

    concat = g_new(gchar, l);
    ptr = concat;

    ptr = g_stpcpy(ptr, string1);
    va_start(args, string1);
    s = va_arg(args, gchar*);
    while (s)
    {
        ptr = g_stpcpy(ptr, s);
        s = va_arg(args, gchar*);
    }
    va_end(args);

    return concat;
}



gchar*
g_strdup_vprintf(const gchar* format,
    va_list      args)
{
    gchar* string = NULL;

    g_vasprintf(&string, format, args);

    return string;
}


gchar*
g_strdup_printf(const gchar* format,
    ...)
{
    gchar* buffer;
    va_list args;

    va_start(args, format);
    buffer = g_strdup_vprintf(format, args);
    va_end(args);

    return buffer;
}

const gchar*
g_basename(const gchar* file_name)
{
    gchar* base;

    g_return_val_if_fail(file_name != NULL, NULL);

    base = strrchr(file_name, G_DIR_SEPARATOR);

#ifdef G_OS_WIN32
    {
        gchar* q;
        q = strrchr(file_name, '/');
        if (base == NULL || (q != NULL && q > base))
            base = q;
    }
#endif

    if (base)
        return base + 1;

#ifdef G_OS_WIN32
    if (g_ascii_isalpha(file_name[0]) && file_name[1] == ':')
        return (gchar*)file_name + 2;
#endif

    return (gchar*)file_name;
}

gint
g_vsnprintf(gchar* string,
    gulong	  n,
    gchar const* format,
    va_list      args)
{
    g_return_val_if_fail(n == 0 || string != NULL, -1);
    g_return_val_if_fail(format != NULL, -1);

    return _g_vsnprintf(string, n, format, args);
}


gsize
g_printf_string_upper_bound(const gchar* format,
    va_list      args)
{
    gchar c;
    return _g_vsnprintf(&c, 1, format, args) + 1;
}



gint
g_vasprintf(gchar** string,
    gchar const* format,
    va_list      args)
{
    gint len;
    g_return_val_if_fail(string != NULL, -1);

    {
        va_list args2;

        G_VA_COPY(args2, args);

        *string = g_new(gchar, g_printf_string_upper_bound(format, args));

        len = _g_vsprintf(*string, format, args2);
        va_end(args2);

        if (len < 0)
        {
            free(*string);
            *string = NULL;
        }
    }

    return len;
}

#define SIZE_OVERFLOWS(a,b) (G_UNLIKELY ((b) > 0 && (a) > G_MAXSIZE / (b)))

gpointer
g_realloc(gpointer mem,
    gsize    n_bytes)
{
    gpointer newmem;

    if (G_LIKELY(n_bytes))
    {
        newmem = realloc(mem, n_bytes);
#ifdef GUDANR
        TRACE(GLIB_MEM_REALLOC((void*)newmem, (void*)mem, (unsigned int)n_bytes, 0));
#endif
        if (newmem)
            return newmem;
#ifdef GUDANR
        g_error("%s: failed to allocate %"G_GSIZE_FORMAT" bytes",
            G_STRLOC, n_bytes);
#endif
    }

    free(mem);

#ifdef GUDANR
    TRACE(GLIB_MEM_REALLOC((void*)NULL, (void*)mem, 0, 0));
#endif

    return NULL;
}


gpointer
g_realloc_n(gpointer mem,
    gsize    n_blocks,
    gsize    n_block_bytes)
{
    if (SIZE_OVERFLOWS(n_blocks, n_block_bytes))
    {
#ifdef GUDANR
        g_error("%s: overflow allocating %"G_GSIZE_FORMAT"*%"G_GSIZE_FORMAT" bytes",
            G_STRLOC, n_blocks, n_block_bytes);
#endif
    }

    return g_realloc(mem, n_blocks * n_block_bytes);
}

static GIConv
open_converter(const gchar* to_codeset,
    const gchar* from_codeset,
    GError** error)
{
    GIConv cd = 0;

#ifdef GUDANR
    cd = g_iconv_open(to_codeset, from_codeset);

    if (cd == (GIConv)-1)
    {
        /* Something went wrong.  */
        if (error)
        {

            if (errno == EINVAL)
                g_set_error(error, G_CONVERT_ERROR, G_CONVERT_ERROR_NO_CONVERSION,
                    _("Conversion from character set ¡°%s¡± to ¡°%s¡± is not supported"),
                    from_codeset, to_codeset);
            else
                g_set_error(error, G_CONVERT_ERROR, G_CONVERT_ERROR_FAILED,
                    _("Could not open converter from ¡°%s¡± to ¡°%s¡±"),
                    from_codeset, to_codeset);

        }
    }
#endif

    return cd;
}

static int
close_converter(GIConv cd)
{
#ifdef GUDANR
    if (cd == (GIConv)-1)
        return 0;

    return g_iconv_close(cd);
#else
    return 0;
#endif
}



gchar*
g_convert(const gchar* str,    gssize       len,    const gchar* to_codeset,    const gchar* from_codeset,    gsize* bytes_read,    gsize* bytes_written,    GError** error)
{
    gchar* res = NULL;
    GIConv cd = 0;

    g_return_val_if_fail(str != NULL, NULL);
    g_return_val_if_fail(to_codeset != NULL, NULL);
    g_return_val_if_fail(from_codeset != NULL, NULL);

    cd = open_converter(to_codeset, from_codeset, error);

    if (cd == (GIConv)-1)
    {
        if (bytes_read)
            *bytes_read = 0;

        if (bytes_written)
            *bytes_written = 0;

        return NULL;
    }

#ifdef GUDANR
    res = g_convert_with_iconv(str, len, cd,
        bytes_read, bytes_written,
        error);
#endif

    close_converter(cd);

    return res;
}


gchar*
g_path_get_basename(const gchar* file_name)
{
    gssize base;
    gssize last_nonslash;
    gsize len;
    gchar* retval;

    g_return_val_if_fail(file_name != NULL, NULL);

    if (file_name[0] == '\0')
        return _strdup(".");

    last_nonslash = strlen(file_name) - 1;

    while (last_nonslash >= 0 && G_IS_DIR_SEPARATOR(file_name[last_nonslash]))
        last_nonslash--;

    if (last_nonslash == -1)
        /* string only containing slashes */
        return _strdup(G_DIR_SEPARATOR_S);

#ifdef G_OS_WIN32
    if (last_nonslash == 1 &&
        g_ascii_isalpha(file_name[0]) &&
        file_name[1] == ':')
        /* string only containing slashes and a drive */
        return g_strdup(G_DIR_SEPARATOR_S);
#endif
    base = last_nonslash;

    while (base >= 0 && !G_IS_DIR_SEPARATOR(file_name[base]))
        base--;

#ifdef G_OS_WIN32
    if (base == -1 &&
        g_ascii_isalpha(file_name[0]) &&
        file_name[1] == ':')
        base = 1;
#endif /* G_OS_WIN32 */

    len = last_nonslash - base;
    retval = malloc(len + 1);
    memcpy(retval, file_name + (base + 1), len);
    retval[len] = '\0';

    return retval;
}



gchar*
g_strrstr(const gchar* haystack,
    const gchar* needle)
{
    gsize i;
    gsize needle_len;
    gsize haystack_len;
    const gchar* p;

    g_return_val_if_fail(haystack != NULL, NULL);
    g_return_val_if_fail(needle != NULL, NULL);

    needle_len = strlen(needle);
    haystack_len = strlen(haystack);

    if (needle_len == 0)
        return (gchar*)haystack;

    if (haystack_len < needle_len)
        return NULL;

    p = haystack + haystack_len - needle_len;

    while (p >= haystack)
    {
        for (i = 0; i < needle_len; i++)
            if (p[i] != needle[i])
                goto next;

        return (gchar*)p;

    next:
        p--;
    }

    return NULL;
}

#if(0)
static gchar*
g_build_filename_va(const gchar* first_argument,
    va_list* args,
    gchar** str_array)
{
    gchar* str;

#ifndef G_OS_WIN32
    str = g_build_path_va(G_DIR_SEPARATOR_S, first_argument, args, str_array);
#else
    str = g_build_pathname_va(first_argument, args, str_array);
#endif

    return str;
}
#endif

/**
 * g_build_filename_valist:
 * @first_element: (type filename): the first element in the path
 * @args: va_list of remaining elements in path
 *
 * Behaves exactly like g_build_filename(), but takes the path elements
 * as a va_list. This function is mainly meant for language bindings.
 *
 * Returns: (type filename) (transfer full): a newly-allocated string that
 *     must be freed with g_free().
 *
 * Since: 2.56
 */
gchar*
g_build_filename_valist(const gchar* first_element,
    va_list* args)
{
    g_return_val_if_fail(first_element != NULL, NULL);

    return g_build_filename_va(first_element, args, NULL);
}

/**
 * g_build_filenamev:
 * @args: (array zero-terminated=1) (element-type filename): %NULL-terminated
 *     array of strings containing the path elements.
 *
 * Behaves exactly like g_build_filename(), but takes the path elements
 * as a string array, instead of varargs. This function is mainly
 * meant for language bindings.
 *
 * Returns: (type filename) (transfer full): a newly-allocated string that
 *     must be freed with g_free().
 *
 * Since: 2.8
 */
gchar*
g_build_filenamev(gchar** args)
{
    return g_build_filename_va(NULL, NULL, args);
}

/**
 * g_build_filename:
 * @first_element: (type filename): the first element in the path
 * @...: remaining elements in path, terminated by %NULL
 *
 * Creates a filename from a series of elements using the correct
 * separator for filenames.
 *
 * On Unix, this function behaves identically to `g_build_path
 * (G_DIR_SEPARATOR_S, first_element, ....)`.
 *
 * On Windows, it takes into account that either the backslash
 * (`\` or slash (`/`) can be used as separator in filenames, but
 * otherwise behaves as on UNIX. When file pathname separators need
 * to be inserted, the one that last previously occurred in the
 * parameters (reading from left to right) is used.
 *
 * No attempt is made to force the resulting filename to be an absolute
 * path. If the first element is a relative path, the result will
 * be a relative path.
 *
 * Returns: (type filename) (transfer full): a newly-allocated string that
 *     must be freed with g_free().
 **/
#if(0)
gchar*
g_build_filename(const gchar* first_element,
    ...)
{
    gchar* str;
    va_list args;

    va_start(args, first_element);
    str = g_build_filename_va(first_element, &args, NULL);
    va_end(args);

    return str;
}
#endif