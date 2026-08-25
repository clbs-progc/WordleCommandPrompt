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
void rngtestprint(HANDLE h, SHORT x, SHORT y, const char* symbol, WORD color)
{
    DWORD length = (DWORD)strlen(symbol);
    COORD here;
    here.X = x;
    here.Y = y;

    WORD attribute = color;
    DWORD written = 0;
    //WriteConsoleOutputAttribute(h, &attribute, length, here, &written);
    WriteConsoleOutputCharacterA(h, symbol, length, here, &written);
}

int rng(int seed, unsigned int range)
{
    static int contrng = 1;
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
    int max = 1000000;
    int maxnumb = 10;
    int percent[maxnumb];
    int random;
    char a[10];
    char stringtext[max];
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
        rngtestprint(hStdOut, 12, 5+random, stringtext, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        snprintf(stringtext, sizeof(stringtext), "%i", random);
        rngtestprint(hStdOut, 12, 16, stringtext, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    }
    for(int cont = 0; cont < maxnumb; cont++)
    {
        a2 = (float) percent[cont]/max;
        printf(" %i: %.4f ", cont, a2);
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
    //return 0;
}
void * keycheck(HANDLE hStdOut, char *wordtyped, int ycoord)
{
    char keyprev = 0, keynew = 0;
    static int xcoord = 0;
    int done = 0;
    while(done == 0)
    {
        keyprev = keynew; // a w
        keynew = key_press(); // maybe put the esc statement here
        if(keyprev != keynew)
        {
            keyprev = keynew; //w a
            //if(keynew > 64 && keynew < 91)
            if(xcoord < 5)
            {

                if(keyprev > 64 && keyprev < 91)
                {
                    //write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                    write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    wordtyped[xcoord] = keyprev;
                    xcoord++;
                }
            }

            if(keyprev == 8)
            {
                keyprev = ' '; //3
                xcoord--;
                //write_symbol_in_color(hStdOut, xcoord, ycoord, &keynew, FOREGROUND_RED | FOREGROUND_INTENSITY); // 3 = ' '
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
    }
    done = 0;
}
void pullWord(char ReturnWordArray[][5])
{
    char word[6];
    char pulledword[6];
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
int wordlerun(HANDLE hStdOut , char *wordpulled, char *wordtyped, int ycoord)
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
void emptyScreen(HANDLE hStdOut, int option, int size, int ycoord)
{
    switch(option)
    {
        case 0: //Word not exist
            for(int cont = 0; cont < size; cont++)
            {
                write_symbol_in_color(hStdOut, cont + 10, ycoord, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            }
            break;
        case 1://wordtyped
            for(int cont = size-1; cont >= 0; cont--)
            {
                write_symbol_in_color(hStdOut, cont, ycoord, " ", FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                usleep(1000);
            }
            break;
    }
}

int main()
{
    int stop = 0;
    int maxwords = 5;
    int wordsize = 5;
    char wordpulled[5];
    char wordtyped[5];
    int ycoord = 0;
    int runcheck = 0;
    char ListWords[14855][5];
    int randomnumber = rng(time(&t), 14855);
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    pullWord(ListWords);
    for(int cont = 0; cont < 5; cont++)
    {
        wordpulled[cont] = ListWords[randomnumber][cont];
        //write_symbol_in_color(hStdOut, cont + 10, 10, &wordpulled[cont], FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    //rngTest(hStdOut);
    char Warn[100] = "Word doesn't exist!"; //19
    while(ycoord < 6)
    {
        keycheck(hStdOut, wordtyped, ycoord);
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
                emptyScreen(hStdOut, 1, MAXLETTERS, ycoord);
            }
            keycheck(hStdOut, wordtyped, ycoord);
            runcheck = wordExist(wordtyped, ListWords);
            emptyScreen(hStdOut, 0, 19, ycoord);
        }
        if(wordlerun(hStdOut, wordpulled, wordtyped, ycoord) != 5)
        {
            ycoord++;
        }
        else
        {
            ycoord = 6;
        }
    }

    Sleep(2);
    system("pause");
    return 1;
}
