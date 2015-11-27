#include<stdio.h>
#include<conio.h>
#include <GL/glut.h>
#include <windows.h>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
using namespace std;
int notInDanger(int);
int checkMove(int);
const int width = 100;
const int height = 100;
const int Ratio = 10;
char lastPressed;
int tailLength = 1;
int currentMove = 1;
int score = 0;
int color;
int MAT[width][height];
bool gameOver = false;
bool failed = false;
int GreedySnake();
int lastMove; // 1 = Greedy ; 2 == Optim
struct position
{
    int x;
    int y;
} head , food , tail[5000];
int startTime;
int prevTime;
void displayScore ()
{
    char *c = "Score: ";
    char *score_text;
    itoa(score,score_text,10);
    glColor3f(0.0, 0.5 , 1.0);
    glRasterPos2f(20 , height-2);
    for(int i = 0 ; i < strlen(c) ; ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c[i]);
    for(int i = 0 ; i < strlen(score_text); ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, score_text[i]);
}
char* activeFunction;
void GenerateRandomFruitPos ()
{
    char x[100];
    char y[100];
    food.x = rand() % (width);
    food.y = rand() % (height);
    food.x /=10;
    food.x *=10;
    food.y /=10;
    food.y *=10;
    if(food.x <= 10)
        food.x+=20;
    if(food.y <= 10)
        food.y+=20;
    if(food.x >= width-10)
        food.x -= 20;
    if(food.y >= height-10)
        food.y -=20;
    for(int k = 0 ; k <= tailLength ; ++k)
        if(food.x == tail[k].x && food.y == tail[k].y)
            GenerateRandomFruitPos();
    itoa(food.x, x , 10);
    itoa(food.y, y , 10);
    printf("\nFood: %s , %s\n",x,y);
    if(food.x <= 10)
        food.x += 10 - food.x + 1;
    if(food.y <= 10)
        food.y += 10 - food.y + 1;
}
void displayEdges ()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(Ratio);
    glBegin(GL_LINES);
    glVertex2i(10,10);
    glVertex2i(width-10,10);
    glVertex2i(10,height-10);
    glVertex2i(width-10,height-10);
    glVertex2i(10,10);
    glVertex2i(10,height-10);
    glVertex2i(width-10,height-10);
    glVertex2i(width-10,10);
    glutPostRedisplay();
    glEnd();
    //glFlush();
}
void showSnake ()
{
    if(color == 1)
        glColor3f(1.0,0.0,0.0);
    if(color ==2)
        glColor3f(0.0,1.0,0.0);
    glBegin(GL_POINTS);
    int i,j;
    for(i = 0 ; i < width ; ++ i)
        for(j = 0 ; j < height ; ++j)
            for(int k = 0 ; k <= tailLength ; ++k)
                if (tail[k].x == i && tail[k].y == j)
                    glVertex2f(tail[k].x,tail[k].y);
    glEnd();
    //glFlush();
    //glutPostRedisplay();
}
void displayCB()
{
    //displayScore();
    displayEdges();
    glPointSize(Ratio);
    glBegin(GL_POINTS);
    glColor3f(255, 255 , 0);
    glVertex2f(food.x,food.y);
    glColor3f(0.0,1.0,0.0);
    //glutPostRedisplay();
    glEnd();
    showSnake();
    displayScore();
    glFlush();
    glutPostRedisplay();
}
void MoveUp ()
{
    lastPressed = 'w';
    ++tail[0].y;
}
void MoveDown ()
{
    lastPressed = 's';
    --tail[0].y;
}
void MoveLeft ()
{
    lastPressed = 'a';
    --tail[0].x;
}
void MoveRight ()
{
    lastPressed = 'd';
    ++tail[0].x;
}
int foodAhead ()
{
    switch(lastPressed)
    {
    case 'w':
        return (food.y > tail[0].y && food.x == tail[0].x);
    case 's':
        return (food.y < tail[0].y && food.x == tail[0].x);
    case 'a':
        return (food.x < tail[0].x && food.y == tail[0].y);
    case 'd':
        return (food.x > tail[0].x && food.y == tail[0].y);
    }
}
int snakeAhead (int Move)
{
    int k;
    switch(Move)
    {
    case 1:
        for(k = 1 ; k <= tailLength ; ++ k)
            if(tail[0].y + Ratio == tail[k].y && tail[0].x == tail[k].x)
                return 0;
        return 1;
    case 2:
        for(k = 1 ; k <= tailLength ; ++ k)
            if(tail[0].y - Ratio == tail[k].y && tail[0].x == tail[k].x)
                return 0;
        return 1;
    case 3:
        for(k = 1 ; k <= tailLength ; ++ k)
            if(tail[0].x - Ratio == tail[k].x && tail[0].y == tail[k].y)
                return 0;
        return 1;
    case 4:
        for(k = 1 ; k <= tailLength ; ++ k)
            if(tail[0].x + Ratio == tail[k].x && tail[0].y == tail[k].y)
                return 0;
        return 1;
    }
}
int wallAhead (int Move)
{
    /*
            1   = UP    w
            2   = DOWN  s
            3   = LEFT  a
            4   = RIGHT d
    */
    switch(Move)
    {
    case 1:
        return (tail[0].y + Ratio == height - 20);
    case 2:
        return (tail[0].y - Ratio == 20);
    case 3:
        return (tail[0].x - Ratio == 20);
    case 4:
        return (tail[0].x + Ratio == width - 20);
    }
}
int isGoingBack (int Move)
{
    if(Move == 1 && lastPressed == 's')
        return 0;
    if(Move == 2 && lastPressed == 'w')
        return 0;
    if(Move == 3 && lastPressed == 'd')
        return 0;
    if(Move == 4 && lastPressed == 'a')
        return 0;
    return 1;
}
int dangerAhead ()
{
    switch(lastPressed)
    {
    case 'w':
        return snakeAhead(1) * wallAhead(1) * isGoingBack(1);
        break;
    case 's':
        return snakeAhead(2) * wallAhead(2) * isGoingBack(2);
        break;
    case 'a':
        return snakeAhead(3) * wallAhead(3) * isGoingBack(3);
        break;
    case 'd':
        return snakeAhead(4) * wallAhead(4) * isGoingBack(4);
        break;
    }
}
int dangerRight ()
{
    int k;
    for(k = 1 ; k <= tailLength ; ++k)
        if(tail[0].x + Ratio == tail[k].x && tail[0].y == tail[k].y)
            return 0;
    return 1;
}
int dangerLeft ()
{
    int k;
    for(k = 1 ; k <= tailLength ; ++k)
        if(tail[0].x - Ratio == tail[k].x && tail[0].y == tail[k].y)
            return 0;
    return 1;
}
int dangerUp ()
{
    int k;
    for(k = 1 ; k <= tailLength ; ++k)
        if(tail[0].x == tail[k].x && tail[0].y + Ratio == tail[k].y)
            return 0;
    return 1*wallAhead(1);
}
int dangerDown ()
{
    int k;
    for(k = 1 ; k <= tailLength ; ++k)
        if(tail[0].x == tail[k].x && tail[0].y - Ratio == tail[k].y)
            return 0;
    return 1*wallAhead(2);
}
void keepMoving ()
{
    switch(lastPressed)
    {
    case 'w':
        MoveUp();
        break;
    case 's':
        MoveDown();
        break;
    case 'a':
        MoveLeft();
        break;
    case 'd':
        MoveRight();
        break;

    }

}
int AI_Logic ()
{
    if(foodAhead())
    {
        printf("Am mancare in fata\n");
        if(dangerAhead())
        {
            if(!dangerRight())
                MoveLeft();
            else if(!dangerLeft())
                MoveRight();
        }
        printf("Nu am obstacol in fata, merg in continuare\n");
        keepMoving();

    }
    else
    {
        printf("Nu am mancare in fata\n");
        if(tail[0].x < food.x)
        {
            if(!dangerRight())
            {
                if(!dangerLeft())
                {
                    if(!dangerUp())
                        MoveDown();
                    else MoveDown();
                }
            }
            else MoveRight();
        }
        else if (tail[0].x > food.x)
        {
            if(!dangerLeft())
            {
                if(!dangerRight())
                {
                    if(!dangerUp())
                        MoveDown();
                    else MoveUp();
                }
            }
            else MoveLeft();
        }
        else if (tail[0].x == food.x && tail[0].y < food.y)
        {
            if(!dangerUp)
            {
                if(!dangerDown())
                {
                    if(!dangerLeft())
                        MoveRight();
                    else MoveLeft();
                }
                else MoveDown();
            }
            else MoveUp();
        }
    else MoveDown();
    }
}
void Idle_AI ()
{
    AI_Logic();
    if(tail[0].x == food.x && tail[0].y == food.y)
    {
        GenerateRandomFruitPos();
        ++tailLength;
        score += 10;
    }
    position last,last2;
    int i;
    last.x = tail[0].x;
    last.y = tail[0].y;
    tail[1].x = tail[0].x;
    tail[1].y = tail[0].y;
    for(i = 1; i <= tailLength; ++i)
    {
        last2.x = tail[i].x;
        last2.y = tail[i].y;
        tail[i]. x =last.x;
        tail[i].y = last.y;
        last.x = last2.x;
        last.y = last2.y;
    }
    Sleep(10);
    glFlush();
}
int main(int argc, char *argv[])
{
    char key;
    srand(time(NULL));
    glutInit(&argc, argv);
    glMatrixMode(GL_PROJECTION);
    glutInitDisplayMode(GLUT_RGB);
    tail[0].x = width / 2;
    tail[0].y = height /2;
    GenerateRandomFruitPos();
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN)-width)*3/4,
                           (GetSystemMetrics(SM_CYSCREEN)-height)/2);
    glutCreateWindow("Snake Game");
    glutInitWindowSize(width,height);
    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0,width,0,height+50);
    glutDisplayFunc(displayCB);
    char *s = argv[1];
    glutIdleFunc(Idle_AI);
    glutMainLoop();
    return 0;
}
