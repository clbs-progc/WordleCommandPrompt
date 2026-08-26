#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

#define MAXLETTERS 5
time_t t;

void write_symbol_in_color(HANDLE h, SHORT x, SHORT y, const char* symbol, WORD color)
{
    COORD here;
    here.X = x;
    here.Y = y;

    WORD attribute = color;
    DWORD written = 0;
    WriteConsoleOutputAttribute(h, &attribute, 1, here, &written);
    WriteConsoleOutputCharacterA(h, symbol, 1, here, &written);
}

void stringprint(HANDLE h, SHORT x, SHORT y, const char* symbol, int set, WORD color)
{
    DWORD length = (DWORD)strlen(symbol);
    COORD here;
    COORD here2;
    here.X = x;
    here2.X = x - length;
    here.Y = y;
    here2.Y = y;
    WORD attribute = color;
    DWORD written = 0;
    //DWORD written2 = 0;
    if(set != 0)
    {
        WriteConsoleOutputAttribute(h, &attribute, length, here2, &written);
    }
    else
    {
        here2.X = 0;
        here2.Y = 0;
        WriteConsoleOutputAttribute(h, &attribute, length, here2, &written);
    }
    WriteConsoleOutputCharacterA(h, symbol, length, here, &written);
}

void EmptyScreen(HANDLE hStdOut)
{
    for(int y = 0; y < 20; y++)
    {
        for(int x = 0; x < 100; x++)
        {
            write_symbol_in_color(hStdOut, x, y, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        }
        Sleep(50);
    }
}

int contrng = 1;
int rng(int seed, unsigned int range)
{
    int RandomGen1;
    srand(contrng);
    int g = rand();
    srand(seed);
    int g2 = rand();
    contrng = contrng+g2;
    g = g + g2;
    srand(g);
    if(range > 0)
    {
        RandomGen1 = rand() % range;
    }
    else
    {
        RandomGen1 = rand();
    }
    contrng++;
    if(contrng > 1000000)
    {
        contrng = 1;
    }
    return RandomGen1;
}


void rngTest(HANDLE hStdOut)
{
    int max = 100000;
    int maxnumb = 10;
    int percent[maxnumb];
    int random;
    char a[10];
    char stringtext[10];
    //char *ptr = &a;
    int contprint = 0;
    float a2;
    for(int cont = 0; cont < maxnumb; cont++)
    {
        percent[cont] = 0;
        a[cont] = cont + 48;
        write_symbol_in_color(hStdOut, 10, 5+cont, &a[cont], FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        contprint = contprint + 2;
    }
    contprint = 0;
    for(int cont = 0; cont < max; cont++)
    {
        random = rng(cont, maxnumb);
        percent[random] = percent[random] + 1;
        snprintf(stringtext, sizeof(stringtext), "%i", percent[random]);
        stringprint(hStdOut, 12, 5+random, stringtext, 0, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        snprintf(stringtext, sizeof(stringtext), "%i", random);
        stringprint(hStdOut, 12, 16, stringtext, 0, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        snprintf(stringtext, sizeof(stringtext), "%i", cont);
        stringprint(hStdOut, 12, 4, stringtext, 0, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        //Sleep(500);
    }
    for(int cont = 0; cont < maxnumb; cont++)
    {
        a2 = (float) percent[cont]/max;
        snprintf(stringtext, sizeof(stringtext), "%.4f", a2);
        stringprint(hStdOut, 22, 5+cont, stringtext, 0, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
}

int key_press()
{
    for (int i = 8; i < 91; i++) // 65 to 91
    {
        if (GetAsyncKeyState(i))
        {
            //printf("%i\n", i);
            return i;
        }
    }
    return 0;
}

int keycheck(HANDLE hStdOut, char *wordtyped, int ycoord)
{
    char keyprev = 0, keynew = 0;
    int xcoord = 0;
    int done = 0;
    while(done == 0)
    {
        contrng++;
        keyprev = keynew;
        keynew = key_press();
        if(keyprev != keynew)
        {
            keyprev = keynew;
            if(keyprev == 27)
            {
                return 1;
            }
            if(xcoord < 5)
            {

                if(keyprev > 64 && keyprev < 91)
                {
                    wordtyped[xcoord] = keyprev;
                    write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    xcoord++;
                }
            }

            if(keyprev == 8)
            {
                keyprev = ' '; //3
                xcoord--;
                write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_INTENSITY ); // 3 = ' '
                if(xcoord < 0)
                {
                    xcoord = 0;
                }
                wordtyped[xcoord] = keyprev;
            }
            else if(keyprev == 0x0D && xcoord >=5)
            {
                xcoord = 0;
                ycoord++;
                done++;
            }

        }
        Sleep(20);

    }
    done = 0;
    return 0;
}
void LoadWordList(char ReturnWordArray[][5])
{
    char word[6];
    char *result;
    int line = 14855;
    FILE *p = fopen("words.txt","r");
    if(p == NULL)
    {
        printf("error while opening file\n");
    }
    for(int cont = 0; cont < line; cont++)
    {
        result = fgets(word, 7,p);
        for(int i = 0; i < 5; i++)
        {
            word[i] = word[i] - 32;
            ReturnWordArray[cont][i] = word[i];
        }
    }

    fclose(p);
}
int wordExist(char *wordtyped, char ListWords[][5])
{
    int value = 0;
    for(int i = 0; i < 14855; i++)
    {
        for(int cont = 0; cont < 5; cont++)
        {
            if(wordtyped[cont] == ListWords[i][cont])
            {
                value++;
            }
        }
        if(value == 5)
        {
            return 1;
        }
        value = 0;
    }

    return 0;
}
int WordleRunLogic(HANDLE hStdOut , char *wordpulled, char *wordtyped, int ycoord)
{
    int value = 0;
    for(int xcoord = 0; xcoord< MAXLETTERS; xcoord++)
    {
        if(wordtyped[xcoord] == wordpulled[xcoord])
        {
            write_symbol_in_color(hStdOut, xcoord, ycoord, &wordtyped[xcoord], FOREGROUND_GREEN| FOREGROUND_INTENSITY);
            value++;
        }
        else
        {
            for(int cont = 0; cont < MAXLETTERS; cont++)
            {
                if(wordtyped[xcoord] == wordpulled[cont])
                {
                    write_symbol_in_color(hStdOut, xcoord, ycoord, &wordtyped[xcoord], FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    break;
                }
                else
                {
                    write_symbol_in_color(hStdOut, xcoord, ycoord, &wordtyped[xcoord], FOREGROUND_RED | FOREGROUND_INTENSITY);
                }
            }
        }
    }
    return value;

}
int WordleRun(HANDLE hStdOut, char ListWords[][5])
{
    write_symbol_in_color(hStdOut, 0, 0, "W", FOREGROUND_GREEN | FOREGROUND_RED);
    write_symbol_in_color(hStdOut, 1, 0, "O", FOREGROUND_GREEN | FOREGROUND_RED);
    write_symbol_in_color(hStdOut, 2, 0, "R", FOREGROUND_GREEN | FOREGROUND_RED);
    write_symbol_in_color(hStdOut, 3, 0, "D", FOREGROUND_GREEN | FOREGROUND_RED);
    write_symbol_in_color(hStdOut, 4, 0, "L", FOREGROUND_GREEN | FOREGROUND_RED);
    write_symbol_in_color(hStdOut, 5, 0, "E", FOREGROUND_GREEN | FOREGROUND_RED);
    char wordpulled[5];
    char wordtyped[5];
    //char ListWords[14855][5];
    int ycoord = 2, runcheck = 0, rngrun = 1;
    int randomnumber = rng(time(&t), 14855);
    //int randomnumber = 10469;
    for(int cont = 0; cont < 5; cont++)
    {
        wordtyped[cont] = ' ';
        wordpulled[cont] = ListWords[randomnumber][cont];
        //write_symbol_in_color(hStdOut, cont + 10, 10, &wordpulled[cont], FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    while(rngrun == 1)
    {
        for(int ii = 0; ii < 4; ii++)
        {
            for(int i = ii + 1; i < 5; i++)
            {
                if(wordpulled[i] == wordpulled[ii])
                {
                    randomnumber = rng(time(&t), 14855);
                    for(int cont = 0; cont < 5; cont ++)
                    {
                        wordpulled[cont] = ListWords[randomnumber][cont];
                        //write_symbol_in_color(hStdOut, cont + 10, 10, &wordpulled[cont], FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                        ii = 0;
                        //break;
                    }
                }
                else
                {
                    rngrun = 0;
                }
            }
        }
    }
    //rngTest(hStdOut);
    char Warn[20] = "Word doesn't exist!"; //19
    while(ycoord < 8)
    {
        if(keycheck(hStdOut, wordtyped, ycoord) == 1)
        {
            ycoord = 10;
            runcheck = 10;
            break;
        }
        runcheck = wordExist(wordtyped, ListWords);
        while(runcheck != 1)
        {
            for(int cont = 0; cont < 19; cont++)
            {
                write_symbol_in_color(hStdOut, cont + 10, ycoord, &Warn[cont], FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                usleep(1000);
            }
            for(int cont = 0; cont < 5; cont++)
            {
                //emptyScreen(hStdOut, 1, MAXLETTERS, ycoord);
                for(int clear = MAXLETTERS - 1; clear >= 0; clear--)
                {
                    write_symbol_in_color(hStdOut, clear, ycoord, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    usleep(1000);
                }
                //write_symbol_in_color(hStdOut, cont, ycoord, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
            if(keycheck(hStdOut, wordtyped, ycoord) == 1)
            {
                ycoord = 10;
                runcheck = 10;
                break;
            }
            //keycheck(hStdOut, wordtyped, ycoord);
            runcheck = wordExist(wordtyped, ListWords);
            for(int clear = 0; clear < 19; clear++)
            {
                write_symbol_in_color(hStdOut, clear + 10, ycoord, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
        }
        switch(runcheck)
        {
            case 10:
                break;

            default:
                if(WordleRunLogic(hStdOut, wordpulled, wordtyped, ycoord) != 5)
                {
                    ycoord++;
                }
                else
                {
                    ycoord = 10;
                }
        }
    }
    sprintf(Warn , "Word was: %.5s", wordpulled);
    stringprint(hStdOut, 0, 8, Warn, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    sprintf(Warn , "Press ESC to Leave");
    stringprint(hStdOut, 0, 10, Warn, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    Sleep(500);
    while(1)
    {
        if(keycheck(hStdOut, wordtyped, ycoord) == 1)
        {
            stringprint(hStdOut, 0, 11, "EXITING", 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            Sleep(2000);
            return 0;
        }
    }

}
void MenuPrint(HANDLE hStdOut)
{
    char MenuChar[50] = "Welcome! Choose an Option:";
    stringprint(hStdOut, 0, 0, MenuChar, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    sprintf(MenuChar, "1: Wordle");
    stringprint(hStdOut, 0, 2, MenuChar, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    sprintf(MenuChar, "2: Dordle");
    stringprint(hStdOut, 0, 4, MenuChar, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    sprintf(MenuChar, "Press Esc to Leave");
    stringprint(hStdOut, 0, 6, MenuChar, 1, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}
int Menu(HANDLE hStdOut)
{
    char ListWords[14855][5];
    LoadWordList(ListWords);
    char keynew = 0, keyprev = 0, run = 1;
    keynew = key_press();
    while(1)
    {
        run = 1;
        //EmptyScreen(hStdOut);
        MenuPrint(hStdOut);
        keynew = key_press();
        while(run == 1)
        {
            keyprev = keynew;
            keynew = key_press();
            if(keyprev != keynew)
            {
                keyprev = keynew;
                switch(keyprev)
                {
                    case 27:
                        EmptyScreen(hStdOut);
                        return 0;
                        break;

                    case 49:
                        EmptyScreen(hStdOut);
                        WordleRun(hStdOut, ListWords);
                        EmptyScreen(hStdOut);
                        //keynew = 0; keyprev = 0;
                        run = 0;
                        break;
                    case 50:
                        break;

                }
            }
            Sleep(20);
        }
    }
    return 0;
}

int main()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Menu(hStdOut);

    return 1;
}
