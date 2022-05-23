#include "Test.h"
#include <string.h>
#include <stdlib.h>

// 1. Found: 09.11.2021
// AddressSanitizer: global-buffer-overflow
// Changeset 350032
#define MAX_PATH_LEN  256
char translate_file_pattern[256];
void strncpy_failure()
{
    char* src = "Pitz";
    int n = MAX_PATH_LEN + 1;
	strncpy(&translate_file_pattern, src, n);
}

// 2. Found: 02.12.2021
// AddressSanitizer: strcpy-param-overlap
// Changeset 350135
char g_translate_buffer[1024];
char* translate(char* text)
{
    char* translated = text;
    if (strchr(text, '\\') != 0)
    {
        strcpy(g_translate_buffer, "translated Text");
        translated = g_translate_buffer;
    }
    return translated;
}

void strcpy_overlap()
{
    char src[128];
    // ok:
    strcpy(src, "\\a74");
    strcpy(src, translate(src)); 
    // Not ok, copy on itself:
    strcpy(src, "Pitzelpatz");
    strcpy(src, translate(src));

}

// 3. Found 02.12.2021
// SUMMARY: AddressSanitizer: global-buffer-overflow
// Changeset 350136
// not reproducable here, seems to depend on memory layout and positions of global variables im memory

static short gw_stack_counter = 0;
static int gw_stack[10];

int global_buffer_overflow()
{
    return (gw_stack[gw_stack_counter - 1]);
}

// 4. Found: 02.12.2021
// AddressSanitizer: strcpy-param-overlap
// Changeset 350137 (same as 2.)

// 5. Found: 03.12.2021
// AddressSanitizer: stack-buffer-overflow
// Changeset 350141
void ext_write_long(long* plen)
{
    *plen = 6;
}

// 6. Found: 03.12.2021
// Changeset 350145
// Implementation in AddressSanitizerDemoDlg.cpp

// 7. Found in the internet:
void heap_use_after_free()
{
    char* x = (char*)malloc(10 * sizeof(char));
    free(x);
    strcpy(x, "Test");
}


void testAddressSanitizer()
{
    static char* sd = "Test";
    static short len = 25;
    int len2;
    //strncpy_failure();                               // (1) reproducable
    //strcpy_overlap();                                // (2) reproducable
    //int test = global_buffer_overflow();             // (3) not reproducable in small sample project
                                                       // (4) same as (2)
    //ext_write_long(&len);                            // (5) not reproducable in small sample project
    //heap_use_after_free();                           // (7)

}