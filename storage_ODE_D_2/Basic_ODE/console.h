#ifndef _Console_H
#define _Console_H

#include <windows.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <io.h>

class ConsoleWindow
{
    public:
        ConsoleWindow();
        ~ConsoleWindow();
        
        void Open();
        void Close();
        HWND GetHandle(){return (HWND)hStdHandle;}
        bool Status(){if (hStdHandle == INVALID_HANDLE_VALUE) return FALSE; else return TRUE;}

    protected:
        const WORD MAX_CONSOLE_ROWS;
        const WORD MAX_CONSOLE_COLUMNS;
        FILE *fp;
        HANDLE hStdHandle;
        int hConHandle;
};

#endif
