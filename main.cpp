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
const int width = 320;
const int height = 240;
const int Ratio = 10;
int tailLength = 1;
bool gameOver = false;
struct position
{
    int x;
    int y;
} head , food , tail[5000];
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
    glEnd();
    glFlush();
}
void showSnake ()
{
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_POINTS);
    int i,j;
    for(i = 0 ; i < width ; ++ i)
        for(j = 0 ; j < height ; ++j)
            for(int k = 0 ; k <= tailLength ; ++k)
                if (tail[k].x == i && tail[k].y == j)
                    glVertex2f(tail[k].x,tail[k].y);
    glEnd();
    glFlush();
    glutPostRedisplay();
}
void displayCB()
{
    displayEdges();
    glPointSize(Ratio);
    glBegin(GL_POINTS);
    glColor3f(255, 255 , 0);
    glVertex2f(food.x,food.y);
    glColor3f(0.0,1.0,0.0);
    glEnd();
    showSnake();
    glFlush();
    glutPostRedisplay();
}
char lastPressed;
void Move ()
{
    switch(lastPressed)
    {
    case 'w':
        ++tail[0].y;
        break;
    case 's':
        --tail[0].y;
        break;
    case 'a':
        --tail[0].x;
        break;
    case 'd':
        ++tail[0].x;
        break;
    }
}
bool inBox()
{
    return (tail[0].x > 10 && tail[0].x < width-10 && tail[0].y > 10 && tail[0].y < height-10);
}
bool doesntEatItself (int posx, int posy)
{
    for(int k = 1 ; k <= tailLength ; ++k)
        if(tail[k].x == posx && tail[k].y == posy)
            return false;
    return true;
}
void myIdleFunc ()
{
    char x[100];
    char y[100];
    /*itoa(head.x,x,10);
    itoa(head.y,y,10);
    //printf("\n(%s , %s)\n",x,y);*/
    if(inBox() == false)
        exit(0);
    Sleep(Ratio*3*width/height);
    int posx,posy;
    posx = tail[0].x;
    posy = tail[0].y;
    switch(lastPressed)
    {
    case 'w':
        posy+=Ratio;
        if(doesntEatItself(posx,posy))
            tail[0].y+=Ratio;
        else exit(0);
        break;
    case 's':
        posy -= Ratio;
        if(doesntEatItself(posx,posy))
            tail[0].y-=Ratio;
        else exit(0);
        break;
    case 'd':
        posx += Ratio;
        if(doesntEatItself(posx,posy))
            tail[0].x+=Ratio;
        else exit(0);
        break;
    case 'a':
        posx -= Ratio;
        if(doesntEatItself(posx,posy))
            tail[0].x-=Ratio;
        else exit(0);
        break;
    }
    if(tail[0].x == food.x && tail[0].y == food.y)
    {
        GenerateRandomFruitPos();
        ++tailLength;
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
    glutPostRedisplay();
}
void keyCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'w':
        lastPressed = 'w';
        break;
    case 's':
        lastPressed = 's';
        break;
    case 'a':
        lastPressed = 'a';
        break;
    case 'd':
        lastPressed = 'd';
        break;
    case 'x':
        exit(0);
        break;
    }
}
int Max (int a , int b , int c , int d)
{
    if(a > b && a > c && a> d)
        return a;
    if (b > a && b > c && b > d)
        return b;
    if (c > a && c > b && c > d)
        return c;
    return d;
}
int willBeInBox(int posx, int posy)
{
    return (posx > 10 && posx < width-10 && posy > 10 && posy < height-10);
}
void MoveAI ()
{
    int posx,posy;
    posx = tail[0].x;
    posy = tail[0].y;
    switch(lastPressed)
    {
    case 'w':
        if(doesntEatItself(posx,posy+Ratio))
        {
            tail[0].y += Ratio;
            break;
        }
    case 's':
        if(doesntEatItself(posx,posy-Ratio))
        {
            tail[0].y -= Ratio;
            break;
        }
    case 'a':
        if(doesntEatItself(posx-Ratio,posy))
        {
            tail[0].x -= Ratio;
            break;
        }
    case 'd':
        if(doesntEatItself(posx+Ratio,posy))
        {
            tail[0].x += Ratio;
            break;
        }
    }
}
int make_decision ()
{
    int posx , posy;
    posx = tail[0].x;
    posy = tail[0].y;
    if(tail[0].x < food.x)
    {
        if(tail[0].x + Ratio > width - 10)
            if(tail[0].y + Ratio < height - 10)
                lastPressed = 'w';
            else lastPressed = 's';
        else lastPressed = 'd';
        return 1;
    }
    if(tail[0].x > food.x)
    {
        if(tail[0].x - Ratio < 10)
            if(tail[0].y + Ratio < height - 10)
                lastPressed = 'w';
            else lastPressed = 's';
        else lastPressed = 'a';
        return 1;
    }
    if(tail[0].x == food.x && tail[0].y < food.y)
    {
        if(tail[0].y + Ratio > height - 10 )
            if(tail[0].x + Ratio < width - 10)
                lastPressed = 'd';
            else lastPressed = 'a';
        lastPressed = 'w';
        return 1;
    }
    if(tail[0].x == food.x && tail[0].y > food.y)
    {
        if(tail[0].y - Ratio < 10 )
            if(tail[0].x + Ratio < width - 10)
                lastPressed = 'd';
            else lastPressed = 'a';
        lastPressed = 's';
        return 1;
    }

}
void Idle_AI ()
{
    make_decision();
    MoveAI();
    Sleep(10);  
    if(tail[0].x == food.x && tail[0].y == food.y)
    {
        GenerateRandomFruitPos();
        ++tailLength;
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
    glutInitWindowSize(width,height);
    glutCreateWindow("Snake Game");
    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0,width,0,height);
    glutDisplayFunc(displayCB);
    //glutKeyboardFunc(keyCB);
    glutIdleFunc(Idle_AI);
    glutMainLoop();
    return 0;
}
