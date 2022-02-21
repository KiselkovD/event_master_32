#include<iostream>//стандартная
#include<conio.h>//для getch
#include<windows.h>// заголовочный файл, содержащий функции API
#include <ctime>//работа со временем
#include<fstream>//работа с файлом
#include<string>//работа со строками

//#define IDI_ICON 101
#define MAX_EV_COUT 90//максимальное допустимое количество событий в файле
#define EV_PART_COUNT 5//число частей события
#define MAX_CHAR_IN_STRING 2500//длинна события
//#define IDI_ICON_1 243
//using std::wcout;//чтобы не приписывать пространство имен std к каждому cout и cin
//using std::cin;

//добавить печать, выбор событий, прокручивание

//объявления функций
LRESULT CALLBACK WindowProcedure(HWND,UINT,WPARAM,LPARAM);//функции, которые обрабатывают все сообщения своих окон
LRESULT CALLBACK WindowProcedure2(HWND,UINT,WPARAM,LPARAM);
//LRESULT CALLBACK WindowProcedure2(HWND,UINT,WPARAM,LPARAM);
//CALLBACK (__stdcall):Функции, вызываемые из "внутри Windows", просто должны быть stdcall
//потому что разработчики Windows решили написать/скомпилировать Windows, что он вызывает функции stdcall
//LRESULT (некоторая int/pointer-like вещь) - это только возвращаемый тип.

void AddControls(HWND);
void AddControls2(HWND);
void AddControls3(HWND);
void AddControls4(HWND);
void LoadImages();
bool load(int);
void eee(HWND);
void eee2(HWND);
void str_word(wchar_t*);

//глобальные переменные
wchar_t Month_edit[4] = {L""};
wchar_t Date_edit[8] = {L""};
wchar_t Type_edit[2] = {L""},start_Type_edit[2] = {L""};
//Type_edit


int i;//сохраняем счетчик цикла для вывода в окнах
bool fl=0,fl_2=0,contin_win=1,not_next=0,fl_next_button=0,fl_destroy_self=0,fl_3_win=0,fl_3_win_yes=0,fl_next_turn=0,fl_evetype=1,fl_win_buff=0;//набор флагов
bool contin=1; //переменная которая определяет идет ли програма или заканчивается
HWND hEdit1, hEdit2, hEdit3, hEdit4, hImgControl;//дескрипторы окон
HBITMAP hImg;//дескриптор изображения
//wchar_t data_main[MAX_EV_COUT*EV_PART_COUNT][MAX_CHAR_IN_STRING]= {L""}; //хранит части событий: даты, информации и подробные информации //число частей
wchar_t data_1[MAX_EV_COUT][20]= {L""};
wchar_t data_2[MAX_EV_COUT][MAX_CHAR_IN_STRING]= {L""};
wchar_t data_3[MAX_EV_COUT][MAX_CHAR_IN_STRING]= {L""};
wchar_t data_4[MAX_EV_COUT][20]= {L""};
wchar_t data_5[MAX_EV_COUT][20]= {L""};
//wchar_t bclear[MAX_CHAR_IN_STRING]= {L""};
int event_count;//считает склько событий
int month=13;//для удобства будем хранить тут месяц

char en_month[12][4]= {"Jan", "Feb", "Mar", "Apr", "May", "Jun",//массив для сверки месяца
                       "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                      };
wchar_t en_month_L[12][4]= {L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun",//массив для сверки месяца
                       L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"
                      };
wchar_t ru_month[12][10]= {L"Январь", L"Февраль", L"Март", L"Апрель", L"Май", L"Июнь",//массив для вывода месяца в окно
                          L"Июль", L"Август", L"Сентябрь", L"Октябрь", L"Ноябрь", L"Декабрь"
                         };




//используем WinMain вместо main чтобы работать с WINAPI
// Основная функция - аналог int main() в консольном приложении:
int WINAPI WinMain(HINSTANCE hInst,     //дескриптор экземпляра приложения
                   HINSTANCE hPrevInst, // в Win32 не используется
                   LPSTR args,          // нужен для запуска окна в режиме командной строки
                   int ncmdshow)        // режим отображения окна
{
    //MAKEINTRESOURCE(243);
    //MessageBoxW(0,L"666",L"console_log", MB_OK);
    std::cout<<"";
    //std::cout<<(Type_edit[0] == start_Type_edit[0])<<"\n";
    //system("pause");
    WNDCLASSW wincl = {0};//создаем и обнуляем класс окна
    //LoadIcon(hInst)

    wincl.hbrBackground = (HBRUSH)COLOR_WINDOW;//фон
    wincl.hCursor = LoadCursor(NULL,IDC_ARROW);//курсор
    wincl.hInstance = hInst;//дескриптор модуля
    //wincl.hIcon = LoadIcon(NULL,IDI_INFORMATION);
    wincl.lpszClassName = L"myClass";//название класса
    wincl.lpfnWndProc = WindowProcedure;//подкл функцию, которая обработает все сообщения окона

    if(!RegisterClassW(&wincl))//Регистрирует класс окна для последующего использования в вызовах функции CreateWindow
        return -1;//код ошибки если не удалось

    WNDCLASSW wincl2 = {0};//создаем и обнуляем класс окна

    wincl2.hbrBackground = (HBRUSH)COLOR_WINDOW;//фон
    wincl2.hCursor = LoadCursor(NULL,IDC_ARROW);//курсор
    wincl2.hInstance = hInst;//дескриптор модуля
    //wincl2.hIcon = LoadIcon(NULL,IDI_INFORMATION);
    wincl2.lpszClassName = L"myClass2";//название класса
    wincl2.lpfnWndProc = WindowProcedure2;//подкл функцию, которая обработает все сообщения окона

    if(!RegisterClassW(&wincl2))//Регистрирует класс окна для последующего использования в вызовах функции CreateWindow
        return -1;//код ошибки если не удалось

    setlocale(LC_ALL, "Russian");//ставим язык нашей родины


    time_t sec = time(NULL);//узнаем системное время
    tm* t_inf = localtime(&sec);//указатель с сегодняшней датой


    for(int t=0; t<12; t++)//счетчик цикла
    {
        //std::cout<<" "<<asctime(t_inf)[4]<<" ";
        //std::cout<<" "<<en_month[t][0]<<" ";
        if(asctime(t_inf)[4]==en_month[t][0]&&//сравниваем символы из сегодняшней даты и из массива месяцов
                asctime(t_inf)[5]==en_month[t][1]&&
                asctime(t_inf)[6]==en_month[t][2])
        {
            month=t;//сохраняем сегодняшний месяц
        }
    }

    load(month);//загружаем данные из файла в массив
    //if(!load())//загружаем данные из файла в массив
    //    wcout<<L"не удалось загрузить файл";
    //    else wcout<<L"удалось загрузить файл";



    //поскольку нужно чтобы сначала мы проверилисегодяшний день а потом запустили таймер добавляем перед циклом все кроме таймера


    while(contin)//отлслеживает когда закончить
    {

        if(fl_next_turn)Sleep(8640000);//ждем один день
        fl_next_turn=1;

        if(fl_evetype)
        {
            CreateWindowW(L"myClass2",L"выбор типа!",WS_OVERLAPPEDWINDOW | WS_VISIBLE,100,100,200,200,NULL,NULL,NULL,NULL);
            MSG msg = {0};  //информация о сообщении из очереди сообщений потока
            while( GetMessage(&msg,NULL,NULL,NULL) )//проверка нажатия клавиш и тп
            {
                TranslateMessage(&msg);//переводит сообщения виртуальных клавиш в символьные сообщения
                DispatchMessage(&msg);//используется, чтобы доставить сообщение, извлеченное функцией GetMessage
            }
        }
        fl_evetype=0;


       time_t seconds = time(NULL);//узнаем системное время
        tm* timeinfo = localtime(&seconds);//указатель с сегодняшней датой
        char timeinf_wchar[24];
        for(int hty=0;hty<24;hty++)
        timeinf_wchar[hty]=(asctime(timeinfo))[hty];
        //std::cout<<timeinf_wchar<<"\n";

//MessageBoxW(0,L"1",L"1", MB_OK);
        if (fl_3_win)
        {


            //MessageBoxW(0,L"fuck",L"console_log", MB_OK);
            //MessageBoxW(0,L"1",L"1", MB_OK);
            CreateWindowW(L"myClass",L"выбор даты!",WS_OVERLAPPEDWINDOW | WS_VISIBLE,100,100,200,200,NULL,NULL,NULL,NULL);

            MSG msg = {0};  //информация о сообщении из очереди сообщений потока

            while( GetMessage(&msg,NULL,NULL,NULL) )//проверка нажатия клавиш и тп
            {

                TranslateMessage(&msg);//переводит сообщения виртуальных клавиш в символьные сообщения
                DispatchMessage(&msg);//используется, чтобы доставить сообщение, извлеченное функцией GetMessage
            }


            for (int hh=0;hh<MAX_EV_COUT/* * EV_PART_COUNT*/;hh++)//очистим массив
                for (int hhh=0;hhh<MAX_CHAR_IN_STRING;hhh++)
                {

                        data_2[hh][hhh]=L'\0';
                        data_3[hh][hhh]=L'\0';

                }
            for (int hh=0;hh<MAX_EV_COUT/* * EV_PART_COUNT*/;hh++)//очистим массив
                for (int hth=0;hth<20;hth++)
                {
                        data_1[hh][hth]=L'\0';
                        data_4[hh][hth]=L'\0';
                        data_5[hh][hth]=L'\0';
                }


            int te=0;
            int tt=0;
            for(te=0; te<12; te++)//счетчик цикла
            {
                //MessageBoxW(0,L"1",en_month_L[te], MB_OK);
                //MessageBoxW(0,L"1",Month_edit, MB_OK);
                //std::cout<<" "<<asctime(t_inf)[4]<<" ";
                //std::cout<<" "<<en_month[t][0]<<" ";
                if(Month_edit[0]==ru_month[te][0]&&//сравниваем символы из сегодняшней даты и из массива месяцов
                        Month_edit[1]==ru_month[te][1]&&
                        Month_edit[2]==ru_month[te][2])
                {
//MessageBoxW(0,L"console_log",L"666", MB_OK);
                    tt=te;//сохраняем сегодняшний месяц
                }
            }

            //19 2021
            //012345678901234567890123
            //std::cout<<" 1111("<<tt<<" ";
            load(tt);
            //for(int htyt=0;htyt<8;htyt++)
            timeinf_wchar[8]=Date_edit[0];
            timeinf_wchar[9]=Date_edit[1];
            //timeinf_wchar[20]=Date_edit[3];
            //timeinf_wchar[21]=Date_edit[4];
            //timeinf_wchar[22]=Date_edit[5];
            //timeinf_wchar[23]=Date_edit[6];
            //std::cout<<timeinf_wchar<<"\n";

            fl_3_win_yes=1;
            fl_3_win=0;
            //MessageBoxW(0,L"1",L"1", MB_OK);
        }

        i=0;
        while (i<event_count/EV_PART_COUNT+1)//сколько событий считали столько дат и проверяем //число частей
        {
            //сравниваем символы из сегодняшней даты и из массива //число частей
            //MessageBoxW(0,L"",Type_edit, MB_OK);
            if( (Type_edit[0] == start_Type_edit[0] || Type_edit[0] == data_5[i][0] )&&//data_main[i*EV_PART_COUNT+4][0] )&&
               timeinf_wchar[8]== data_1[i][4-4]&&//data_main[i*EV_PART_COUNT][4-4]&&
                    timeinf_wchar[9]== data_1[i][5-4]&&//data_main[i*EV_PART_COUNT][5-4]&&
                    timeinf_wchar[22]== data_1[i][9-4]&&//data_main[i*EV_PART_COUNT][9-4]&&
                    timeinf_wchar[23]== data_1[i][10-4])//data_main[i*EV_PART_COUNT][10-4])
            {
                for (int l=i+1;l<event_count/EV_PART_COUNT+1;l++)
                {
                    if( (Type_edit[0] == start_Type_edit[0] || Type_edit[0] == data_5[l][0] )&&//data_main[i*EV_PART_COUNT+4][0] )&&
                       timeinf_wchar[8]== data_1[l][4-4]&&//data_main[l*EV_PART_COUNT][4-4]&&
                    timeinf_wchar[9]== data_1[l][5-4]&&//data_main[l*EV_PART_COUNT][5-4]&&
                    timeinf_wchar[22]== data_1[l][9-4]&&//data_main[l*EV_PART_COUNT][9-4]&&
                    timeinf_wchar[23]== data_1[l][10-4])//data_main[l*EV_PART_COUNT][10-4])
                    fl_next_button=1;//MessageBoxW(0,L"666",L"console_log", MB_OK);}

                }
                while(contin_win)//пока окно не закрыли
                {
                    //MessageBoxW(0,L"fuck2",L"console_log", MB_OK);
                    CreateWindowW(L"myClass",L"Событие!",WS_OVERLAPPEDWINDOW | WS_VISIBLE,100,100,500,450,NULL,NULL,NULL,NULL);
                    MSG msg = {0};  //информация о сообщении из очереди сообщений потока
                    while( GetMessage(&msg,NULL,NULL,NULL) )//проверка нажатия клавиш и тп
                    {
                        TranslateMessage(&msg);//переводит сообщения виртуальных клавиш в символьные сообщения
                        DispatchMessage(&msg);//используется, чтобы доставить сообщение, извлеченное функцией GetMessage
                    }
                    fl_2=fl;//перставляем флаг выбирающий внутренности окна
                }
                fl_next_button=0;
                contin_win=1;//повторно инициализируем переменную
                fl=fl_2=0; //след событие выведется первым окном

                /*if (fl)//проверяем нужно ли второе окно
                {
                    CreateWindowW(L"myClass2",L"Дополнительная информация",WS_OVERLAPPEDWINDOW | WS_VISIBLE,100,100,500,450,NULL,NULL,NULL,NULL);//создаем второе окно
                    MSG msg = {0};  //информация о сообщении из очереди сообщений потока
                    while( GetMessage(&msg,NULL,NULL,NULL) )//проверка нажатия клавиш и тп
                    {
                        TranslateMessage(&msg);//переводит сообщения виртуальных клавиш в символьные сообщения
                        DispatchMessage(&msg);//используется, чтобы доставить сообщение, извлеченное функцией GetMessage
                    }
                    fl=0;//обнуляем флаг для повтороного использования
                }*/


            }
            ++i;
            if (not_next)//не смотреть другие события за сегодня
            {
                not_next=0;
                break;
            }
        }

        if (fl_3_win)
        {
            contin=1;
            fl_next_turn=0;
        }
    }

/////////////////////////////////////////////////////////////////
    return 0;
}

bool load(int month_)//загружает файл
{

    //std::cout<<" "<<month<<" ";
    //std::cout<<month;
    //MessageBoxW(0,ru_month[1],L"1", MB_OK);
    //MessageBoxW(0,en_month[month],L"1", MB_OK);
    wchar_t x='0';//переменная для посимвольного считывания

    std::fstream f;//поток
    switch (month_)//выбираем файл для открытия в зависимости от месяца
    {//"Jan", "Feb", "Mar", "Apr", "May", "Jun",//массив для сверки месяца
      //                 "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    case 0:
        f.open("month\\Jan.txt",std::ios_base::in);
        break;
    case 1:
        f.open("month\\Feb.txt",std::ios_base::in);
        break;
    case 2:
        f.open("month\\Mar.txt",std::ios_base::in);
        break;
    case 3:
        f.open("month\\Apr.txt",std::ios_base::in);
        break;
    case 4:
        f.open("month\\May.txt",std::ios_base::in);
        break;
    case 5:
        f.open("month\\Jun.txt",std::ios_base::in);
        break;
    case 6:
        f.open("month\\Jul.txt",std::ios_base::in);
        break;
    case 7:
        f.open("month\\Aug.txt",std::ios_base::in);
        break;
    case 8:
        f.open("month\\Sep.txt",std::ios_base::in);
        break;
    case 9:
        f.open("month\\Oct.txt",std::ios_base::in);
        break;
    case 10:
        f.open("month\\Nov.txt",std::ios_base::in);
        break;
    case 11:
        f.open("month\\Dec.txt",std::ios_base::in);
        break;
    }

    //f.open("t.txt",std::ios_base::in);//открываем поток для работы с файлом
    if(!f.is_open())//проверка успешности открытия
        return false;//функция load скажет удалось ли ей загрузить
    int i=0,j=0;//переменные цикла

    while('$'!=x)//проверка конца файла
    {

        {
            //перевод символов в wchar_t
            char c[1];
            c[0]=f.get();//считываем один символ
            size_t cSize = strlen(c)+1;
            wchar_t wc[cSize];
            mbstowcs (wc, c, cSize);
            x=wc[0];
        }

        if(x==';')//если это точка с запятой - начинаем записывать в след элемент массива
        {
            i=0;//начинаем считывать строку с её начала
            j++;//следующий элемент массива
            if (MAX_EV_COUT*EV_PART_COUNT<j)//проверка на превышение допустисого количесва событий //число частей
            {
                //wcout<<"превышено число событий";
                return(1);//функция всеравно вернет успешно
            }
        }
        else
        {
            if(x!='\n')//проверка что это не перенос строки(нужно чтобы игнорировать переносы строки в файле)
            {
                switch(j%5)
                {
                case 0:
                    data_1[j/5][i]=x;
                    break;
                case 1:
                    data_2[j/5][i]=x;
                    break;
                case 2:
                    data_3[j/5][i]=x;
                    break;
                case 3:
                    data_4[j/5][i]=x;
                    break;
                case 4:
                    data_5[j/5][i]=x;
                    break;
                }

                //data_main[j][i]=x;//запись символа из файла в строку в массив
                i++;//увеличиваем переменную которая помнит на каком символе строки мы находимся чтоб в следующий раз считать следующий символ
            }
        }
        event_count=j;//запоминаем сколько событий мы считали
    }//считываем
    f.close();//закрывае поток
    //DEBAG
    /*for (int nd=0;nd<60;nd++)
    {
        MessageBoxW(0,data_1[nd],L"console_log", MB_OK);
    }/* */
    return true;//если програма завершится успешно вернет 1
}

LRESULT CALLBACK WindowProcedure(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    if(fl_3_win)
    {
        switch(msg)//сообщение окна
        {
        case WM_COMMAND:
            switch(wp)
            {
            case 1:
                GetWindowTextW(hEdit1,Month_edit,4);
                GetWindowTextW(hEdit2,Date_edit,8);
                GetWindowTextW(hEdit3,Type_edit,2);
                DestroyWindow(hWnd);//уничтожить окно
                PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
                break;

            case 2:
                GetWindowTextW(hEdit3,Type_edit,2);
                DestroyWindow(hWnd);//уничтожить окно
                PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
                break;
            }
            break;
        case WM_CREATE:

            AddControls3(hWnd);
            break;
        case WM_DESTROY://если кнопка закрытия
            //если окно закрыто пользователем прекратить создавать программу
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);//окно сообщает что все норм
        }
    }
    else
    if(fl_2)//флаг определяющий наполнение окна
    {//второе окно при fl_2==1
        switch(msg)//сообщение окна
        {
        case WM_COMMAND:
            switch(wp)
            {
            case 1:
                fl=0;//флаг "какое окно откроется следующим"
                DestroyWindow(hWnd);//уничтожить окно
                PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
                break;
            }
            break;
        case WM_CREATE:
            LoadImages();

            AddControls2(hWnd);
            break;
        case WM_DESTROY://если кнопка закрытия
            if(fl==fl_2){contin_win=0;//если окно закрыто пользователем прекратить создавать окна
            contin=0;}//если окно закрыто пользователем прекратить создавать программу
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);//окно сообщает что все норм
        }

    }
    else
    {//первое окно при fl_2==0
        switch(msg)//сообщение окна
    {
    case WM_COMMAND:
        switch(wp)
        {
        case 1:
            fl=1;//флаг второго окна
            //msg=WM_DESTROY;
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        case 2:
            contin_win=0;//если окно закрыто пользователем прекратить создавать окна
            fl=1;//флаг второго окна
            not_next=1;//не смотреть другие события за сегодня
            //msg=WM_DESTROY;
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        case 3:
            contin_win=0;//если окно закрыто пользователем прекратить создавать окна
            fl=1;//флаг второго окна
            not_next=0;//смотреть другие события за сегодня
            fl_next_button=0;
            //msg=WM_DESTROY;
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        case 6:
            fl_3_win=1;
            contin_win=0;
            i=32;
            //msg=WM_DESTROY;
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        case 4:
            contin_win=0;//если окно закрыто пользователем прекратить создавать окна
            fl=1;//флаг второго окна
            not_next=0;
            i=-1;
            //msg=WM_DESTROY;
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        case 5:
            //std::fstream ff;
            std::wofstream  ff;//trunc
            ff.open("ctrl_p.txt",std::ios::out);
            char x;
            int th=-1;
            //while(data_main[i*EV_PART_COUNT][th+1]!='\0')
            while(data_1[i][th+1]!='\0')//если это точка с запятой - начинаем записывать в след элемент массива
                {
                    th++;//следующий элемент массива

                    ff << data_1[i][th];//data_main[i*EV_PART_COUNT][th];
                }
            ff << L" ";
            //ff << '\n';
            th=-1;
            //while(data_main[i*EV_PART_COUNT+1][th+1]!='\0')
            while(data_2[i][th+1]!='\0')//если это точка с запятой - начинаем записывать в след элемент массива
                {
                    th++;//следующий элемент массива

                    //std::cout << (int)b[i*EV_PART_COUNT+1][th]<<"\n";
                    //MessageBoxW(0,L"1",(int)b[i*EV_PART_COUNT+1][th],MB_OK);
                    //std::cout << " " << convert((int)bb[0]) << " ";

                    ff << data_2[i][th];//data_main[i*EV_PART_COUNT+1][th];

                }
            ff << L" ";
            //ff << '\n';
            th=-1;
            //while(data_main[i*EV_PART_COUNT+2][th+1]!='\0')
            while(data_3[i][th+1]!='\0')//если это точка с запятой - начинаем записывать в след элемент массива
                {
                    th++;//следующий элемент массива
                    ff << data_3[i][th];//data_main[i*EV_PART_COUNT+2][th];

                }
            //MessageBoxW(0,L"1",L"1", MB_OK);
            //ff << (char*)(wchar_t*)b[i*EV_PART_COUNT];
            //ff.write(,sizeof);
            ff.close();

            str_word(data_4[i]);

            //ff.open("Aug.txt",std::ios_base::out);
            break;

        }
        break;
    case WM_CREATE:
        LoadImages();
        AddControls(hWnd);
        break;
    case WM_DESTROY://если кнопка закрытия
        if(fl==fl_2){contin_win=0;//если окно закрыто пользователем прекратить создавать окна
        contin=0;}//если окно закрыто пользователем прекратить создавать программу

        PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
        break;
    default:
        return DefWindowProcW(hWnd,msg,wp,lp);//окно сообщает что все норм
    }
    }

}

LRESULT CALLBACK WindowProcedure2(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)//сообщение окна
        {
        case WM_COMMAND:
            switch(wp)
            {
            case 1:
                GetWindowTextW(hEdit1,Month_edit,4);
                GetWindowTextW(hEdit2,Date_edit,8);
                GetWindowTextW(hEdit3,Type_edit,2);
                DestroyWindow(hWnd);//уничтожить окно
                PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
                break;

            case 2:
                GetWindowTextW(hEdit3,Type_edit,2);
                DestroyWindow(hWnd);//уничтожить окно
                PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
                break;
            }
            break;
        case WM_CREATE:

            AddControls4(hWnd);
            break;
        case WM_DESTROY://если кнопка закрытия
            //если окно закрыто пользователем прекратить создавать программу
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        default:
            return DefWindowProcW(hWnd,msg,wp,lp);//окно сообщает что все норм
        }
}
void AddControls(HWND hWnd)//создаем все внутри окна
{
    //wcout<<" "<< ru_month[month]  <<" ";
    //for (int j=0;j<60;j++)
    //for (int j2=0;j2<60;j2++)
    //CreateWindowW(L"static",L"",WS_VISIBLE | WS_CHILD | WS_BORDER,j*10,j2*10,10,10,hWnd,NULL,NULL,NULL);

    wchar_t b0[MAX_CHAR_IN_STRING]= {L""};
    int th=-1;
    //while(data_main[i*EV_PART_COUNT+1][th+1]!='\0')//если это точка с запятой - начинаем записывать в след элемент массива
    while(data_2[i][th+1]!='\0')
        {
            //MessageBoxW(0,b[i*EV_PART_COUNT+1][th+1],L"1", MB_OK);
            th++;//следующий элемент массива
            b0[th]=data_2[i][th];//data_main[i*EV_PART_COUNT+1][th];

        }


    if(fl_3_win_yes)
    {
        CreateWindowW(L"static",Month_edit,WS_VISIBLE | WS_CHILD | ES_CENTER,80,20,60,20,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"static",Date_edit,WS_VISIBLE | WS_CHILD | ES_CENTER,80,40,60,20,hWnd,NULL,NULL,NULL); //дата (здесь можно менять его размеры/положение)

    }
    else
    {
        CreateWindowW(L"static",ru_month[month],WS_VISIBLE | WS_CHILD | ES_CENTER,80,20,60,20,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"static",data_1[i],WS_VISIBLE | WS_CHILD | ES_CENTER,80,40,60,20,hWnd,NULL,NULL,NULL);//data_main[i*EV_PART_COUNT],WS_VISIBLE | WS_CHILD | ES_CENTER,80,40,60,20,hWnd,NULL,NULL,NULL); //дата (здесь можно менять его размеры/положение)
    }
    CreateWindowW(L"static",data_5[i],WS_VISIBLE | WS_CHILD | ES_CENTER,80,60,60,20,hWnd,NULL,NULL,NULL);//data_main[i*EV_PART_COUNT+4],WS_VISIBLE | WS_CHILD | ES_CENTER,80,60,60,20,hWnd,NULL,NULL,NULL);
    CreateWindowW(L"Edit",b0,WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,220,10,230,320,hWnd,NULL,NULL,NULL); //первая информация (здесь можно менять его размеры/положение)
    CreateWindowW(L"Button",L"печать",WS_VISIBLE | WS_CHILD,20,350,100,20,hWnd,(HMENU)5,NULL,NULL);
    CreateWindowW(L"Button",L"подробнее",WS_VISIBLE | WS_CHILD,260,350,100,50,hWnd,(HMENU)1,NULL,NULL); //кнопка подробнее (здесь можно менять его размеры/положение)
    CreateWindowW(L"Button",L"выход",WS_VISIBLE | WS_CHILD,140,350,100,50,hWnd,(HMENU)2,NULL,NULL);
    CreateWindowW(L"Button",L"сменить дату",WS_VISIBLE | WS_CHILD,20,380,100,20,hWnd,(HMENU)6,NULL,NULL);
    if(fl_next_button)
    {
        CreateWindowW(L"Button",L"след",WS_VISIBLE | WS_CHILD,380,350,100,20,hWnd,(HMENU)3,NULL,NULL);
    }
    CreateWindowW(L"Button",L"с начала",WS_VISIBLE | WS_CHILD,380,380,100,20,hWnd,(HMENU)4,NULL,NULL);
    hImgControl = CreateWindowW(L"Static",NULL,WS_VISIBLE | WS_CHILD | SS_BITMAP,10,100,0,0,hWnd,NULL,NULL,NULL); //изображение (здесь можно менять его положение)
    SendMessageW(hImgControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hImg);//для связи создания и загрузки изображения
}

void LoadImages()
{
   wchar_t buffer[30]=L"1";
    wcscpy_s(buffer,30,L"img\\");
    wcscat_s(buffer,30-ARRAYSIZE(L"img\\"),data_4[i]);
    hImg = (HBITMAP)LoadImageW(NULL,buffer,IMAGE_BITMAP,180,230,LR_LOADFROMFILE);//data_main[i*EV_PART_COUNT+3],IMAGE_BITMAP,180,230,LR_LOADFROMFILE);//загрузка изображения (здесь можно менять его размеры)
}

/*LRESULT CALLBACK WindowProcedure2(HWND hWnd,UINT msg,WPARAM wp,LPARAM lp)
{
    switch(msg)//сообщение окна
    {
    case WM_COMMAND:
        switch(wp)
        {
        case 1:
            DestroyWindow(hWnd);//уничтожить окно
            PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
            break;
        }
        break;
    case WM_CREATE:
        LoadImages();

        AddControls2(hWnd);
        break;
    case WM_DESTROY://если кнопка закрытия
        PostQuitMessage(0);//поток сделал запрос на то, чтобы завершить свою работу
        break;
    default:
        return DefWindowProcW(hWnd,msg,wp,lp);//окно сообщает что все норм
    }
}*/

void AddControls2(HWND hWnd)
{
    wchar_t b0[MAX_CHAR_IN_STRING]= {L""};
    int th=-1;
    while(data_3[i][th+1]!='\0')//data_main[i*EV_PART_COUNT+2][th+1]!='\0')//если это точка с запятой - начинаем записывать в след элемент массива
        {
            //MessageBoxW(0,b0,L"1", MB_OK);
            th++;//следующий элемент массива
            b0[th]=data_3[i][th];//data_main[i*EV_PART_COUNT+2][th];

        }
    CreateWindowW(L"Edit",b0,WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,10,10,460,320,hWnd,NULL,NULL,NULL); //вторая информация (здесь можно менять его размеры/положение)
    CreateWindowW(L"Button",L"Назад",WS_VISIBLE | WS_CHILD,200,350,100,50,hWnd,(HMENU)1,NULL,NULL); //кнопка (здесь можно менять его размеры/положение)
    SendMessageW(hImgControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hImg);//для связи создания и загрузки изображения
}

void AddControls3(HWND hWnd)
{


        //hEdit1=CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,10,10,90,20,hWnd,NULL,NULL,NULL); //вторая информация (здесь можно менять его размеры/положение)
        hEdit1=CreateWindow("COMBOBOX", "combobox",WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,10,10,90,500,hWnd,NULL,NULL,NULL);
        eee(hEdit1);
        //SendMessage(hEdit1, CB_ADDSTRING, 0, (LPARAM)"Янв");
        //SendMessage(hEdit1, CB_ADDSTRING, 0, (LPARAM)"Дек");
        //SendMessage(hEdit1, CB_ADDSTRING, 0, (LPARAM)"Лол");
        CreateWindowW(L"static",L"***",WS_VISIBLE | WS_CHILD ,11,12,18,10,hWnd,NULL,NULL,NULL);
        //hEdit2=CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,10,40,90,20,hWnd,NULL,NULL,NULL); //вторая информация (здесь можно менять его размеры/положение)
        hEdit2=CreateWindowW(L"COMBOBOX", L"combobox",WS_CHILD | WS_VISIBLE  | CBS_DROPDOWNLIST,10,40,90,600,hWnd,NULL,NULL,NULL);
        eee2(hEdit2);
        CreateWindowW(L"static",L"**",WS_VISIBLE | WS_CHILD ,11,42,60,10,hWnd,NULL,NULL,NULL);
        hEdit3=CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,10,70,90,20,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"static",L"****",WS_VISIBLE | WS_CHILD ,11,72,60,10,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"Button",L"Поиск",WS_VISIBLE | WS_CHILD,10,100,90,20,hWnd,(HMENU)1,NULL,NULL); //кнопка (здесь можно менять его размеры/положение)

        SendMessageW(hImgControl,STM_SETIMAGE,IMAGE_BITMAP,(LPARAM)hImg);//для связи создания и загрузки изображения


}
void AddControls4(HWND hWnd)
{

        hEdit3=CreateWindowW(L"Edit",L"",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL,10,70,90,20,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"static",L"****",WS_VISIBLE | WS_CHILD ,11,72,60,10,hWnd,NULL,NULL,NULL);
        CreateWindowW(L"Button",L"Поиск",WS_VISIBLE | WS_CHILD,10,100,90,20,hWnd,(HMENU)2,NULL,NULL);


}
