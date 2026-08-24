#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>

#define MAXLETTERS 5

typedef struct{
    char *letters;
}words;

void create_buffer(int initial_capacity, words *wordtyped) {
    wordtyped->letters = malloc(initial_capacity * sizeof(char));
}

void free_buffer(words *wordtyped) {
    free(wordtyped->letters);
    free(wordtyped);
}

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
void * keycheck(HANDLE hStdOut, words* wordtyped)
{
    char keyprev = 0, keynew = 0;
    static int xcoord = 0, ycoord = 0;
    int done = 0;
    //char* wordtyped = (char*)malloc(5*sizeof(char));
    while(done == 0)
    {
        keyprev = keynew; // a w
        keynew = key_press(); // maybe put the esc statement here
        //printf("%c", &keynew);
        if(keyprev != keynew)
        {
            keyprev = keynew; //w a
            //if(keynew > 64 && keynew < 91)
            if(xcoord < 5)
            {

                if(keyprev > 64 && keyprev < 91)
                {
                    //write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
                    write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    wordtyped->letters[xcoord] = keyprev;
                    xcoord++;
                }
            }

            if(keyprev == 8)
            {
                keyprev = ' '; //3
                xcoord--;
                //write_symbol_in_color(hStdOut, xcoord, ycoord, &keynew, FOREGROUND_RED | FOREGROUND_INTENSITY); // 3 = ' '
                write_symbol_in_color(hStdOut, xcoord, ycoord, &keyprev, FOREGROUND_RED | FOREGROUND_INTENSITY ); // 3 = ' '
                //xcoord--;
                if(xcoord < 0)
                {
                    xcoord = 0;
                }
                //wordtyped[xcoord] = keyprev;
                wordtyped->letters[xcoord] = keyprev;
            }
            else if(keyprev == 0x0D && xcoord >=5)
            {
                xcoord = 0;
                ycoord++;
                done++;
            }

        }
        //sleep(1);
    }
    done = 0;
    //return wordtyped;

}

int initWordleGame(int maxwords, int maxletters)
{
    return 0;
}

int main()
{
    int stop = 0;
    int maxwords = 5;
    int wordsize = 5;
    words wordtyped[maxwords];
    for(int cont = 0; cont< maxwords; cont++)
    {
        create_buffer(wordsize, &wordtyped[cont]);
    }
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    for(int i = 0; i<2; i++)
    {
        keycheck(hStdOut, &wordtyped[i]);
        Sleep(5);
    }
    for(int cont = 0; cont < 5; cont++){
        write_symbol_in_color(hStdOut, cont, 10, &wordtyped[0].letters[cont], FOREGROUND_GREEN| FOREGROUND_INTENSITY);
    }
    for(int cont = 0; cont < 5; cont++){
        write_symbol_in_color(hStdOut, cont, 11, &wordtyped[1].letters[cont], FOREGROUND_GREEN| FOREGROUND_INTENSITY);
    }

    Sleep(2);
    //free(wordtyped[0]->letters);
    //free_buffer(wordtyped->letters);
    for(int cont = 0; cont < 5; cont++)
    {
        free(wordtyped[cont].letters);
    }
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    system("pause");
    return 1;
}
