#include <cstdlib>
#include <ctime>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

#define strzalka_lewo 0x25
#define strzalka_prawo 0x27
#define strzalka_dol 0x28
#define strzalka_gora 0x26
#define enter 0x0D
#define f_key 0x46

struct pole 
{
    int wartosc;
    bool odkryte;
    bool flaga;

};

int lvlx = 0, lvly = 0, o_lvlx = 1, o_lvly = 1;
int koniec = 0;
int ilosc_flag;
time_t start;

void odkryj_plansze(int x, int y, int sizex, int sizey, pole * plansza);

void pokaz_plansze(int sizex, int sizey, pole * plansza, int bombs);

void sterowanie(int bombs, int sizex, int sizey, pole * plansza);

bool setmine(int x, int y, int sizex, int sizey, pole * plansza);

void mina(int miny, int sizex, int sizey, pole * plansza);

bool checkwin(int minesnum, int sizex, int sizey, pole * plansza);

bool rob_plansze(int sizex, int sizey, pole * plansza);

void pokaz_plansze_puste(int sizex, int sizey, pole * plansza);

double czas_gry();



int main(int argc, char *argv[])
{
    int sizex, sizey;
    cout << "Witaj w grze C++ Saper 2020!";
    cout << endl << "Podaj prosze poziomy wymiar planszy!" << endl;
    cin >> sizey;
    system("cls");
    cout << "Teraz podaj pionowy wymiar planszy" << endl;
    cin >> sizex;
    pole * plansza = new pole [sizex];
    for(int i=0; i<sizex; i++)
    {
        plansza[i] = new pole [sizey];
    }  
    system("cls");
    int bombs;
    cout << "Teraz podaj proszê iloœæ bomb na planszy" << endl;
    cin >> bombs;
    while(bombs>=sizex*sizey || bombs<1)
    {
        system("cls");
        cout << "Za duzo bomb! Wprowadz inna wartosc!" << endl;
        cin >> bombs;
    }
    rob_plansze(sizex, sizey, plansza);
    mina(bombs, sizex, sizey, plansza);
    Sleep(200);
    start=time(0);
    while(koniec==0)
    {
        sterowanie(bombs, sizex, sizey, plansza);
        if (checkwin(bombs, sizex, sizey, plansza)==true)
        koniec=1;
        cout  << czas_gry() << "\r";
    }
    if(koniec==1)
    {
        system("cls");
        cout << "Gra zakonczona, wygrales!" << endl << endl;
        pokaz_plansze_puste(sizex, sizey, plansza);
        Sleep(6000);
    }
    if(koniec==2)
    {
        system("cls");
        cout << "Przykro mi ale trafiles na mine, przegrana!"<< endl << endl;
        pokaz_plansze_puste(sizex, sizey, plansza);
        Sleep(6000);
    }
    return 0;
}

///////////////////////////////////////////////////////////////

void odkryj_plansze(int x, int y, int sizex, int sizey, pole * plansza)
{
    if (x<0 || x>sizex-1) return; 
    if (y<0 || y>sizey-1) return; 
    if (plansza[x][y].odkryte==true) return;  

    if(plansza[x][y].wartosc!=9 && plansza[x][y].odkryte==false)
        plansza[x][y].odkryte=true;
        
    if (plansza[x][y].wartosc!=0) return;

    odkryj_plansze(x-1,y-1, sizex, sizey,plansza);
    odkryj_plansze(x-1,y, sizex, sizey, plansza);
    odkryj_plansze(x-1,y+1, sizex, sizey, plansza);
    odkryj_plansze(x+1,y-1, sizex, sizey, plansza);
    odkryj_plansze(x+1,y, sizex, sizey, plansza);
    odkryj_plansze(x+1,y+1, sizex, sizey, plansza);
    odkryj_plansze(x,y-1, sizex, sizey, plansza);
    odkryj_plansze(x,y, sizex, sizey, plansza);
    odkryj_plansze(x,y+1, sizex, sizey, plansza);
}

//////////////////////////////////////////////////////////////////

void pokaz_plansze(int sizex, int sizey, pole * plansza, int bombs)
{
    system ("cls"); 
    char c='0'-13, f='0'+22;
    ilosc_flag=0;
    for (int i = 0; i<sizex; i++)
    {
        for (int j = 0; j<sizey; j++)
        {
            if (i==lvlx && j==lvly) 
            {
                SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE  ), 0x02);
                cout << c;
            }
            else
            {
                SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE  ), 0x07);
                if (plansza[i][j].odkryte==true) 
                {
                    if (plansza[i][j].wartosc==0)
                        cout << " ";                
                    else
                        cout << plansza[i][j].wartosc;

                }
                if (plansza[i][j].odkryte==false)
                { 
                    if(plansza[i][j].flaga==false)
                        cout << c;
                    if(plansza[i][j].flaga==true)
                        cout<< f; 
                }
            }
        }
        SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), 0x07 );
        cout << endl;
    }
    
    for(int i=0; i<sizex; i++)
    {
        for(int j=0; j<sizey; j++)
        {
           if(plansza[i][j].flaga==true)
                ilosc_flag++;
        }
    }

    cout << "\npozycja kursora:\n"; 
    cout << "X: " << lvly << endl;  
    cout << "Y: " << lvlx << endl;
    cout << "Pozostalo: " << bombs-ilosc_flag << " bomb" << endl; 
    cout << "Czas Gry:   " << endl;
}

/////////////////////////////////////////////////////////////

void sterowanie(int bombs, int sizex, int sizey, pole * plansza)
{
    if (GetKeyState(enter) & 0x8000)
        {
            if (plansza[lvlx][lvly].wartosc==9)
                koniec=2;
            odkryj_plansze(lvlx, lvly, sizex, sizey, plansza);
            pokaz_plansze(sizex, sizey, plansza, bombs);
        }
    if(GetKeyState(f_key) & 0x8000)
    {
        if(plansza[lvlx][lvly].flaga==false && plansza[lvlx][lvly].odkryte==false){
            plansza[lvlx][lvly].flaga=true;
        }
        else if(plansza[lvlx][lvly].flaga==true && plansza[lvlx][lvly].odkryte==false){
            plansza[lvlx][lvly].flaga=false;
        }
        pokaz_plansze(sizex, sizey, plansza, bombs);
    }

    if ((GetKeyState(strzalka_prawo) & 0x8000) && lvly<sizey-1) lvly++;
    if ((GetKeyState(strzalka_lewo) & 0x8000) && lvly>0) lvly--;
    if ((GetKeyState(strzalka_dol) & 0x8000) && lvlx<sizex-1) lvlx++;
    if ((GetKeyState(strzalka_gora) & 0x8000) && lvlx>0) lvlx--;

    if (o_lvly==lvly && o_lvlx==lvlx) return;

    o_lvly = lvly;
    o_lvlx = lvlx;

    pokaz_plansze(sizex, sizey, plansza, bombs);
}

////////////////////////////////////////////////////////////

bool setmine(int x, int y, int sizex, int sizey, pole * plansza)
{
    if(plansza[x][y].wartosc!=9)
    {
        plansza[x][y].wartosc=9;
        for(int k=-1; k<2; k++)
        {
            for(int l=-1; l<2; l++)
            {
                if((x+k)<0 || (y+l)<0) 
                continue;
                if((x+k)>sizex-1 || (y+l)>sizey-1)
                continue;
                if(plansza[x+k][y+l].wartosc==9)
                continue;
                plansza[x+k][y+l].wartosc+=1;
            }
        }
    }
    return true;
} 

////////////////////////////////////////////////////////////

void mina(int miny, int sizex, int sizey, pole * plansza)
{
    srand( time(NULL) ); 
    while(miny>0)
    {
        int x=rand()%sizex;
        int y=rand()%sizey;
        
        if (plansza[x][y].wartosc!=9)
        {
            setmine(x, y, sizex, sizey, plansza);
            miny--;
        }
    } 
}

////////////////////////////////////////////////////////////

bool checkwin(int minesnum, int sizex, int sizey, pole * plansza)
{
    int miny = 0;
    for (int i = 0; i<sizex; i++)
    {
        for (int j = 0; j<sizey; j++)
        {
            if(plansza[i][j].odkryte==false)
            miny++;
        }
    }
    if (miny==minesnum) return true;
    return false;
}

////////////////////////////////////////////////////////////

bool rob_plansze(int sizex, int sizey, pole * plansza)
{
    for(int i=0; i<sizex; i++)
        for(int j=0; j<sizey; j++)
        {
            plansza[i][j].wartosc=0;
            plansza[i][j].odkryte=false;
            plansza[i][j].flaga=false;
        }
    return true;
}
            
/////////////////////////////////////////////////////////////

void pokaz_plansze_puste(int sizex, int sizey, pole * plansza)
{

char bomba= '0'+18;

for (int i = 0; i<sizex; i++)
    {
        for (int j = 0; j<sizey; j++)
        {
                if (plansza[i][j].wartosc!=9) 
                {
                    if (plansza[i][j].wartosc==0)
                        cout << " ";               
                    else
                        cout << plansza[i][j].wartosc; 

                }
                else 
                 cout << bomba;
                
        }
        cout << endl;
    }
}

///////////////////////////////////////////////////////////////////

double czas_gry()
{
    time_t now=time(0);
    double gametime= difftime(now, start);
    return(gametime);
}
