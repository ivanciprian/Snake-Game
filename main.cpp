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
#define TIMERMSECS 33
using namespace std;
const int width = 320;
const int height = 240;
const int Ratio = 10;
int tailLength = 1;
int score = 0;
int color;
bool gameOver = false;
bool failed = false;
int lastMove; // 1 = Greedy ; 2 == Optim
struct position
{
    int x;
    int y;
} head , food , tail[5000];
int startTime;
int prevTime;
static void animate(int value)
{
    // Set up the next timer tick (do this first)
    glutTimerFunc(TIMERMSECS, animate, 0);

    // Measure the elapsed time
    int currTime = glutGet(GLUT_ELAPSED_TIME);
    int timeSincePrevFrame = currTime - prevTime;
    int elapsedTime = currTime - startTime;

    // ##### REPLACE WITH YOUR OWN GAME/APP MAIN CODE HERE #####

    // Rotate the triangle
    //rot = (ROTRATE / 1000) * elapsedTime;

    // ##### END OF GAME/APP MAIN CODE #####



    // Force a redisplay to render the new image
    glutPostRedisplay();

    prevTime = currTime;
}

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
int willBeInBox(int , int);
int moveOk (int , int);
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
    itoa(tail[0].x,x,10);
    itoa(tail[0].y,y,10);
    //printf("\n(%s , %s)\n",x,y);
    if(inBox() == false)
        exit(0);
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
    //glutPostRedisplay();
    Sleep(30);
    glFlush();
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
char steps[1000];
int k = 1;
char rev_step (char x)
{
    if(x == 'w')
        return 's';
    if(x == 's')
        return 'w';
    if(x == 'd')
        return 'a';
    if(x=='a')
        return 'd';
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
    return (posx >= 20 && posx <= width-20 && posy >= 20 && posy <= height-20);
}
int willNotBiteItSelf(int , int);
void getToFood();
void GreedySnake();
int MoveAI ()
{
    int posx,posy;
    posx = tail[0].x;
    posy = tail[0].y;
    printf("%d , %d \n",posx,posy);
    switch(lastPressed)
    {
    case 'w':
        if(willNotBiteItSelf(posx,posy+Ratio) && willBeInBox(posx,posy+Ratio))
        {
            tail[0].y += Ratio;
            return 1;
            break;
        }
        //break;
    case 's':
        if(willNotBiteItSelf(posx,posy-Ratio) && willBeInBox(posx,posy-Ratio))
        {
            tail[0].y -= Ratio;
            return 1;
            break;
        }
        //break;
    case 'a':
        if(willNotBiteItSelf(posx-Ratio,posy) && willBeInBox(posx-Ratio,posy))
        {
            tail[0].x -= Ratio;
            return 1;
            break;
        }
        //break;
    case 'd':
        if(willNotBiteItSelf(posx+Ratio,posy) && willBeInBox(posx+Ratio,posy))
        {
            tail[0].x += Ratio;
            return 1;
            break;
        }
    default:
    {
        break;
    }
    }
    char x[100];
    char y[200];
    itoa(tail[0].x,x,10);
    itoa(tail[0].y,y,10);
    //printf("\n(%s , %s)",x,y);
    //system("pause");
}
int moveOk (int posx, int posy)
{
    return (willBeInBox(posx,posy) && willNotBiteItSelf(posx,posy));
}
void GreedySnake();
void getToFood ()
{
    color = 2;
    lastMove = 2;
    int posx,posy;
    posx = tail[0].x;
    posy = tail[0].y;
    if (tail[0].x < food.x && moveOk(posx+Ratio,posy))
        lastPressed ='d';
    else if (tail[0].x > food.x && moveOk (posx-Ratio,posy))
        lastPressed ='a';
    else if(tail[0].x == food.x && tail[0].y < food.y && moveOk (posx,posy+Ratio))
        lastPressed ='w';
    else if(tail[0].x == food.x && tail[0].y > food.y && moveOk (posx,posy-Ratio))
        lastPressed = 's';
    else
    {
        failed = true;
        GreedySnake();
    }
}
int currentWidth = width;
int currentHeight = height;
int currentMove = 1;
int getHeight();
void ReachMostLeft ()
{
    if(tail[0].x > 20 && moveOk(tail[0].x-10,tail[0].y))
        lastPressed = 'a';
}
void ReachTop ()
{
    if(tail[0].y < getHeight() && moveOk(tail[0].x,tail[0].y+10))
        lastPressed = 'w';
}
bool MostLeft ()
{
    return tail[0].x == 20;
}
bool MostTop ()
{
    return tail[0].y == currentHeight - 20;
}
void ReachMostRight ()
{
    if(tail[0].x < width - 20 && moveOk(tail[0].x+10,tail[0].y))
        lastPressed = 'd';
}
bool MostRight ()
{
    return tail[0].x == width - 20;
}
void MoveDown ()
{
    if(tail[0].y > getHeight() && moveOk(tail[0].x,tail[0].y-10))
        lastPressed = 's';
}
int willNotBiteItSelf (int posx ,int posy)
{
    int k;
    if(tailLength <= 5)
        return 1;
    for(k = 1 ; k <= tailLength ; ++k)
        if(tail[k].x == posx && tail[k].y == posy)
            return 0;
    return 1;
}
int getHeight ()
{
    for(k = tailLength ; k >= 1 ; --k)
        if(tail[0].y == tail[k].y)
            return tail[k].y;
    return -1;
}
void GreedySnake ()
{
    /*if(getHeight() == -1)
        getToFood();*/
        color = 1;
        lastMove = 1;
        if(tail[0].x == width - 20 && tail[0].y == 20)
        {
            currentMove = 99;
        }
        if(currentMove == 99)
        {
            ReachTop();
            if(tail[0].y != height - 20)
                currentMove = 99;
            else
            {
                failed = false;
                currentMove = 3;
            }
        }
        if (currentMove == 1)
        {
            ReachMostLeft();
            if(!MostLeft())
                currentMove = 1;
            else currentMove = 2;
        }
        if (currentMove == 2)
        {
            ReachTop();
            if(!MostTop())
                currentMove = 2;
            else currentMove = 3;
        }
        if (currentMove == 3)
        {
            if(tail[0].x != width - 20)
            {
                ReachMostRight();
                currentMove = 3;
            }
            else currentMove =4;
        }
        else if (currentMove == 4)
        {
            MoveDown();
            currentMove = 5;
        }
        else if (currentMove == 5)
        {
            ReachMostLeft();
            if(MostLeft())
                currentMove = 6;
            //else getToFood();
        }
        else if (currentMove == 6)
        {
            MoveDown();
            currentMove = 3;
        }
}
void Idle_AI ()
{
    //make_decision();
    //if(tailLength*Ratio < width - 20 && failed == false)
      //  getToFood();
    //else
        GreedySnake();
    MoveAI();
    if(tail[0].x == food.x && tail[0].y == food.y)
    {
        GenerateRandomFruitPos();
        ++tailLength;
        score += 10;
        /*if(getHeight() == -1)
            getToFood();
        else GreedySnake();*/
        GreedySnake();
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
    Sleep(30);
    glFlush();
    //startTime = glutGet(GLUT_ELAPSED_TIME);
    //prevTime = startTime;
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
    glutInitWindowSize(width,height);
    glutCreateWindow("Snake Game");
    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0,width,0,height+50);
    glutDisplayFunc(displayCB);
    char *s = argv[1];
    //if(argc == 2 && strcmp(s,"ai")==0)
    glutIdleFunc(Idle_AI);
    /*else
    {
        glutIdleFunc(myIdleFunc);
        glutKeyboardFunc(keyCB);
    }*/
    glutTimerFunc(TIMERMSECS, animate, 0);
    startTime = glutGet(GLUT_ELAPSED_TIME);
    prevTime = startTime;
    glutMainLoop();
    return 0;

}
