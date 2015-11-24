#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <cmath>
#include <windows.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
//#include "neural_net.cpp"
std :: ofstream g ("date.out");
bool gameOver;
const int width = 20;
const int height = 20;
int MAT[width][height];
struct pos
{
    int x;
    int y;
} head,  fruit, tail[200];
int score = 0 , tailLength = 0;
enum direction {STOP = 0 , LEFT , RIGHT, UP , DOWN};
direction dir;
void GenerateRandomFruitPosition()
{
    fruit.x = rand() % (width-1);
    fruit.y = rand() % (height-1);
    if(fruit.x == 0)
        fruit.x+=2;
    if(fruit.x == 1)
        ++fruit.x;
    if(fruit.y == 0)
        fruit.y+=2;
    if(fruit.y == 1)
        ++fruit.y;
    g<<fruit.x<<" "<<fruit.y<<"\n";
}
void Draw ()
{
    system("cls");
    int i , j;
    for(i = 0 ; i < width ; ++i)
        printf("#");
    printf("\n");
    for(i = 0 ; i < height ; ++i)
    {
        for(j = 0 ; j < width ; ++j)
        {
            if(j == 0 || j == width-1)
                printf("#");
            else if ( j == head.x && i == head.y )
            {
                printf("0");
                MAT[i][j] = 1;
            }
            else if (j == fruit.x && i == fruit.y)
            {
                printf("X");
                MAT[i][j] = 2;
            }
            else
            {
                bool print = false;
                for(int k = 0 ; k < tailLength ; ++k)
                {
                    if (tail[k].x == j && tail[k].y == i)
                    {
                        printf("o");
                        print = true;
                    }
                }
                if(!print)
                {
                    printf(" ");
                    MAT[i][j] = 0;
                }
            }
        }
        printf("\n");
    }
    for(i = 0 ; i < width ; ++i)
        printf("#");
    //printf("\nScore: %d",score);
    printf("\n\n\n");

}
void MainStart ()
{
    gameOver = false;
    dir = STOP;
    head.x = width / 2;
    head.y = height / 2;
    GenerateRandomFruitPosition();
}
void GetUserInput ()
{
    if( _kbhit() )
    {
        switch(_getch())
        {
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'x':
            gameOver = true;
            break;
        }
    }
}
void Logic ()
{
    pos last,last2;
    int i;
    last.x = tail[0].x;
    last.y = tail[0].y;
    tail[0].x = head.x;
    tail[0].y = head.y;
    for(i = 1; i <= tailLength; ++i)
    {
        last2.x = tail[i].x;
        last2.y = tail[i].y;
        tail[i]. x =last.x;
        tail[i].y = last.y;
        last.x = last2.x;
        last.y = last2.y;
    }
    switch(dir)
    {
    case LEFT:
        --head.x;
        break;
    case RIGHT:
        ++head.x;
        break;
    case UP:
        --head.y;
        break;
    case DOWN:
        ++head.y;
    default:
        break;
    }
    if (head . x < 0 || head.x > width || head.y < 0 || head.y > height)
        gameOver = true;
    else if (head.x == fruit.x && head.y == fruit.y)
    {
        score+=10;
        ++tailLength;
        GenerateRandomFruitPosition();
    }
    for(i = 0 ; i < tailLength ; ++i)
        if(tail[i].x == head.x && tail[i].y == head.y)
            gameOver = true;
}
bool isOnTable (int posx, int posy)
{
    return (posx > 0 && posx < width && posy > 0 && posy < height);
}
pos prev;
int AI ()
{
    if(gameOver == true)
        return 0;
    /*else
    {
        if()
    }*/
}
void display ()
{
 //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer (background)
   glFlush();
}
int main (int argc , char* argv[])
{
    glutInit(&argc,argv);
    glutCreateWindow("Snake Game");
    glutInitWindowSize(640,480);
    glutInitWindowPosition(50, 50);
    glutDisplayFunc(display);
    glutMainLoop();
    srand(time(NULL));
    //SetWindow(width,height+1);
    MainStart();
    while( !gameOver )
    {
        Draw();
        GetUserInput();
        //AI();
        Logic();
        Sleep(10);
    }
}
