#include<string>
#include<iostream>
#include<windows.h>

void str_word(wchar_t* x)
{
    /*MessageBoxW(0,x,L"console_log", MB_OK);
    wchar_t wn[1]={L'.'};
    //wchar_t wn_1 = wn[0];
    char n[1]={(char)(int)wn[0]};
    std::cout<<"|"<<n[0]<<"|";
    //wc=L"1";*/


    char xsys_part[20]="";//(char *)data_5[i];//"1.bmp";//что копируем

    for(int ghx=0;ghx<5;ghx++)
    {
        xsys_part[ghx] = (char)((int)x[ghx]);
    }

    //system("pause");
    std::string xsys_str="COPY /Y img\\";//откуда копируем
    //xsys_str += xsys_part;
    xsys_str += xsys_part;//что копируем
    xsys_str += " x.bmp";//куда копируем
    const char *xsys=xsys_str.c_str();
    system(xsys);
    //MoveFile([in] LPCTSTR lpExistingFileName,[in] LPCTSTR lpNewFileName);
    //system("C:\\Users\\denis\\Desktop\\event_master\\q\\winapi_1\\ctrl_p.txt");
    //system("C:\\Users\\denis\\Desktop\\event_master\\q\\winapi_1\\ctrl_p.docm");
    system("ctrl_p.docm");
}
