#include <windows.h>
#include "winfuncs.h"

extern int DialogInUse;
extern float lightColor[3];
extern HWND hWnd;

// Hook function for color common dialog window
UINT APIENTRY ColorDialogHook(HWND hdlg, UINT msg, WPARAM, LPARAM)
{
    switch (msg)
    {
        case WM_INITDIALOG:
        {
            if(DialogInUse == 1)  // Ambient
                SetWindowText(hdlg, "Choose Ambient Color");
            if(DialogInUse == 2)  // Diffuse
                SetWindowText(hdlg, "Choose Diffuse Color");
            if(DialogInUse == 3)  // Specular
                SetWindowText(hdlg, "Choose Specular Color");
            return TRUE;
        }

        default:
            return FALSE;
    }
}

void GetLightColor()
{
    COLORREF colors[16];
    COLORREF colorRGB = RGB(lightColor[0]*255, lightColor[1]*255, lightColor[2]*255);
    CHOOSECOLOR colorDialog;
    memset(&colorDialog,0,sizeof(colorDialog));
    memset(colors,255,sizeof(colors));
    colorDialog.lStructSize = sizeof(CHOOSECOLOR);
    colorDialog.hwndOwner = hWnd;
    colorDialog.rgbResult = colorRGB;
    colorDialog.lpCustColors = colors;
    colorDialog.Flags = CC_FULLOPEN | CC_RGBINIT | CC_ENABLEHOOK;
    colorDialog.lpfnHook = (LPCCHOOKPROC)ColorDialogHook;
    ChooseColor(&colorDialog);
    lightColor[0] = (float)GetRValue(colorDialog.rgbResult)/255;
    lightColor[1] = (float)GetGValue(colorDialog.rgbResult)/255;
    lightColor[2] = (float)GetBValue(colorDialog.rgbResult)/255;
}
