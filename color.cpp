#ifndef WIN32
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#endif

#include "color.h"
#include "mutil.h"
#include "util.h"

/////////////////////////////////////////////////////////////////////////////
//
COLORREF GetRGB(const char *strColor) {
    char *string = new char[strlen(strColor) + 1];
    int i;
    for(i = 0; strColor[i]; i++) string[i] = toupper(strColor[i]);
    string[i] = 0x00;
    COLORREF color;
    if(!strcmp(string, "BLACK"))
        color = GetRGB(BlackColor);
    else if(!strcmp(string, "DARKRED"))
        color = GetRGB(DarkRedColor);
    else if(!strcmp(string, "DARKGREEN"))
        color = GetRGB(DarkGreenColor);
    else if(!strcmp(string, "DARKYELLO"))
        color = GetRGB(DarkYelloColor);
    else if(!strcmp(string, "DARKBLUE"))
        color = GetRGB(DarkBlueColor);
    else if(!strcmp(string, "DARKMAGENTA"))
        color = GetRGB(DarkMagentaColor);
    else if(!strcmp(string, "DARKCYAN"))
        color = GetRGB(DarkCyanColor);
    else if(!strcmp(string, "LIGHTGRAY"))
        color = GetRGB(LightGrayColor);
    else if(!strcmp(string, "MONEYGREEN"))
        color = GetRGB(MoneyGreenColor);
    else if(!strcmp(string, "SKYBLUE"))
        color = GetRGB(SkyBlueColor);
    else if(!strcmp(string, "CREAM"))
        color = GetRGB(CreamColor);
    else if(!strcmp(string, "MEDIUMGRAY"))
        color = GetRGB(MediumGrayColor);
    else if(!strcmp(string, "DARKGRAY"))
        color = GetRGB(DarkGrayColor);
    else if(!strcmp(string, "RED"))
        color = GetRGB(RedColor);
    else if(!strcmp(string, "GREEN"))
        color = GetRGB(GreenColor);
    else if(!strcmp(string, "YELLO"))
        color = GetRGB(YelloColor);
    else if(!strcmp(string, "BLUE"))
        color = GetRGB(BlueColor);
    else if(!strcmp(string, "MAGENTA"))
        color = GetRGB(MagentaColor);
    else if(!strcmp(string, "CYAN"))
        color = GetRGB(CyanColor);
    else if(!strcmp(string, "WHITE"))
        color = GetRGB(WhiteColor);
    else
        color = GetRGB(WhiteColor);
    delete string;
    return color;
}

COLORREF GetRGB(ColorEnum color) {
    switch(color) {
    case BlackColor: return RGB(0, 0, 0);
    case DarkRedColor: return RGB(128, 0, 0);
    case DarkGreenColor: return RGB(0, 128, 0);
    case DarkYelloColor: return RGB(128, 128, 0);
    case DarkBlueColor: return RGB(0, 0, 128);
    case DarkMagentaColor: return RGB(128, 0, 128);
    case DarkCyanColor: return RGB(0, 128, 128);
    case LightGrayColor: return RGB(192, 192, 192);
    case MoneyGreenColor: return RGB(192, 220, 192);
    case SkyBlueColor: return RGB(166, 202, 240);
    case CreamColor: return RGB(255, 251, 240);
    case MediumGrayColor: return RGB(160, 160, 164);
    case DarkGrayColor: return RGB(128, 128, 128);
    case RedColor: return RGB(255, 0, 0);
    case GreenColor: return RGB(0, 255, 0);
    case YelloColor: return RGB(255, 255, 0);
    case BlueColor: return RGB(0, 0, 255);
    case MagentaColor: return RGB(255, 0, 255);
    case CyanColor: return RGB(0, 255, 255);
    case WhiteColor: return RGB(255, 255, 255);
    }
    return RGB(255, 255, 255);
}
