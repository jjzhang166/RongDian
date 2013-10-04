#pragma once

#define USE_GIFLIB_DLL

#include <Shlwapi.h>
#include <GifLib/lib/gif_lib_private.h>
#pragma comment(lib, "Shlwapi.lib")

#ifdef _DEBUG
#define GIF_DLL L"GifLibd.dll"
#else
#define GIF_DLL L"GifLib.dll"
#endif

#define GIF_TRANSPARENT		0x01
#define GIF_USER_INPUT		0x02
#define GIF_DISPOSE_MASK	0x07
#define GIF_DISPOSE_SHIFT	2

#define GIF_NOT_TRANSPARENT	-1

#define GIF_DISPOSE_NONE	0		// No disposal specified. The decoder is
// not required to take any action.
#define GIF_DISPOSE_LEAVE	1		// Do not dispose. The graphic is to be left
// in place.
#define GIF_DISPOSE_BACKGND	2		// Restore to background color. The area used by the
// graphic must be restored to the background color.

#define GIF_DISPOSE_RESTORE	3		// Restore to previous. The decoder is required to
// restore the area overwritten by the graphic with
// what was there prior to rendering the graphic.

#define DWORD_PAD(x)		(((x) + 3) & ~3)

const COLORREF kTransColor = RGB(255, 255, 255);
// Netscape 2.0 looping extension block
const GifByteType kNetscape20ext[] = "\x0bNETSCAPE2.0";
#define NSEXT_LOOP      0x01        // Loop Count field code

//
//  Appendix E. Interlaced Images.
//
//  The rows of an Interlaced images are arranged in the following order:
//  
//        Group 1 : Every 8th. row, starting with row 0.              (Pass 1)
//        Group 2 : Every 8th. row, starting with row 4.              (Pass 2)
//        Group 3 : Every 4th. row, starting with row 2.              (Pass 3)
//        Group 4 : Every 2nd. row, starting with row 1.              (Pass 4)
//  
const int InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
const int InterlacedJumps[] = { 8, 8, 4, 2 };    /* be read - offsets and jumps... */
//
//  The Following example illustrates how the rows of an interlaced image are
//  ordered.
//  
//        Row Number                                        Interlace Pass
//  
//   0    -----------------------------------------       1
//   1    -----------------------------------------                         4
//   2    -----------------------------------------                   3
//   3    -----------------------------------------                         4
//   4    -----------------------------------------             2
//   5    -----------------------------------------                         4
//   6    -----------------------------------------                   3
//   7    -----------------------------------------                         4
//   8    -----------------------------------------       1
//   9    -----------------------------------------                         4
//   10   -----------------------------------------                   3
//   11   -----------------------------------------                         4
//   12   -----------------------------------------             2
//   13   -----------------------------------------                         4
//   14   -----------------------------------------                   3
//   15   -----------------------------------------                         4
//   16   -----------------------------------------       1
//   17   -----------------------------------------                         4
//   18   -----------------------------------------                   3
//   19   -----------------------------------------                         4
//

// Encode
typedef GifFileType* (* LPFNEGifOpenFileName)(const char *GifFileName, int GifTestExistance);
typedef GifFileType* (* LPFNEGifOpenFileHandle)(int GifFileHandle);
typedef GifFileType* (* LPFNEGifOpen)(void *userPtr, OutputFunc writeFunc);

typedef int (* LPFNEGifSpew)(GifFileType * GifFile);
typedef void (* LPFNEGifSetGifVersion)(const char *Version);
typedef int (* LPFNEGifPutScreenDesc)(GifFileType * GifFile, int GifWidth, int GifHeight, int GifColorRes, int GifBackGround, const ColorMapObject * GifColorMap);
typedef int (* LPFNEGifPutImageDesc)(GifFileType * GifFile, int GifLeft, int GifTop, int Width, int GifHeight, int GifInterlace, const ColorMapObject * GifColorMap);
typedef int (* LPFNEGifPutLine)(GifFileType * GifFile, GifPixelType * GifLine, int GifLineLen);
typedef int (* LPFNEGifPutPixel)(GifFileType * GifFile, GifPixelType GifPixel);
typedef int (* LPFNEGifPutComment)(GifFileType * GifFile, const char *GifComment);
typedef int (* LPFNEGifPutExtensionFirst)(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension);
typedef int (* LPFNEGifPutExtensionNext)(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension);
typedef int (* LPFNEGifPutExtensionLast)(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension);
typedef int (* LPFNEGifPutExtension)(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension);
typedef int (* LPFNEGifPutCode)(GifFileType * GifFile, int GifCodeSize, const GifByteType * GifCodeBlock);
typedef int (* LPFNEGifPutCodeNext)(GifFileType * GifFile, const GifByteType * GifCodeBlock);
typedef int (* LPFNEGifCloseFile)(GifFileType * GifFile);

// Decode
typedef GifFileType* (* LPFNDGifOpenFileName)(const char *GifFileName);
typedef GifFileType* (* LPFNDGifOpenFileHandle)(int GifFileHandle);
typedef int (* LPFNDGifSlurp)(GifFileType * GifFile);

typedef GifFileType* (* LPFNDGifOpen)(void *userPtr, InputFunc readFunc);
typedef int (* LPFNDGifGetScreenDesc)(GifFileType * GifFile);
typedef int (* LPFNDGifGetRecordType)(GifFileType * GifFile, GifRecordType * GifType);
typedef int (* LPFNDGifGetImageDesc)(GifFileType * GifFile);
typedef int (* LPFNDGifGetLine)(GifFileType * GifFile, GifPixelType * GifLine, int GifLineLen);
typedef int (* LPFNDGifGetPixel)(GifFileType * GifFile, GifPixelType GifPixel);
typedef int (* LPFNDGifGetComment)(GifFileType * GifFile, char *GifComment);
typedef int (* LPFNDGifGetExtension)(GifFileType * GifFile, int *GifExtCode, GifByteType ** GifExtension);
typedef int (* LPFNDGifGetExtensionNext)(GifFileType * GifFile, GifByteType ** GifExtension);
typedef int (* LPFNDGifGetCode)(GifFileType * GifFile, int *GifCodeSize, GifByteType ** GifCodeBlock);
typedef int (* LPFNDGifGetCodeNext)(GifFileType * GifFile, GifByteType ** GifCodeBlock);
typedef int (* LPFNDGifGetLZCodes)(GifFileType * GifFile, int *GifCode);
typedef int (* LPFNDGifCloseFile)(GifFileType * GifFile);

typedef enum _tagGIFLIB_STATUS
{
	GIFLIB_INVALID = -1,
	GIFLIB_OK = 0
} GIFLIB_STATUS;

typedef enum _tagGIFLIB_MODE
{
	GIFLIB_NONE = -1,
	GIFLIB_ENCODER,
	GIFLIB_DECODER,
	GIFLIB_ALL
} GIFLIB_MODE;

typedef struct _tagBITMAP256
{
	BITMAPINFO bmi;
	RGBQUAD quad[255];
} BITMAP256, *PBITMAP256, *LPBITMAP256;

#define GET_PROC_ADDR(hModule, szFn, lpFn, lpAddr) \
	assert(hModule!=NULL); \
	lpAddr = (lpFn)GetProcAddress(hModule, szFn);


class CGifEncoder
{
public:
	CGifEncoder()
	{
		// Encode
		lpfnEGifOpenFileName = NULL;
		lpfnEGifOpenFileHandle = NULL;
		lpfnEGifOpen = NULL;
		lpfnEGifSpew = NULL;
		lpfnEGifSetGifVersion = NULL;
		lpfnEGifPutScreenDesc = NULL;
		lpfnEGifPutImageDesc = NULL;
		lpfnEGifPutLine = NULL;
		lpfnEGifPutPixel = NULL;
		lpfnEGifPutComment = NULL;
		lpfnEGifPutExtensionFirst = NULL;
		lpfnEGifPutExtensionNext = NULL;
		lpfnEGifPutExtensionLast = NULL;
		lpfnEGifPutExtension = NULL;
		lpfnEGifPutCode = NULL;
		lpfnEGifPutCodeNext = NULL;
	}
	~CGifEncoder()
	{

	}
	GIFLIB_STATUS Init(HMODULE hModule)
	{
		if(!hModule)
			return GIFLIB_INVALID;
		// Encode
		GET_PROC_ADDR(hModule, "EGifOpenFileName", LPFNEGifOpenFileName, lpfnEGifOpenFileName);
		GET_PROC_ADDR(hModule, "EGifOpenFileHandle", LPFNEGifOpenFileHandle, lpfnEGifOpenFileHandle);
		GET_PROC_ADDR(hModule, "EGifOpen", LPFNEGifOpen, lpfnEGifOpen);

		GET_PROC_ADDR(hModule, "EGifSpew", LPFNEGifSpew, lpfnEGifSpew);
		GET_PROC_ADDR(hModule, "EGifSetGifVersion", LPFNEGifSetGifVersion, lpfnEGifSetGifVersion);
		GET_PROC_ADDR(hModule, "EGifPutScreenDesc", LPFNEGifPutScreenDesc, lpfnEGifPutScreenDesc);
		GET_PROC_ADDR(hModule, "EGifPutImageDesc", LPFNEGifPutImageDesc, lpfnEGifPutImageDesc);
		GET_PROC_ADDR(hModule, "EGifPutLine", LPFNEGifPutLine, lpfnEGifPutLine);
		GET_PROC_ADDR(hModule, "EGifPutPixel", LPFNEGifPutPixel, lpfnEGifPutPixel);
		GET_PROC_ADDR(hModule, "EGifPutComment", LPFNEGifPutComment, lpfnEGifPutComment);
		GET_PROC_ADDR(hModule, "EGifPutExtensionFirst", LPFNEGifPutExtensionFirst, lpfnEGifPutExtensionFirst);
		GET_PROC_ADDR(hModule, "EGifPutExtensionNext", LPFNEGifPutExtensionNext, lpfnEGifPutExtensionNext);
		GET_PROC_ADDR(hModule, "EGifPutExtensionLast", LPFNEGifPutExtensionLast, lpfnEGifPutExtensionLast);
		GET_PROC_ADDR(hModule, "EGifPutExtension", LPFNEGifPutExtension, lpfnEGifPutExtension);
		GET_PROC_ADDR(hModule, "EGifPutCode", LPFNEGifPutCode, lpfnEGifPutCode);
		GET_PROC_ADDR(hModule, "EGifPutCodeNext", LPFNEGifPutCodeNext, lpfnEGifPutCodeNext);
		GET_PROC_ADDR(hModule, "EGifCloseFile", LPFNEGifCloseFile, lpfnEGifCloseFile);
		if(lpfnEGifOpenFileName && lpfnEGifOpenFileHandle && lpfnEGifOpen 
			&& lpfnEGifSpew && lpfnEGifSetGifVersion && lpfnEGifPutScreenDesc
			&& lpfnEGifPutImageDesc && lpfnEGifPutLine && lpfnEGifPutPixel 
			&& lpfnEGifPutComment && lpfnEGifPutExtensionFirst && lpfnEGifPutExtensionNext 
			&& lpfnEGifPutExtensionLast && lpfnEGifPutExtension && lpfnEGifPutCode 
			&& lpfnEGifPutCodeNext && lpfnEGifCloseFile)
			return GIFLIB_OK;
		return GIFLIB_INVALID;
	}

public:
	// Encode
	LPFNEGifOpenFileName lpfnEGifOpenFileName;
	LPFNEGifOpenFileHandle lpfnEGifOpenFileHandle;
	LPFNEGifOpen lpfnEGifOpen;

	LPFNEGifSpew lpfnEGifSpew;
	LPFNEGifSetGifVersion lpfnEGifSetGifVersion;
	LPFNEGifPutScreenDesc lpfnEGifPutScreenDesc;
	LPFNEGifPutImageDesc lpfnEGifPutImageDesc;
	LPFNEGifPutLine lpfnEGifPutLine;
	LPFNEGifPutPixel lpfnEGifPutPixel;
	LPFNEGifPutComment lpfnEGifPutComment;
	LPFNEGifPutExtensionFirst lpfnEGifPutExtensionFirst;
	LPFNEGifPutExtensionNext lpfnEGifPutExtensionNext;
	LPFNEGifPutExtensionLast lpfnEGifPutExtensionLast;
	LPFNEGifPutExtension lpfnEGifPutExtension;
	LPFNEGifPutCode lpfnEGifPutCode;
	LPFNEGifPutCodeNext lpfnEGifPutCodeNext;
	LPFNEGifCloseFile lpfnEGifCloseFile;
};

class CGifDecoder
{
public:
	CGifDecoder()
	{
		// Decode
		lpfnDGifOpen = NULL;
		lpfnDGifGetScreenDesc = NULL;
		lpfnDGifGetRecordType = NULL;
		lpfnDGifGetImageDesc = NULL;
		lpfnDGifGetLine = NULL;
		lpfnDGifGetPixel = NULL;
		lpfnDGifGetComment = NULL;
		lpfnDGifGetExtension = NULL;
		lpfnDGifGetExtensionNext = NULL;
		lpfnDGifGetCode = NULL;
		lpfnDGifGetCodeNext = NULL;
		lpfnDGifGetLZCodes = NULL;
		lpfnDGifCloseFile = NULL;
		
		lpfnDGifOpenFileName = NULL;
		lpfnDGifOpenFileHandle = NULL;
		lpfnDGifSlurp = NULL;
	}
	~CGifDecoder()
	{

	}
	GIFLIB_STATUS Init(HMODULE hModule)
	{
		if(!hModule)
			return GIFLIB_INVALID;
		// Decode
		GET_PROC_ADDR(hModule, "DGifOpenFileName", LPFNDGifOpenFileName, lpfnDGifOpenFileName);
		GET_PROC_ADDR(hModule, "DGifOpenFileHandle", LPFNDGifOpenFileHandle, lpfnDGifOpenFileHandle);
		GET_PROC_ADDR(hModule, "DGifSlurp", LPFNDGifSlurp, lpfnDGifSlurp);

		GET_PROC_ADDR(hModule, "DGifOpen", LPFNDGifOpen, lpfnDGifOpen);
		GET_PROC_ADDR(hModule, "DGifGetScreenDesc", LPFNDGifGetScreenDesc, lpfnDGifGetScreenDesc);
		GET_PROC_ADDR(hModule, "DGifGetRecordType", LPFNDGifGetRecordType, lpfnDGifGetRecordType);
		GET_PROC_ADDR(hModule, "DGifGetImageDesc", LPFNDGifGetImageDesc, lpfnDGifGetImageDesc);
		GET_PROC_ADDR(hModule, "DGifGetLine", LPFNDGifGetLine, lpfnDGifGetLine);
		GET_PROC_ADDR(hModule, "DGifGetPixel", LPFNDGifGetPixel, lpfnDGifGetPixel);
		//GET_PROC_ADDR(hModule, "DGifGetComment", LPFNDGifGetComment, lpfnDGifGetComment);
		GET_PROC_ADDR(hModule, "DGifGetExtension", LPFNDGifGetExtension, lpfnDGifGetExtension);
		GET_PROC_ADDR(hModule, "DGifGetExtensionNext", LPFNDGifGetExtensionNext, lpfnDGifGetExtensionNext);
		GET_PROC_ADDR(hModule, "DGifGetCode", LPFNDGifGetCode, lpfnDGifGetCode);
		GET_PROC_ADDR(hModule, "DGifGetCodeNext", LPFNDGifGetCodeNext, lpfnDGifGetCodeNext);
		GET_PROC_ADDR(hModule, "DGifGetLZCodes", LPFNDGifGetLZCodes, lpfnDGifGetLZCodes);
		GET_PROC_ADDR(hModule, "DGifSlurp", LPFNDGifSlurp, lpfnDGifCloseFile);
		if(lpfnDGifOpenFileName && lpfnDGifOpenFileHandle && lpfnDGifSlurp 
			&& lpfnDGifOpen && lpfnDGifGetScreenDesc && lpfnDGifGetRecordType 
			&& lpfnDGifGetImageDesc && lpfnDGifGetLine && lpfnDGifGetPixel 
			/*&& lpfnDGifGetComment*/ && lpfnDGifGetExtension && lpfnDGifGetExtensionNext 
			&& lpfnDGifGetCode && lpfnDGifGetCodeNext && lpfnDGifGetLZCodes 
			&& lpfnDGifCloseFile)
			return GIFLIB_OK;
		return GIFLIB_INVALID;
	}

	COLORREF ColorDiff(COLORREF rgb1, COLORREF rgb2)
	{
		int r = GetRValue(rgb1) - GetRValue(rgb2);
		int g = GetGValue(rgb1) - GetGValue(rgb2);
		int b = GetBValue(rgb1) - GetBValue(rgb2);
		long lDiff = r*r + g*g + b*b;
		return lDiff;
	}

	COLORREF AvePixel(LPBYTE lpSrcRow, DWORD dwSrcRow, LPCOLORMAP lpColorMap, UINT uColors, int nScale)
	{
		int nPower = nScale * nScale;
		int nPower2 = nPower / 2;
		int nRed = nPower2;
		int nGreen = nPower2;
		int nBlue = nPower2;
		for(int row=nScale; row>0; --row)
		{
			LPBYTE lpSrc = lpSrcRow;
			for(int col=nScale; col>0; --col)
			{
				COLORREF rgb = RGB(lpSrc[2], lpSrc[1], lpSrc[0]);
				lpSrc += 3;
				// Map Color Based on lpColorMap, uColors
				long lClosest = 5;
				for(UINT u=0; u<uColors; ++u)
				{
					long lDiff = ColorDiff(lpColorMap[u].from, rgb);
					if(lDiff<lClosest)
					{
						lClosest = lDiff;
						rgb = lpColorMap[u].to;
					}
				}
				if(rgb & 0xff000000)
					return rgb;
				nRed += GetRValue(rgb);
				nGreen += GetGValue(rgb);
				nBlue += GetBValue(rgb);
			}
			lpSrcRow += dwSrcRow;
		}
		// Return "average" pixel
		return RGB(nRed/nPower, nGreen/nPower, nBlue/nPower);
	}

	void CopyBitmap24(LPBYTE lpDstRow, LPBYTE lpSrcRow, int nWidth, int nHeight, LPCOLORMAP lpColorMap, UINT uColors, int nScale)
	{
		assert(nScale>0);
		DWORD dwSrcRowBytes = DWORD_PAD(nWidth * 3);
		DWORD dwDstRowBytes = DWORD_PAD(nWidth / nScale * 3);
		for(int row=0; row<nHeight; row+=nScale)
		{
			LPBYTE lpDst = lpDstRow;
			LPBYTE lpSrc = lpSrcRow + (row * dwSrcRowBytes);
			for(int col=0; col<nWidth; col+=nScale)
			{
				COLORREF rgb = AvePixel(lpSrc, dwSrcRowBytes, lpColorMap, uColors, nScale);
				*lpDst++ = GetBValue(rgb);
				*lpDst++ = GetGValue(rgb);
				*lpDst++ = GetRValue(rgb);
				lpSrc += (nScale * 3);
			}
			lpDstRow += dwDstRowBytes;
		}
	}

	HBITMAP CreateMappedBitmap(LPBYTE lpData, LPBITMAP256 lpBit256, LPCOLORMAP lpColorMap, int nWidth, int nHeight, UINT uCount, int nScale)
	{
		HBITMAP hBitmap = NULL;
		assert(lpData!=NULL);
		assert(lpBit256!=NULL);
		HDC hDC = CreateCompatibleDC(NULL);
		if(!hDC)
			return NULL;
		LPVOID lpBits = NULL;
		BITMAP256 bmiMapped = *lpBit256;
		if(nScale>0)
		{
			bmiMapped.bmi.bmiHeader.biWidth /= nScale;
			bmiMapped.bmi.bmiHeader.biHeight /= nScale;
		}
		hBitmap = CreateDIBSection(hDC, &bmiMapped.bmi, DIB_RGB_COLORS, &lpBits, NULL, 0L);
		if(hBitmap && lpBits)
		{
			assert(lpBit256->bmi.bmiHeader.biBitCount==24);
			CopyBitmap24((LPBYTE)lpBits, lpData, nWidth, nHeight, lpColorMap, uCount, nScale);
		}
		DeleteDC(hDC);
		return hBitmap;
	}

	void InitBitmapInfo(LPBITMAPINFO lpBmi, DWORD dwWidth, DWORD dwHeight)
	{
		::ZeroMemory(lpBmi, sizeof(BITMAPINFOHEADER));
		lpBmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpBmi->bmiHeader.biWidth = dwWidth;
		lpBmi->bmiHeader.biHeight = -((int)dwHeight);     // negative for top-down bitmap
		lpBmi->bmiHeader.biPlanes = 1;
		lpBmi->bmiHeader.biBitCount = 24;
		lpBmi->bmiHeader.biClrUsed = 256;
	};

	void CopyColorMap(ColorMapObject* pColorMap, LPBITMAPINFO liBmi)
	{
		assert(pColorMap!=NULL);
		assert(liBmi!=NULL);
		int nLen = pColorMap->ColorCount;
		assert(nLen<=256);
		int nCount = min(nLen, 256);
		BYTE red = 0, green = 0, blue = 0;
		for(int i=0; i<nCount; i++)
		{
			liBmi->bmiColors[i].rgbRed = pColorMap->Colors[i].Red;
			liBmi->bmiColors[i].rgbGreen = pColorMap->Colors[i].Green;
			liBmi->bmiColors[i].rgbBlue = pColorMap->Colors[i].Blue;
		}
	}

	void CopyGif(LPBYTE pDst, LPBYTE pSrc, int nWidth, int nTrans, GifColorType* pColorType)
	{
		assert(pColorType!=NULL);
		assert(nWidth>0);
		BYTE lpByte;
		GifColorType* pColor = NULL;
		do 
		{
			lpByte = *pSrc++;
			if(lpByte!=nTrans)
			{
				pColor = pColorType + lpByte;
				pDst[0] = pColor->Blue;
				pDst[1] = pColor->Green;
				pDst[2] = pColor->Red;
			}
			pDst += 3;
		} while (--nWidth);
	}

	void FillGif(LPBYTE pDst, DWORD dwWidth, COLORREF colColor)
	{
		if (!dwWidth)
			return;
		do
		{
			pDst[0] = GetBValue(colColor);
			pDst[1] = GetGValue(colColor);
			pDst[2] = GetRValue(colColor);
			pDst += 3;
		} while (--dwWidth);
	}

	BOOL LoadGif(LPCWSTR lpszPath, GifFileType **ppGifFile, LPBYTE *ppData)
	{
		char szPath_a[2048] = {0};
		WideCharToMultiByte (CP_OEMCP, NULL, lpszPath, -1, szPath_a, _countof(szPath_a), NULL, FALSE);
		COLORREF colTrans = RGB(255, 255, 255);
		*ppGifFile = lpfnDGifOpenFileName(szPath_a);
		if(!*ppGifFile)
			return FALSE;
		DWORD dwWidth = (*ppGifFile)->SWidth;
		DWORD dwHeight = (*ppGifFile)->SHeight;
		DWORD dwRowBytes = ((((dwWidth * 3)) + 3) & ~3);
		DWORD dwScreen = dwRowBytes * dwHeight;
		*ppData = (LPBYTE)malloc(dwScreen * 2 + dwRowBytes);
		if(!(*ppData))
			return FALSE;
		for(int i=0; i<(*ppGifFile)->SHeight*2; ++i)
			FillGif(*ppData+i*dwRowBytes, (*ppGifFile)->SWidth, colTrans);
		return TRUE;
	}

	COLORREF GetBackgnd(GifFileType *pGifFile)
	{
		static COLORREF colBackgnd = 0;
		if(pGifFile->SColorMap && colBackgnd==0)
		{
			GifColorType* pColor = pGifFile->SColorMap->Colors + pGifFile->SBackGroundColor;
			colBackgnd = RGB(pColor->Red, pColor->Green, pColor->Blue);
		}
		return colBackgnd;
	}

	BOOL GetNext(GifFileType *pGifFile, LPBYTE lpData, LPBITMAP256 lpBit256, int *pDelay, int *pTrans)
	{
		if(!pGifFile || !lpData || !lpBit256)
			return FALSE;
		DWORD dwWidth = pGifFile->SWidth;
		DWORD dwHeight = pGifFile->SHeight;
		DWORD dwRowBytes = DWORD_PAD(dwWidth * 3);
		DWORD dwScreen = dwRowBytes * dwHeight;
		LPBYTE lpBit1 = lpData;
		LPBYTE lpBit2 = lpBit1 + dwScreen;
		GifPixelType *pPixelType = lpBit2 + dwScreen;
		GifRecordType stRecordType;
		GifByteType *pByteType = NULL;
		int nDelay = 10;
		int nDispose = 0;
		int nTrans = GIF_NOT_TRANSPARENT;
		//BITMAP256 bmiGlobal;
#define XYOFFSET(x, y)	((y) * dwRowBytes + (x) * 3)
		const RGBQUAD rgbWhite = { 255, 255, 255, 0 };
		const RGBQUAD rgbBlack = { 0, 0, 0, 0 };
		do 
		{
			int nExtCode;
			if(lpfnDGifGetRecordType(pGifFile, &stRecordType)==GIF_ERROR)
				return FALSE;
			switch(stRecordType)
			{
			case IMAGE_DESC_RECORD_TYPE:
				{
					if(lpfnDGifGetImageDesc(pGifFile)==GIF_ERROR)
						break;
					int nLeft = pGifFile->Image.Left;
					int nTop = pGifFile->Image.Top;

					GifColorType *pColorTable = NULL;
					lpBit256->bmi.bmiColors[0] = rgbBlack;
					for (int i = 1; i < 256; ++i)
						lpBit256->bmi.bmiColors[i] = rgbWhite;
					InitBitmapInfo(&lpBit256->bmi, dwWidth, dwHeight);
					if(pGifFile->Image.ColorMap==NULL)
					{
						pColorTable = pGifFile->SColorMap->Colors;
					}
					else
					{
						CopyColorMap(pGifFile->Image.ColorMap, &lpBit256->bmi);
						pColorTable = pGifFile->Image.ColorMap->Colors;
					}
					// Always copy next -> current image
					memcpy(lpBit1, lpBit2, dwScreen);

					int nWidth = pGifFile->Image.Width;
					int nHeight = pGifFile->Image.Height;
					if(pGifFile->Image.Interlace)
					{
						// Need to perform 4 passes on the images:
						for(int pass=0; pass<4; pass++)
						{
							for(int i=InterlacedOffset[pass]; i<nHeight; i+=InterlacedJumps[pass])
							{
								if(lpfnDGifGetLine(pGifFile, pPixelType, nWidth)==GIF_ERROR)
									return FALSE;
								CopyGif(lpBit1+XYOFFSET(nLeft, nTop+i), pPixelType, nWidth, nTrans, pColorTable);
							}
						}
					}
					else
					{
						// Non-interlaced image
						for(int i=0; i<nHeight; i++)
						{
							if(lpfnDGifGetLine(pGifFile, pPixelType, nWidth)==GIF_ERROR)
								return FALSE;
							CopyGif(lpBit1+XYOFFSET(nLeft, nTop+i), pPixelType, nWidth, nTrans, pColorTable);
						}
					}
					// Prepare second image with next starting
					if(nDispose==GIF_DISPOSE_BACKGND)
					{
						COLORREF colFill = (nTrans == GIF_NOT_TRANSPARENT) ? GetBackgnd(pGifFile) : kTransColor;
						for(int i=0; i<nHeight; i++)
							FillGif(lpBit2 + XYOFFSET(nLeft, nTop+i), colFill, nWidth);
					}
					else if(nDispose!=GIF_DISPOSE_RESTORE)
					{
						memcpy(lpBit2, lpBit1, dwScreen);
					}
					nDispose = 0;
					if(nDelay)
					{
						*pDelay = nDelay * 10;
						return TRUE;
					}
				}
				break;
			case EXTENSION_RECORD_TYPE:
				{
					if(lpfnDGifGetExtension(pGifFile, &nExtCode, &pByteType)==GIF_ERROR)
						return FALSE;
					BOOL bNetscapeExt = FALSE;
					switch(nExtCode)
					{
					case COMMENT_EXT_FUNC_CODE:
						break;
					case GRAPHICS_EXT_FUNC_CODE:
						{
							int nFlag = pByteType[1];
							nDelay = MAKEWORD(pByteType[2], pByteType[3]);
							nTrans = (nFlag & GIF_TRANSPARENT) ? pByteType[4] : GIF_NOT_TRANSPARENT;
							*pTrans = nTrans;
							nDispose = (nFlag >> GIF_DISPOSE_SHIFT) & GIF_DISPOSE_MASK;
						}
						break;
					case PLAINTEXT_EXT_FUNC_CODE:
						break;
					case APPLICATION_EXT_FUNC_CODE:
						{
							if(memcmp(pByteType, kNetscape20ext, kNetscape20ext[0])==0)
								bNetscapeExt = TRUE;
						}
						break;
					default:
						break;
					}
					do 
					{
						if(lpfnDGifGetExtensionNext(pGifFile, &pByteType)==GIF_ERROR)
							return FALSE;
					} while (pByteType);
				}
				break;
			case TERMINATE_RECORD_TYPE:
			default:
				break;
			}
		} while (stRecordType!=TERMINATE_RECORD_TYPE);
		return TRUE;
	}

public:
	// Decode
	LPFNDGifOpenFileName lpfnDGifOpenFileName;
	LPFNDGifOpenFileHandle lpfnDGifOpenFileHandle;
	LPFNDGifSlurp lpfnDGifSlurp;

	LPFNDGifOpen lpfnDGifOpen;
	LPFNDGifGetScreenDesc lpfnDGifGetScreenDesc;
	LPFNDGifGetRecordType lpfnDGifGetRecordType;
	LPFNDGifGetImageDesc lpfnDGifGetImageDesc;
	LPFNDGifGetLine lpfnDGifGetLine;
	LPFNDGifGetPixel lpfnDGifGetPixel;
	LPFNDGifGetComment lpfnDGifGetComment;
	LPFNDGifGetExtension lpfnDGifGetExtension;
	LPFNDGifGetExtensionNext lpfnDGifGetExtensionNext;
	LPFNDGifGetCode lpfnDGifGetCode;
	LPFNDGifGetCodeNext lpfnDGifGetCodeNext;
	LPFNDGifGetLZCodes lpfnDGifGetLZCodes;
	LPFNDGifCloseFile lpfnDGifCloseFile;
};

class CGifLib
{
public:
	CGifLib(int nEncoder = GIFLIB_DECODER)
	{
		wchar_t szModule[1024] = {0};
		GetModuleFileName(NULL, szModule, sizeof(szModule));
		PathRemoveFileSpec(szModule);
		if(PathIsRoot(szModule))
		{
			szModule[wcslen(szModule)-1] = '\0';
		}
		PathAppend(szModule, GIF_DLL);
		hModule = LoadLibraryW(szModule);
		nInit = GIFLIB_INVALID;
		if(hModule)
		{
			switch(nEncoder)
			{
			case GIFLIB_ENCODER:
				nInit = gifEncoder.Init(hModule);
				break;
			case GIFLIB_DECODER:
				nInit = gifDecoder.Init(hModule);
				break;
			case GIFLIB_ALL:
				if(gifEncoder.Init(hModule)==0 && gifDecoder.Init(hModule)==0)
					nInit = GIFLIB_OK;
				break;
			case GIFLIB_NONE:
				// do nothing, return not init
			default:
				break;
			}
		}
	}
	~CGifLib()
	{
		if(hModule)
			FreeLibrary(hModule);
	}

public:
	// Common
	int IsInit() { return nInit; };
	CGifEncoder* GetEncoder() { return &gifEncoder; };
	CGifDecoder* GetDecoder() { return &gifDecoder; };
	// Encode
	GifFileType *EGifOpenFileName(const char *GifFileName, int GifTestExistance)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifOpenFileName(GifFileName, GifTestExistance);
		return NULL;
	}
	GifFileType *EGifOpenFileHandle(int GifFileHandle)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifOpenFileHandle(GifFileHandle);
		return NULL;
	}
	GifFileType *EGifOpen(void *userPtr, OutputFunc writeFunc)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifOpen(userPtr, writeFunc);
		return NULL;
	}
	int EGifSpew(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifSpew(GifFile);
		return GIF_ERROR;
	}
	void EGifSetGifVersion(const char *Version)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifSetGifVersion(Version);
	}
	int EGifPutScreenDesc(GifFileType * GifFile, int GifWidth, int GifHeight, int GifColorRes, int GifBackGround, const ColorMapObject * GifColorMap)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutScreenDesc(GifFile, GifWidth, GifHeight, GifColorRes, GifBackGround, GifColorMap);
		return GIF_ERROR;
	}
	int EGifPutImageDesc(GifFileType * GifFile, int GifLeft, int GifTop, int Width, int GifHeight, int GifInterlace, const ColorMapObject * GifColorMap)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutImageDesc(GifFile, GifLeft, GifTop, Width, GifHeight, GifInterlace, GifColorMap);
		return GIF_ERROR;
	}
	int EGifPutLine(GifFileType * GifFile, GifPixelType * GifLine, int GifLineLen)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutLine(GifFile, GifLine, GifLineLen);
		return GIF_ERROR;
	}
	int EGifPutPixel(GifFileType * GifFile, GifPixelType GifPixel)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutPixel(GifFile, GifPixel);
		return GIF_ERROR;
	}
	int EGifPutComment(GifFileType * GifFile, const char *GifComment)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutComment(GifFile, GifComment);
		return GIF_ERROR;
	}
	int EGifPutExtensionFirst(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutExtensionFirst(GifFile, GifExtCode, GifExtLen, GifExtension);
		return GIF_ERROR;
	}
	int EGifPutExtensionNext(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutExtensionNext(GifFile, GifExtCode, GifExtLen, GifExtension);
		return GIF_ERROR;
	}
	int EGifPutExtensionLast(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutExtensionLast(GifFile, GifExtCode, GifExtLen, GifExtension);
		return GIF_ERROR;
	}
	int EGifPutExtension(GifFileType * GifFile, int GifExtCode, int GifExtLen, const VoidPtr GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutExtension(GifFile, GifExtCode, GifExtLen, GifExtension);
		return GIF_ERROR;
	}
	int EGifPutCode(GifFileType * GifFile, int GifCodeSize, const GifByteType * GifCodeBlock)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutCode(GifFile, GifCodeSize, GifCodeBlock);
		return GIF_ERROR;
	}
	int EGifPutCodeNext(GifFileType * GifFile, const GifByteType * GifCodeBlock)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifPutCodeNext(GifFile, GifCodeBlock);
		return GIF_ERROR;
	}


	int EGifCloseFile(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifEncoder.lpfnEGifCloseFile(GifFile);
		return GIF_ERROR;
	}

	// Decode
	GifFileType *DGifOpenFileName(const char *GifFileName)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifOpenFileName(GifFileName);
		return NULL;
	}
	GifFileType *DGifOpenFileHandle(int GifFileHandle)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifOpenFileHandle(GifFileHandle);
		return NULL;
	}
	int DGifSlurp(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifSlurp(GifFile);
		return GIF_ERROR;
	}
	GifFileType *DGifOpen(void *userPtr, InputFunc readFunc)    /* new one * (TVT) */
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifOpen(userPtr, readFunc);
		return NULL;
	}
	int DGifGetScreenDesc(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetScreenDesc(GifFile);
		return GIF_ERROR;
	}
	int DGifGetRecordType(GifFileType * GifFile, GifRecordType * GifType)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetRecordType(GifFile, GifType);
		return GIF_ERROR;
	}
	int DGifGetImageDesc(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetImageDesc(GifFile);
		return GIF_ERROR;
	}
	int DGifGetLine(GifFileType * GifFile, GifPixelType * GifLine, int GifLineLen)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetLine(GifFile, GifLine, GifLineLen);
		return GIF_ERROR;
	}
	int DGifGetPixel(GifFileType * GifFile, GifPixelType GifPixel)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetPixel(GifFile, GifPixel);
		return GIF_ERROR;
	}
	int DGifGetComment(GifFileType * GifFile, char *GifComment)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetComment(GifFile, GifComment);
		return GIF_ERROR;
	}
	int DGifGetExtension(GifFileType * GifFile, int *GifExtCode, GifByteType ** GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetExtension(GifFile, GifExtCode, GifExtension);
		return GIF_ERROR;
	}
	int DGifGetExtensionNext(GifFileType * GifFile, GifByteType ** GifExtension)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetExtensionNext(GifFile, GifExtension);
		return GIF_ERROR;
	}
	int DGifGetCode(GifFileType * GifFile, int *GifCodeSize, GifByteType ** GifCodeBlock)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetCode(GifFile, GifCodeSize, GifCodeBlock);
		return GIF_ERROR;
	}
	int DGifGetCodeNext(GifFileType * GifFile, GifByteType ** GifCodeBlock)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetCodeNext(GifFile, GifCodeBlock);
		return GIF_ERROR;
	}
	int DGifGetLZCodes(GifFileType * GifFile, int *GifCode)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifGetLZCodes(GifFile, GifCode);
		return GIF_ERROR;
	}
	int DGifCloseFile(GifFileType * GifFile)
	{
		if(nInit==GIFLIB_OK)
			return gifDecoder.lpfnDGifCloseFile(GifFile);
		return GIF_ERROR;
	}
	
private:
	CGifEncoder gifEncoder;
	CGifDecoder gifDecoder;
	GIFLIB_STATUS nInit;
	HMODULE hModule;
};

__declspec(selectany) CGifLib theGifLib;