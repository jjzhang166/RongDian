#ifndef __H_DIBUTIL_20120802
#define __H_DIBUTIL_20120802

#include <Windows.h>

/*  PortTool v2.2     dibapi.h          
 *  ** Win32 Version **
 *  Copyright 1991-1998 Microsoft Corporation. All rights reserved
 *  Header file for Device-Independent Bitmap (DIB) API. 
 */
 
/* Handle to a DIB */
#define HDIB HANDLE

/* Print Area selection */
#define PW_WINDOW        1
#define PW_CLIENT        2

/* Print Options selection */
#define PW_BESTFIT       1
#define PW_STRETCHTOPAGE 2
#define PW_SCALE         3

/* DIB Macros*/
// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed 
// to hold those bits.
#ifndef __AMVIDEO__
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
#endif

/* Error constants */
enum {
      ERR_MIN = 0,                     // All error #s >= this value
      ERR_NOT_DIB = 0,                 // Tried to load a file, NOT a DIB!
      ERR_MEMORY,                      // Not enough memory!
      ERR_READ,                        // Error reading file!
      ERR_LOCK,                        // Error on a GlobalLock()!
      ERR_OPEN,                        // Error opening a file!
      ERR_CREATEPAL,                   // Error creating palette.
      ERR_GETDC,                       // Couldn't get a DC.
      ERR_CREATEDDB,                   // Error create a DDB.
      ERR_STRETCHBLT,                  // StretchBlt() returned failure.
      ERR_STRETCHDIBITS,               // StretchDIBits() returned failure.
      ERR_SETDIBITSTODEVICE,           // SetDIBitsToDevice() failed.
      ERR_STARTDOC,                    // Error calling StartDoc().
      ERR_NOGDIMODULE,                 // Couldn't find GDI module in memory.
      ERR_SETABORTPROC,                // Error calling SetAbortProc().
      ERR_STARTPAGE,                   // Error calling StartPage().
      ERR_NEWFRAME,                    // Error calling NEWFRAME escape.
      ERR_ENDPAGE,                     // Error calling EndPage().
      ERR_ENDDOC,                      // Error calling EndDoc().
      ERR_SETDIBITS,                   // Error calling SetDIBits().
      ERR_FILENOTFOUND,                // Error opening file in GetDib()
      ERR_INVALIDHANDLE,               // Invalid Handle
      ERR_DIBFUNCTION,                 // Error on call to DIB function
      ERR_MAX                          // All error #s < this value
};

/* Function prototypes */
// Creates a DIB from a bitmap
HDIB BitmapToDIB (HBITMAP hBitmap, HPALETTE hPal);

// Changes a bitmap to a specified DIB format
HDIB ChangeBitmapFormat (HBITMAP hBitmap, WORD wBitCount, DWORD dwCompression, HPALETTE hPal);

// Changes a DIB's BPP and/or compression format
HDIB ChangeDIBFormat (HDIB hDIB, WORD wBitCount, DWORD dwCompression);

// Copies entire screen to a standard Bitmap
HBITMAP CopyScreenToBitmap (LPRECT);

// Copies entire screen to a DIB
HDIB CopyScreenToDIB (LPRECT);

// Copies a window to a standard Bitmap
HBITMAP CopyWindowToBitmap (HWND, WORD);

// Copies a window to a DIB
HDIB CopyWindowToDIB (HWND, WORD);

// Creates a palette from a DIB
HPALETTE CreateDIBPalette (HDIB);

// Creates a new DIB
HDIB CreateDIB(DWORD, DWORD, WORD);

// Deletes DIB when finished using it
WORD DestroyDIB (HDIB);

// Displays message box with error message
void DIBError (int ErrNo);

// Gets the DIB height
DWORD DIBHeight (LPSTR lpDIB);

// Calculates number of colors in the DIB's color table
WORD DIBNumColors (LPSTR lpDIB);

// Creates a bitmap from a DIB
HBITMAP DIBToBitmap (HDIB hDIB, HPALETTE hPal);

// Gets the DIB width
DWORD DIBWidth (LPSTR lpDIB);

// Sets pointer to the DIB bits
LPSTR FindDIBBits (LPSTR lpDIB);

// Gets the current palette
HPALETTE GetSystemPalette (void);

// Loads a DIB from a file
HDIB LoadDIB (LPSTR);

// Displays standard bitmap in the specified DC
BOOL PaintBitmap (HDC, LPRECT, HBITMAP, LPRECT, HPALETTE);

// Displays DIB in the specified DC
BOOL PaintDIB (HDC, LPRECT, HDIB, LPRECT, HPALETTE);

// Gets the number of palette entries
int PalEntriesOnDevice (HDC hDC);

// Calculates the buffer size required by a palette
WORD PaletteSize (LPSTR lpDIB);

// Prints the specified DIB
WORD PrintDIB (HDIB, WORD, WORD, WORD, LPSTR);

// Prints the entire screen
WORD PrintScreen (LPRECT, WORD, WORD, WORD, LPSTR);

// Prints all or part of a window
WORD PrintWindow (HWND, WORD, WORD, WORD, WORD, LPSTR);

// Saves the specified dib in a file
WORD SaveDIB (HDIB, LPSTR);



/*  PortTool v2.2     dibutil.h
 *  dibutil.h
 *
 *  Copyright 1991-1998 Microsoft Corporation. All rights reserved.
 *  Header file for Device-Independent Bitmap (DIB) API.
 */

/* DIB constants */
#define PALVERSION   0x300

/* DIB macros */
#define IS_WIN30_DIB(lpbi)  ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))
#define RECTWIDTH(lpRect)     ((lpRect)->right - (lpRect)->left)
#define RECTHEIGHT(lpRect)    ((lpRect)->bottom - (lpRect)->top)

// Allocates memory for a DIB
HANDLE AllocRoomForDIB(BITMAPINFOHEADER bi, HBITMAP hBitmap);

#endif // __H_DIBUTIL_20120802
