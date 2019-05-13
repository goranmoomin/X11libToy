// Toy/color.h
//
// Last Modified : 2002/10/19
//

#ifndef _TOY_COLOR_H_
#define _TOY_COLOR_H_

#include "util.h"

//
// Default (or System) Color
//
typedef enum {
    BlackColor = 0,
    DarkRedColor,
    DarkGreenColor,
    DarkYelloColor,
    DarkBlueColor,
    DarkMagentaColor,
    DarkCyanColor,
    LightGrayColor,
    MoneyGreenColor,
    SkyBlueColor,
    CreamColor,
    MediumGrayColor,
    DarkGrayColor,
    RedColor,
    GreenColor,
    YelloColor,
    BlueColor,
    MagentaColor,
    CyanColor,
    WhiteColor
} ColorEnum;

#ifndef __AFX_H__

typedef unsigned long COLORREF;
#define RGB(r, g, b)                                                           \
    ((COLORREF)(((BYTE)(r) | ((WORD)((BYTE)(g)) << 8))                         \
                | (((DWORD)(BYTE)(b)) << 16)))
#define GetRValue(rgb) ((BYTE)(rgb))
#define GetGValue(rgb) ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb) ((BYTE)((rgb) >> 16))

typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#endif

// COLORREF GetRGB(const char *color);
COLORREF GetRGB(ColorEnum color);

#ifdef __AFX_H__

inline COLORREF GrayColor(BYTE intensity) {
    return RGB(intensity, intensity, intensity);
}

class Canvas;
class Bitmap;

class Color {
  public:
    Color();
    ~Color();

    int PaletteSize() { return WhiteColor - WhiteColor + 1; }

    //
    // Pallette
    //
  public:
    typedef enum {
        GrayType,
        BlueType,
        GreenType,
        RedType,
        RainbowType
    } PaletteType;

  private:
    RGBQUAD *m_RGBPalette;
    CPalette m_palette;
    int m_paletteMin, m_paletteMax;

  public:
    void AllocPallette(PaletteType palleteType);

    //    COLORREF GrayColor(BYTE intensity);

    //    int GraySize()    { return m_grayMax-m_grayMin+1; }
    //    int GrayMin()     { return m_grayMin; }
    //    int GrayMax()     { return m_grayMax; }

    void Invert();

    friend class Canvas;
    friend class Bitmap;
};

#endif

#endif
