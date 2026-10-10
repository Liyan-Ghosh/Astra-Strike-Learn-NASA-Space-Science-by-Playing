// 1. Opening / Cover / Play / Controls
// 2. Level 1
// 3. Level 2
// 4. Level 3
// 5. Level 4
// 6. Level 5


#include <windows.h>
#include <mmsystem.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int activeScene=0;
int pausedScene=0;
int pausedTransitionCounter=0;
int levelTimerVersion=0;
int transitionCounter=0;
int carriedScore=0;
int carriedLife=3;
int highScore=0;
int highScoreSaveFailed=0;
char highScoreFile[MAX_PATH+32]="highscore.txt";
int shootSoundReady=0;
int blastSoundReady=0;
int enemyShootSoundReady=0;
int soundCleanupRegistered=0;

void showOpening();
void showMainMenu();
void resumeGame();
void showLevel1();
void showLevel2();
void showLevel3();
void showLevel4();
void showLevel5();
void loadHighScore();
void saveHighScore();
void updateHighScore(int currentScore);
void initSoundEffects();
void stopSoundEffects();
void closeSoundEffects();
void playShootSound();
void playBlastSound();
void playEnemyShootSound();

// NASA Space Apps additions
void showBriefing(int level);
void showFinalScreen();
void resetDiscoveries();
void showAsteroidFact(int level,int index);
void drawAsteroidLabel(int level,int index,float x,float y);
void drawFactPopup();

namespace Opening
{

//====================================================
// SPACE SHOOTER GAME
// OPENING COVER + PLAY / CONTROLS PAGES ONLY
//====================================================

#define SCREEN_PROJECT_INFO 0
#define SCREEN_MAIN_MENU 1
#define SCREEN_CONTROLS 2
#define SCREEN_SETTINGS 3
#define SCREEN_HIGH_SCORE 4

int gameScreen=SCREEN_PROJECT_INFO;
int menuSelection=0;       // 0 = PLAY, 1 = CONTROLS, 2 = SETTINGS, 3 = HIGH SCORE
int playButtonPressed=0;   // Standalone preview message only
int soundEnabled=1;

void sound();

//====================================================
// TEXT DRAWING
//====================================================

void renderBitmapString(
    float x,
    float y,
    float z,
    void *font,
    const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(
    float y,
    void *font,
    const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

//====================================================
// BOX DRAWING
//====================================================

void drawBox(
    float x1,
    float y1,
    float x2,
    float y2,
    float r,
    float g,
    float b)
{
    glColor3f(r,g,b);

    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}

void drawBoxOutline(
    float x1,
    float y1,
    float x2,
    float y2,
    float r,
    float g,
    float b)
{
    glColor3f(r,g,b);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}

//====================================================
// OPENING COVER
//====================================================

void drawProjectInfoScreen()
{
    glClearColor(0.02f,0.03f,0.01f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    // PROJECT NAME

    glColor3f(1,1,1);

    renderCenterBitmapString(
        0.45,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "NASA SPACE GAMES"
    );

    glColor3f(0.4f,0.8f,1.0f);

    renderCenterBitmapString(
        0.32,
        GLUT_BITMAP_HELVETICA_18,
        "NASA Space Apps Challenge"
    );

    // BUILT BY BOX

    drawBox(-0.45,0.10,0.45,-0.34,0.10,0.10,0.02);
    drawBoxOutline(-0.45,0.10,0.45,-0.34,0.65,0.65,0.35);

    glColor3f(0.9,0.9,0.7);

    renderCenterBitmapString(
        0.00,
        GLUT_BITMAP_HELVETICA_18,
        "Built by"
    );

    glColor3f(1,1,1);

    renderCenterBitmapString(
        -0.13,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "Liyan Ghosh"
    );

    renderCenterBitmapString(
        -0.26,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "Tithi Kirttania"
    );

    glColor3f(0.9,0.9,0.7);

    renderCenterBitmapString(
        -0.70,
        GLUT_BITMAP_HELVETICA_18,
        "Press ENTER to Continue"
    );

    renderCenterBitmapString(
        -0.80,
        GLUT_BITMAP_HELVETICA_12,
        "Press C to Close Game"
    );

    glFlush();
}

//====================================================
// PLAY / CONTROLS MAIN MENU PAGE
//====================================================

void drawMainMenuScreen()
{
    glClearColor(0.02f,0.03f,0.01f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1,1,1);

    renderCenterBitmapString(
        0.42,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "SPACE SHOOTER GAME"
    );

    glColor3f(0.4f,0.8f,1.0f);

    renderCenterBitmapString(
        0.30,
        GLUT_BITMAP_HELVETICA_18,
        "NASA Space Apps Challenge  -  Space Mission Design Game"
    );

    glColor3f(1,1,1);

    // PLAY BUTTON

    if(menuSelection==0)
    {
        drawBox(-0.28,0.05,0.28,-0.07,0.45,0.45,0.15);
    }
    else
    {
        drawBox(-0.28,0.05,0.28,-0.07,0.16,0.16,0.05);
    }

    // CONTROLS BUTTON

    if(menuSelection==1)
    {
        drawBox(-0.28,-0.18,0.28,-0.30,0.45,0.45,0.15);
    }
    else
    {
        drawBox(-0.28,-0.18,0.28,-0.30,0.16,0.16,0.05);
    }

    // SETTINGS BUTTON

    if(menuSelection==2)
    {
        drawBox(-0.28,-0.41,0.28,-0.53,0.45,0.45,0.15);
    }
    else
    {
        drawBox(-0.28,-0.41,0.28,-0.53,0.16,0.16,0.05);
    }

    // HIGH SCORE BUTTON

    if(menuSelection==3)
    {
        drawBox(-0.28,-0.64,0.28,-0.76,0.45,0.45,0.15);
    }
    else
    {
        drawBox(-0.28,-0.64,0.28,-0.76,0.16,0.16,0.05);
    }

    drawBoxOutline(-0.28,0.05,0.28,-0.07,0.6,0.6,0.35);
    drawBoxOutline(-0.28,-0.18,0.28,-0.30,0.6,0.6,0.35);
    drawBoxOutline(-0.28,-0.41,0.28,-0.53,0.6,0.6,0.35);
    drawBoxOutline(-0.28,-0.64,0.28,-0.76,0.6,0.6,0.35);

    glColor3f(1,1,1);

    renderCenterBitmapString(
        -0.01,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "PLAY"
    );

    renderCenterBitmapString(
        -0.24,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "CONTROLS"
    );

    renderCenterBitmapString(
        -0.47,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "SETTINGS"
    );

    renderCenterBitmapString(
        -0.70,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "HIGH SCORE"
    );

    // Standalone preview message.
    // In the full project, PLAY calls startNewGame().

    if(playButtonPressed==1)
    {
        glColor3f(0,1,0);

        renderCenterBitmapString(
            -0.48,
            GLUT_BITMAP_HELVETICA_18,
            "PLAY SELECTED - CONNECT startNewGame() HERE"
        );
    }

    glColor3f(0.9,0.9,0.7);

    if(::pausedScene!=0)
    {
        renderBitmapString(
            -0.9,
            -0.94,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "Resume=Esc"
        );
    }

    renderBitmapString(
        0.38,
        -0.70,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press W/S to Toggle"
    );

    renderBitmapString(
        0.38,
        -0.78,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press ENTER to Select"
    );

    renderBitmapString(
        0.38,
        -0.86,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press B to Back"
    );

    renderBitmapString(
        0.38,
        -0.94,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press C to Close Game"
    );

    glFlush();
}

//====================================================
// CONTROLS INSTRUCTION PAGE
// Opened from the CONTROLS button.
//====================================================

void drawControlsScreen()
{
    glClearColor(0.02f,0.03f,0.01f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1,1,1);

    renderBitmapString(
        -0.92,
        0.84,
        0,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "CONTROLS INSTRUCTION"
    );

    renderBitmapString(-0.92,0.72,0,GLUT_BITMAP_HELVETICA_18,"********************************");
    renderBitmapString(-0.92,0.60,0,GLUT_BITMAP_HELVETICA_18,"PLAYER");
    renderBitmapString(-0.92,0.48,0,GLUT_BITMAP_HELVETICA_18,"-> LEFT ARROW  : Move Left");
    renderBitmapString(-0.92,0.36,0,GLUT_BITMAP_HELVETICA_18,"-> RIGHT ARROW : Move Right");
    renderBitmapString(-0.92,0.24,0,GLUT_BITMAP_HELVETICA_18,"-> SPACE       : Shoot Bullet");
    renderBitmapString(-0.92,0.12,0,GLUT_BITMAP_HELVETICA_18,"-> C           : Close Game");

    renderBitmapString(-0.92,-0.06,0,GLUT_BITMAP_HELVETICA_18,"GAME OVER OPTIONS");
    renderBitmapString(-0.92,-0.18,0,GLUT_BITMAP_HELVETICA_18,"-> R : Retry Same Level");
    renderBitmapString(-0.92,-0.30,0,GLUT_BITMAP_HELVETICA_18,"-> N : New Game From Level 1");

    renderBitmapString(-0.92,-0.58,0,GLUT_BITMAP_HELVETICA_18,"POWER-UPS");
    renderBitmapString(-0.92,-0.68,0,GLUT_BITMAP_HELVETICA_18,"-> Heart increases life up to 3");
    renderBitmapString(-0.92,-0.80,0,GLUT_BITMAP_HELVETICA_18,"-> Shield protects player for limited time");
    renderBitmapString(-0.92,-0.92,0,GLUT_BITMAP_HELVETICA_18,"-> Bullet Power increases attack power");

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        0.42,
        -0.86,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press B to Back"
    );

    renderBitmapString(
        0.42,
        -0.94,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press C to Close Game"
    );

    glFlush();
}

//====================================================
// SETTINGS PAGE
// Only game sound ON / OFF is controlled here.
//====================================================

void drawSettingsScreen()
{
    glClearColor(0.02f,0.03f,0.01f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1,1,1);

    renderCenterBitmapString(
        0.42,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "SETTINGS"
    );

    drawBox(-0.28,0.05,0.28,-0.07,0.45,0.45,0.15);
    drawBoxOutline(-0.28,0.05,0.28,-0.07,0.6,0.6,0.35);

    glColor3f(1,1,1);

    if(soundEnabled==1)
    {
        renderCenterBitmapString(
            -0.01,
            GLUT_BITMAP_TIMES_ROMAN_24,
            "SOUND: ON"
        );
    }
    else
    {
        renderCenterBitmapString(
            -0.01,
            GLUT_BITMAP_TIMES_ROMAN_24,
            "SOUND: OFF"
        );
    }

    glColor3f(0.9,0.9,0.7);

    if(::pausedScene!=0)
    {
        renderBitmapString(
            -0.9,
            -0.94,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "Resume=Esc"
        );
    }

    renderBitmapString(
        0.38,
        -0.78,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press ENTER to Toggle Sound"
    );

    renderBitmapString(
        0.38,
        -0.86,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press B to Back"
    );

    renderBitmapString(
        0.38,
        -0.94,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press C to Close Game"
    );

    glFlush();
}

//====================================================
// HIGH SCORE PAGE
//====================================================

void drawHighScoreScreen()
{
    char highScoreText[40];

    glClearColor(0.02f,0.03f,0.01f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1,1,1);

    renderCenterBitmapString(
        0.42,
        GLUT_BITMAP_TIMES_ROMAN_24,
        "HIGH SCORE"
    );

    drawBox(-0.34,0.14,0.34,-0.14,0.16,0.16,0.05);
    drawBoxOutline(-0.34,0.14,0.34,-0.14,0.6,0.6,0.35);

    glColor3f(1,1,1);
    sprintf(highScoreText,"BEST SCORE: %03d",::highScore);

    renderCenterBitmapString(
        -0.01,
        GLUT_BITMAP_TIMES_ROMAN_24,
        highScoreText
    );

    glColor3f(0.9,0.9,0.7);

    renderCenterBitmapString(
        -0.30,
        GLUT_BITMAP_HELVETICA_18,
        "Beat this score to set a new record."
    );

    if(::highScoreSaveFailed==1)
    {
        glColor3f(1,0.4,0.4);

        renderCenterBitmapString(
            -0.46,
            GLUT_BITMAP_HELVETICA_12,
            "Record not saved. Check folder write permission."
        );
    }

    glColor3f(0.9,0.9,0.7);

    if(::pausedScene!=0)
    {
        renderBitmapString(
            -0.9,
            -0.94,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "Resume=Esc"
        );
    }

    renderBitmapString(
        0.38,
        -0.86,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press B to Back"
    );

    renderBitmapString(
        0.38,
        -0.94,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Press C to Close Game"
    );

    glFlush();
}

//====================================================
// DISPLAY CONTROLLER
//====================================================

void display()
{
    if(gameScreen==SCREEN_PROJECT_INFO)
    {
        drawProjectInfoScreen();
    }
    else if(gameScreen==SCREEN_MAIN_MENU)
    {
        drawMainMenuScreen();
    }
    else if(gameScreen==SCREEN_CONTROLS)
    {
        drawControlsScreen();
    }
    else if(gameScreen==SCREEN_SETTINGS)
    {
        drawSettingsScreen();
    }
    else if(gameScreen==SCREEN_HIGH_SCORE)
    {
        drawHighScoreScreen();
    }
}

//====================================================
// KEYBOARD CONTROLLER
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    // OPENING COVER PAGE

    if(gameScreen==SCREEN_PROJECT_INFO)
    {
        if(key==13)
        {
            gameScreen=SCREEN_MAIN_MENU;
        }
        else if(key=='c' || key=='C')
        {
            PlaySound(NULL,NULL,0);
            exit(0);
        }

        glutPostRedisplay();
        return;
    }

    // PLAY / CONTROLS MENU PAGE

    if(gameScreen==SCREEN_MAIN_MENU)
    {
        if(
            key=='w' || key=='W' ||
            key=='s' || key=='S'
          )
        {
            playButtonPressed=0;

            if(key=='w' || key=='W')
            {
                menuSelection--;

                if(menuSelection<0)
                {
                    menuSelection=3;
                }
            }
            else
            {
                menuSelection++;

                if(menuSelection>3)
                {
                    menuSelection=0;
                }
            }
        }
        else if(key==13)
        {
            if(menuSelection==0)
            {
                // In the complete project use:
                // startNewGame();

                playButtonPressed=1;
            }
            else if(menuSelection==1)
            {
                gameScreen=SCREEN_CONTROLS;
            }
            else if(menuSelection==2)
            {
                gameScreen=SCREEN_SETTINGS;
            }
            else
            {
                gameScreen=SCREEN_HIGH_SCORE;
            }
        }
        else if(key=='b' || key=='B')
        {
            playButtonPressed=0;
            gameScreen=SCREEN_PROJECT_INFO;
        }
        else if(key=='c' || key=='C')
        {
            PlaySound(NULL,NULL,0);
            exit(0);
        }

        glutPostRedisplay();
        return;
    }

    // CONTROLS INSTRUCTION PAGE

    if(gameScreen==SCREEN_CONTROLS)
    {
        if(
            key=='b' || key=='B' ||
            key==13
          )
        {
            gameScreen=SCREEN_MAIN_MENU;
        }
        else if(key=='c' || key=='C')
        {
            PlaySound(NULL,NULL,0);
            exit(0);
        }

        glutPostRedisplay();
    }

    // SETTINGS PAGE

    if(gameScreen==SCREEN_SETTINGS)
    {
        if(key==13)
        {
            if(soundEnabled==1)
            {
                soundEnabled=0;
            }
            else
            {
                soundEnabled=1;
            }

            sound();
        }
        else if(key=='b' || key=='B' || key==27)
        {
            gameScreen=SCREEN_MAIN_MENU;
        }
        else if(key=='c' || key=='C')
        {
            PlaySound(NULL,NULL,0);
            exit(0);
        }

        glutPostRedisplay();
    }

    // HIGH SCORE PAGE

    if(gameScreen==SCREEN_HIGH_SCORE)
    {
        if(key=='b' || key=='B' || key==13 || key==27)
        {
            gameScreen=SCREEN_MAIN_MENU;
        }
        else if(key=='c' || key=='C')
        {
            PlaySound(NULL,NULL,0);
            exit(0);
        }

        glutPostRedisplay();
    }
}

//====================================================
// BACKGROUND SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    if(soundEnabled==0)
    {
        ::stopSoundEffects();
        PlaySound(NULL,NULL,0);
        return;
    }

    PlaySound(
        "a.wav",
        NULL,
        SND_ASYNC|SND_FILENAME|SND_LOOP
    );
}

//====================================================
// SAME gluOrtho2D SYSTEM AS THE FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// COVER + MENU ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Opening
// OPENING FULL CPP SECTION END
#undef SCREEN_PROJECT_INFO
#undef SCREEN_MAIN_MENU
#undef SCREEN_CONTROLS
#undef SCREEN_SETTINGS
#undef SCREEN_HIGH_SCORE

//============================================================
// LEVEL1 FULL CPP SECTION START
//============================================================
namespace Level1
{

#define PI 3.14159265358979323846

//====================================================
// MEMBER 1 : TITHI KIRTTANIA
// ID       : 23-50788-1
// LEVEL 1  : BASIC SHOOTER LEVEL
//====================================================

int currentLevel = 1;
int levelComplete = 0;
int gameOver = 0;

float playerX = 0.0;
int playerLife = 3;
int playerDamageDelay = 0;
int leftKeyPressed = 0;
int rightKeyPressed = 0;
int score = 0;

//====================================================
// LEVEL 1 ENEMY
//====================================================

#define MAX_ENEMIES 1

float enemyX[MAX_ENEMIES];
float enemyY[MAX_ENEMIES];
float enemySpeed[MAX_ENEMIES];
int enemyDirection[MAX_ENEMIES];
int enemyAlive[MAX_ENEMIES];
int enemyHealth[MAX_ENEMIES];

//====================================================
// PLAYER BULLETS
//====================================================

#define MAX_BULLETS 12

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
float bulletSpeed = 0.02;
int shootDelay = 0;

//====================================================
// SIDE ENEMY BULLETS
//====================================================

#define MAX_ENEMY_BULLETS 1

float enemyBulletX[MAX_ENEMY_BULLETS];
float enemyBulletY[MAX_ENEMY_BULLETS];
int enemyBulletActive[MAX_ENEMY_BULLETS];
float enemyBulletSpeed = 0.015;
int enemyShootDelay[MAX_ENEMIES];

//====================================================
// LEVEL 1 SOUND EFFECTS AND BLASTS
//====================================================

#define MAX_BLASTS 16
#define BLAST_TIME 16

float blastX[MAX_BLASTS];
float blastY[MAX_BLASTS];
float blastSize[MAX_BLASTS];
int blastTimer[MAX_BLASTS];

//====================================================
// COMMON SMALL HELPER FUNCTIONS USED BY LEVEL 1
//====================================================

float getPlayerY()
{
    return -0.55;
}

float getPlayerSpeed()
{
    return 0.015;
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(float y,void *font,const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

void drawCircle(GLfloat x,GLfloat y,GLfloat radius)
{
    int i;
    int triangleAmount=100;
    GLfloat twicePi=2.0f*PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(i=0;i<=triangleAmount;i++)
    {
        glVertex2f(
            x+(radius*cos(i*twicePi/triangleAmount)),
            y+(radius*sin(i*twicePi/triangleAmount))
        );
    }

    glEnd();
}

void drawCenterMessage(
    const char *line1,
    const char *line2,
    float r,
    float g,
    float b)
{
    glColor3f(0.04,0.04,0.04);

    glBegin(GL_QUADS);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);

    if(line2==NULL)
    {
        renderCenterBitmapString(
            0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );
    }
    else
    {
        renderCenterBitmapString(
            0.08,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );

        renderCenterBitmapString(
            -0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line2
        );
    }
}

//====================================================
// SIMPLE BLAST FUNCTIONS
//====================================================

void startBlast(float x,float y,float size)
{
    int slot=0;

    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]==0)
        {
            slot=i;
            break;
        }

        if(blastTimer[i]<blastTimer[slot])
        {
            slot=i;
        }
    }

    blastX[slot]=x;
    blastY[slot]=y;
    blastSize[slot]=size;
    blastTimer[slot]=BLAST_TIME;
    ::playBlastSound();
}

void updateBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            blastTimer[i]--;
        }
    }
}

void drawBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            float fade=(float)blastTimer[i]/BLAST_TIME;
            float radius=blastSize[i]*(0.25f+(1.0f-fade)*0.75f);

            glColor3f(fade,0.18f*fade,0);
            drawCircle(blastX[i],blastY[i],radius);

            glColor3f(fade,0.55f*fade,0.05f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.65f);

            glColor3f(fade,0.95f*fade,0.45f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.30f);

            glLineWidth(2);
            glColor3f(fade,0.70f*fade,0.10f*fade);

            glBegin(GL_LINES);

            for(int j=0;j<8;j++)
            {
                float direction=j*2.0f*PI/8;

                glVertex2f(
                    blastX[i]+radius*0.75f*cos(direction),
                    blastY[i]+radius*0.75f*sin(direction)
                );

                glVertex2f(
                    blastX[i]+radius*1.25f*cos(direction),
                    blastY[i]+radius*1.25f*sin(direction)
                );
            }

            glEnd();
        }
    }
}

//====================================================
// LEVEL 1 DRAWING FUNCTIONS
//====================================================

void drawHeart(float x,float y)
{
    glColor3f(1,0,0);

    drawCircle(x-0.025,y,0.025);
    drawCircle(x+0.025,y,0.025);

    glBegin(GL_TRIANGLES);
    glVertex2f(x-0.05,y);
    glVertex2f(x+0.05,y);
    glVertex2f(x,y-0.07);
    glEnd();
}

void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.2,0.7);
    glVertex2f(0.2,0.6);
    glVertex2f(0.6,0.8);
    glVertex2f(-0.7,0.2);
    glVertex2f(-0.3,0.3);
    glVertex2f(0.4,0.2);
    glVertex2f(0.8,-0.2);
    glVertex2f(-0.9,-0.6);
    glVertex2f(-0.4,-0.8);
    glVertex2f(0.2,-0.7);
    glVertex2f(0.7,-0.6);
    glEnd();
}

void playerShip()
{
    glPushMatrix();
    glTranslatef(playerX,getPlayerY(),0);

    glColor3f(0.8,0.8,0.8);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.15);
    glVertex2f(-0.08,-0.05);
    glVertex2f(0.08,-0.05);
    glEnd();

    glColor3f(0.3,0.3,0.3);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,-0.02);
    glVertex2f(-0.18,-0.10);
    glVertex2f(-0.07,-0.12);
    glVertex2f(0.07,-0.02);
    glVertex2f(0.18,-0.10);
    glVertex2f(0.07,-0.12);
    glEnd();

    glColor3f(0,0.5,1);

    glBegin(GL_QUADS);
    glVertex2f(-0.03,0.05);
    glVertex2f(0.03,0.05);
    glVertex2f(0.03,-0.02);
    glVertex2f(-0.03,-0.02);
    glEnd();

    glColor3f(0,0,1);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,-0.12);
    glVertex2f(0.03,-0.12);
    glVertex2f(0,-0.20);
    glEnd();

    glPopMatrix();
}

void enemyShip(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(0.9,0.1,0.1);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.12);
    glVertex2f(-0.08,0.08);
    glVertex2f(0.08,0.08);
    glEnd();

    glColor3f(0.5,0.5,0.5);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,0.03);
    glVertex2f(-0.18,0.10);
    glVertex2f(-0.08,0.12);
    glVertex2f(0.07,0.03);
    glVertex2f(0.18,0.10);
    glVertex2f(0.08,0.12);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.02,-0.06);
    glVertex2f(0.02,-0.06);
    glVertex2f(0.02,0.00);
    glVertex2f(-0.02,0.00);
    glEnd();

    glColor3f(1,0.5,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,0.12);
    glVertex2f(0.03,0.12);
    glVertex2f(0,0.20);
    glEnd();

    glPopMatrix();
}








//====================================================
// LEVEL 1 BULLET DRAWING AND CREATION
//====================================================

void drawPlayerBullets()
{
    glColor3f(0,0.7,1);
    glLineWidth(4);

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bulletX[i],bulletY[i]);
            glVertex2f(bulletX[i],bulletY[i]+0.12);
            glEnd();
        }
    }
}

void createPlayerBullet(float x,float y)
{
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            bulletActive[i]=1;
            bulletX[i]=x;
            bulletY[i]=y;
            break;
        }
    }
}

void fireBullet()
{
    int freeBullet=0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    float startY=-0.25;

    createPlayerBullet(playerX,startY);

    ::playShootSound();
}

void drawEnemyBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]-0.12);
            glEnd();
        }
    }
}

void fireEnemyBullet(int enemyNumber)
{
    if(
        enemyAlive[enemyNumber]==1 &&
        enemyBulletActive[enemyNumber]==0
      )
    {
        enemyBulletActive[enemyNumber]=1;
        enemyBulletX[enemyNumber]=enemyX[enemyNumber];
        enemyBulletY[enemyNumber]=enemyY[enemyNumber]-0.12;
        ::playEnemyShootSound();
    }
}




//====================================================
// LEVEL 1 INITIAL SETUP
//====================================================

void startLevel1()
{
    ::initSoundEffects();
    ::stopSoundEffects();

    for(int i=0;i<MAX_BLASTS;i++)
    {
        blastTimer[i]=0;
    }

    currentLevel=1;
    levelComplete=0;
    gameOver=0;

    playerX=0.0;
    playerLife=3;
    playerDamageDelay=0;
    leftKeyPressed=0;
    rightKeyPressed=0;
    shootDelay=0;

    enemyX[0]=0.0;
    enemyY[0]=0.55;
    enemySpeed[0]=0.0;
    enemyDirection[0]=0;
    enemyAlive[0]=1;
    enemyHealth[0]=100;

    enemyShootDelay[0]=70;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i]=0;
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        enemyBulletActive[i]=0;
    }

}

//====================================================
// LEVEL 1 MOVEMENT FUNCTIONS
//====================================================

void movePlayer()
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(leftKeyPressed==1)
    {
        playerX=playerX-getPlayerSpeed();

        if(playerX<-0.75)
        {
            playerX=-0.75;
        }
    }

    if(rightKeyPressed==1)
    {
        playerX=playerX+getPlayerSpeed();

        if(playerX>0.75)
        {
            playerX=0.75;
        }
    }
}





//====================================================
// LEVEL 1 COMPLETE CONDITION
//====================================================

void checkLevel1Complete()
{
    if(
        enemyAlive[0]==0 &&
        levelComplete==0
      )
    {
        levelComplete=1;

        enemyBulletActive[0]=0;
    }
}

//====================================================
// LEVEL 1 COLLISION FUNCTIONS
//====================================================

void checkPlayerBulletCollision()
{
    for(int e=0;e<MAX_ENEMIES;e++)
    {
        if(enemyAlive[e]==1)
        {
            for(int i=0;i<MAX_BULLETS;i++)
            {
                if(bulletActive[i]==1)
                {
                    if(
                        bulletX[i]>enemyX[e]-0.08 &&
                        bulletX[i]<enemyX[e]+0.08 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.08
                      )
                    {
                        enemyHealth[e]=enemyHealth[e]-20;
                        score=score+20;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]-0.18 &&
                        bulletX[i]<enemyX[e]-0.07 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]+0.07 &&
                        bulletX[i]<enemyX[e]+0.18 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }

                    if(enemyHealth[e]<=0)
                    {
                        enemyHealth[e]=0;
                        enemyAlive[e]=0;
                        enemyBulletActive[e]=0;
                        startBlast(enemyX[e],enemyY[e],0.22);
                        checkLevel1Complete();
                        break;
                    }
                }
            }
        }
    }
}



void damagePlayer()
{
    if(gameOver==1)
    {
        return;
    }

    playerLife=playerLife-1;
    playerDamageDelay=30;

    if(playerLife<=0)
    {
        playerLife=0;
        gameOver=1;
        startBlast(playerX,getPlayerY(),0.22);
        leftKeyPressed=0;
        rightKeyPressed=0;
    }
}

void checkPlayerDamage()
{
    float playerY=getPlayerY();

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(
            enemyBulletActive[i]==1 &&
            playerDamageDelay==0
          )
        {
            if(
                enemyBulletX[i]>playerX-0.18 &&
                enemyBulletX[i]<playerX+0.18 &&
                enemyBulletY[i]>playerY-0.23 &&
                enemyBulletY[i]<playerY+0.17
              )
            {
                enemyBulletActive[i]=0;
                damagePlayer();
            }
        }
    }
}







//====================================================
// LEVEL 1 UPDATE LOOP
//====================================================

void update(int value)
{
    // Stop this level's timer after another display becomes active.
    if(::activeScene!=1 || value!=::levelTimerVersion)
    {
        return;
    }

    updateBlasts();

    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        glutPostRedisplay();
        glutTimerFunc(30,update,value);
        return;
    }

    movePlayer();

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            bulletY[i]=bulletY[i]+bulletSpeed;

            if(bulletY[i]>1.0)
            {
                bulletActive[i]=0;
            }
        }
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            enemyBulletY[i]=enemyBulletY[i]-enemyBulletSpeed;

            if(enemyBulletY[i]<-1.0)
            {
                enemyBulletActive[i]=0;
            }
        }
    }

    checkPlayerBulletCollision();
    checkPlayerDamage();

    if(playerDamageDelay>0)
    {
        playerDamageDelay--;
    }

    if(shootDelay>0)
    {
        shootDelay--;
    }

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            if(enemyShootDelay[i]>0)
            {
                enemyShootDelay[i]--;
            }
            else
            {
                fireEnemyBullet(i);
                enemyShootDelay[i]=90;
            }
        }
    }

    checkLevel1Complete();

    ::updateHighScore(score);

    glutPostRedisplay();
    glutTimerFunc(30,update,value);
}

//====================================================
// LEVEL 1 KEYBOARD CONTROL
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        ::showMainMenu();
        return;
    }

    if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }

    if(gameOver==1)
    {
        if(key=='r' || key=='R')
        {
            score=0;
            startLevel1();
            glutPostRedisplay();
        }
        else if(key=='n' || key=='N')
        {
            ::carriedScore=0;
            ::carriedLife=3;
            ::transitionCounter=0;

            score=0;
            startLevel1();
            glutPostRedisplay();
        }

        return;
    }

    if(levelComplete==1)
    {
        return;
    }

    if(key==' ')
    {
        if(shootDelay==0)
        {
            fireBullet();
            shootDelay=5;
        }
    }
}

void specialKey(int key,int x,int y)
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=1;
    }

    glutPostRedisplay();
}

void specialKeyUp(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=0;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=0;
    }
}

//====================================================
// LEVEL 1 DISPLAY
//====================================================

void display()
{
    char scoreText[30];

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawStars();

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            enemyShip(enemyX[i],enemyY[i]);
        }
    }

    if(gameOver==0)
    {
        playerShip();
    }

    drawPlayerBullets();
    drawEnemyBullets();

    drawBlasts();

    glColor3f(1,1,1);

    sprintf(scoreText,"SCORE: %03d",score);

    renderBitmapString(
        -0.9,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        scoreText
    );

    renderBitmapString(
        -0.05,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LEVEL: 1"
    );

    renderBitmapString(
        0.55,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LIFE:"
    );

    if(playerLife>=1)
    {
        drawHeart(0.72,0.91);
    }

    if(playerLife>=2)
    {
        drawHeart(0.82,0.91);
    }

    if(playerLife>=3)
    {
        drawHeart(0.92,0.91);
    }

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        -0.9,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Menu=Esc"
    );

    renderBitmapString(
        0.72,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "C=CLOSE"
    );

    if(gameOver==1)
    {
        drawCenterMessage(
            "GAME OVER",
            "R=RETRY  N=NEW",
            1,
            0,
            0
        );
    }

    if(
        levelComplete==1 &&
        enemyAlive[0]==0
      )
    {
        drawCenterMessage(
            "LEVEL 1 COMPLETE",
            "GOING TO NEXT LEVEL",
            0,
            1,
            0
        );
    }

    glFlush();
}

//====================================================
// SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    Opening::sound();
}

//====================================================
// SAME gluOrtho2D COORDINATE SYSTEM AS FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// LEVEL 1 ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Level1
// LEVEL1 FULL CPP SECTION END
#undef MAX_BLASTS
#undef BLAST_TIME
#undef PI
#undef MAX_ENEMIES
#undef MAX_BULLETS
#undef MAX_ENEMY_BULLETS

//============================================================
// LEVEL2 FULL CPP SECTION START
//============================================================
namespace Level2
{

#define PI 3.14159265358979323846

//====================================================
// MEMBER 2 : SHORME RANI SAHA
// ID       : 23-54389-3
// LEVEL 2  : TWO ENEMIES AND HEART POWER-UP
//====================================================

int currentLevel = 2;
int levelComplete = 0;
int gameOver = 0;

float playerX = 0.0;
int playerLife = 3;
int playerDamageDelay = 0;
int leftKeyPressed = 0;
int rightKeyPressed = 0;
int score = 0;

//====================================================
// LEVEL 2 ENEMIES
//====================================================

#define MAX_ENEMIES 2

float enemyX[MAX_ENEMIES];
float enemyY[MAX_ENEMIES];
float enemySpeed[MAX_ENEMIES];
int enemyDirection[MAX_ENEMIES];
int enemyAlive[MAX_ENEMIES];
int enemyHealth[MAX_ENEMIES];

//====================================================
// PLAYER BULLETS
//====================================================

#define MAX_BULLETS 12

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
float bulletSpeed = 0.02;
int shootDelay = 0;

//====================================================
// SIDE ENEMY BULLETS
//====================================================

#define MAX_ENEMY_BULLETS 2

float enemyBulletX[MAX_ENEMY_BULLETS];
float enemyBulletY[MAX_ENEMY_BULLETS];
int enemyBulletActive[MAX_ENEMY_BULLETS];
float enemyBulletSpeed = 0.015;
int enemyShootDelay[MAX_ENEMIES];

//====================================================
// LEVEL 2 HEART POWER-UP
//====================================================

float heartPowerX = 0.0;
float heartPowerY = 0.85;
float heartPowerSpeed = 0.003;
int heartPowerActive = 1;

//====================================================
// LEVEL 2 SOUND EFFECTS AND BLASTS
//====================================================

#define MAX_BLASTS 16
#define BLAST_TIME 16

float blastX[MAX_BLASTS];
float blastY[MAX_BLASTS];
float blastSize[MAX_BLASTS];
int blastTimer[MAX_BLASTS];

//====================================================
// COMMON SMALL HELPER FUNCTIONS USED BY LEVEL 2
//====================================================

float getPlayerY()
{
    return -0.55;
}

float getPlayerSpeed()
{
    return 0.015;
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(float y,void *font,const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

void drawCircle(GLfloat x,GLfloat y,GLfloat radius)
{
    int i;
    int triangleAmount=100;
    GLfloat twicePi=2.0f*PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(i=0;i<=triangleAmount;i++)
    {
        glVertex2f(
            x+(radius*cos(i*twicePi/triangleAmount)),
            y+(radius*sin(i*twicePi/triangleAmount))
        );
    }

    glEnd();
}

void drawCenterMessage(
    const char *line1,
    const char *line2,
    float r,
    float g,
    float b)
{
    glColor3f(0.04,0.04,0.04);

    glBegin(GL_QUADS);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);

    if(line2==NULL)
    {
        renderCenterBitmapString(
            0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );
    }
    else
    {
        renderCenterBitmapString(
            0.08,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );

        renderCenterBitmapString(
            -0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line2
        );
    }
}

//====================================================
// SIMPLE BLAST FUNCTIONS
//====================================================

void startBlast(float x,float y,float size)
{
    int slot=0;

    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]==0)
        {
            slot=i;
            break;
        }

        if(blastTimer[i]<blastTimer[slot])
        {
            slot=i;
        }
    }

    blastX[slot]=x;
    blastY[slot]=y;
    blastSize[slot]=size;
    blastTimer[slot]=BLAST_TIME;
    ::playBlastSound();
}

void updateBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            blastTimer[i]--;
        }
    }
}

void drawBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            float fade=(float)blastTimer[i]/BLAST_TIME;
            float radius=blastSize[i]*(0.25f+(1.0f-fade)*0.75f);

            glColor3f(fade,0.18f*fade,0);
            drawCircle(blastX[i],blastY[i],radius);

            glColor3f(fade,0.55f*fade,0.05f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.65f);

            glColor3f(fade,0.95f*fade,0.45f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.30f);

            glLineWidth(2);
            glColor3f(fade,0.70f*fade,0.10f*fade);

            glBegin(GL_LINES);

            for(int j=0;j<8;j++)
            {
                float direction=j*2.0f*PI/8;

                glVertex2f(
                    blastX[i]+radius*0.75f*cos(direction),
                    blastY[i]+radius*0.75f*sin(direction)
                );

                glVertex2f(
                    blastX[i]+radius*1.25f*cos(direction),
                    blastY[i]+radius*1.25f*sin(direction)
                );
            }

            glEnd();
        }
    }
}

//====================================================
// LEVEL 2 DRAWING FUNCTIONS
//====================================================

void drawHeart(float x,float y)
{
    glColor3f(1,0,0);

    drawCircle(x-0.025,y,0.025);
    drawCircle(x+0.025,y,0.025);

    glBegin(GL_TRIANGLES);
    glVertex2f(x-0.05,y);
    glVertex2f(x+0.05,y);
    glVertex2f(x,y-0.07);
    glEnd();
}

void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.2,0.7);
    glVertex2f(0.2,0.6);
    glVertex2f(0.6,0.8);
    glVertex2f(-0.7,0.2);
    glVertex2f(-0.3,0.3);
    glVertex2f(0.4,0.2);
    glVertex2f(0.8,-0.2);
    glVertex2f(-0.9,-0.6);
    glVertex2f(-0.4,-0.8);
    glVertex2f(0.2,-0.7);
    glVertex2f(0.7,-0.6);
    glEnd();
}

void playerShip()
{
    glPushMatrix();
    glTranslatef(playerX,getPlayerY(),0);

    glColor3f(0.8,0.8,0.8);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.15);
    glVertex2f(-0.08,-0.05);
    glVertex2f(0.08,-0.05);
    glEnd();

    glColor3f(0.3,0.3,0.3);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,-0.02);
    glVertex2f(-0.18,-0.10);
    glVertex2f(-0.07,-0.12);
    glVertex2f(0.07,-0.02);
    glVertex2f(0.18,-0.10);
    glVertex2f(0.07,-0.12);
    glEnd();

    glColor3f(0,0.5,1);

    glBegin(GL_QUADS);
    glVertex2f(-0.03,0.05);
    glVertex2f(0.03,0.05);
    glVertex2f(0.03,-0.02);
    glVertex2f(-0.03,-0.02);
    glEnd();

    glColor3f(0,0,1);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,-0.12);
    glVertex2f(0.03,-0.12);
    glVertex2f(0,-0.20);
    glEnd();

    glPopMatrix();
}

void enemyShip(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(0.9,0.1,0.1);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.12);
    glVertex2f(-0.08,0.08);
    glVertex2f(0.08,0.08);
    glEnd();

    glColor3f(0.5,0.5,0.5);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,0.03);
    glVertex2f(-0.18,0.10);
    glVertex2f(-0.08,0.12);
    glVertex2f(0.07,0.03);
    glVertex2f(0.18,0.10);
    glVertex2f(0.08,0.12);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.02,-0.06);
    glVertex2f(0.02,-0.06);
    glVertex2f(0.02,0.00);
    glVertex2f(-0.02,0.00);
    glEnd();

    glColor3f(1,0.5,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,0.12);
    glVertex2f(0.03,0.12);
    glVertex2f(0,0.20);
    glEnd();

    glPopMatrix();
}








//====================================================
// LEVEL 2 BULLET DRAWING AND CREATION
//====================================================

void drawPlayerBullets()
{
    glColor3f(0,0.7,1);
    glLineWidth(4);

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bulletX[i],bulletY[i]);
            glVertex2f(bulletX[i],bulletY[i]+0.12);
            glEnd();
        }
    }
}

void createPlayerBullet(float x,float y)
{
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            bulletActive[i]=1;
            bulletX[i]=x;
            bulletY[i]=y;
            break;
        }
    }
}

void fireBullet()
{
    int freeBullet=0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    float startY=-0.25;

    createPlayerBullet(playerX,startY);

    ::playShootSound();
}

void drawEnemyBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]-0.12);
            glEnd();
        }
    }
}

void fireEnemyBullet(int enemyNumber)
{
    if(
        enemyAlive[enemyNumber]==1 &&
        enemyBulletActive[enemyNumber]==0
      )
    {
        enemyBulletActive[enemyNumber]=1;
        enemyBulletX[enemyNumber]=enemyX[enemyNumber];
        enemyBulletY[enemyNumber]=enemyY[enemyNumber]-0.12;
        ::playEnemyShootSound();
    }
}




//====================================================
// LEVEL 2 INITIAL SETUP
//====================================================

void startLevel2()
{
    ::initSoundEffects();
    ::stopSoundEffects();

    for(int i=0;i<MAX_BLASTS;i++)
    {
        blastTimer[i]=0;
    }

    currentLevel=2;
    levelComplete=0;
    gameOver=0;

    playerX=0.0;
    playerLife=3;
    playerDamageDelay=0;
    leftKeyPressed=0;
    rightKeyPressed=0;
    shootDelay=0;

    enemyX[0]=-0.45;
    enemyY[0]=0.55;
    enemySpeed[0]=0.004;
    enemyDirection[0]=1;
    enemyAlive[0]=1;
    enemyHealth[0]=100;

    enemyX[1]=0.45;
    enemyY[1]=0.55;
    enemySpeed[1]=0.004;
    enemyDirection[1]=-1;
    enemyAlive[1]=1;
    enemyHealth[1]=100;

    enemyShootDelay[0]=60;
    enemyShootDelay[1]=90;

    heartPowerX=0.0;
    heartPowerY=0.85;
    heartPowerSpeed=0.003;
    heartPowerActive=1;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i]=0;
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        enemyBulletActive[i]=0;
    }

}

//====================================================
// LEVEL 2 MOVEMENT FUNCTIONS
//====================================================

void movePlayer()
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(leftKeyPressed==1)
    {
        playerX=playerX-getPlayerSpeed();

        if(playerX<-0.75)
        {
            playerX=-0.75;
        }
    }

    if(rightKeyPressed==1)
    {
        playerX=playerX+getPlayerSpeed();

        if(playerX>0.75)
        {
            playerX=0.75;
        }
    }
}

void moveEnemies()
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            enemyX[i]=
                enemyX[i]+enemySpeed[i]*enemyDirection[i];

            if(i==0)
            {
                if(enemyX[i]<-0.60)
                {
                    enemyX[i]=-0.60;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>-0.30)
                {
                    enemyX[i]=-0.30;
                    enemyDirection[i]=-1;
                }
            }

            if(i==1)
            {
                if(enemyX[i]<0.30)
                {
                    enemyX[i]=0.30;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>0.60)
                {
                    enemyX[i]=0.60;
                    enemyDirection[i]=-1;
                }
            }
        }
    }
}




//====================================================
// LEVEL 2 COMPLETE CONDITION
//====================================================

void checkLevel2Complete()
{
    if(
        enemyAlive[0]==0 &&
        enemyAlive[1]==0 &&
        levelComplete==0
      )
    {
        levelComplete=1;

        enemyBulletActive[0]=0;
        enemyBulletActive[1]=0;
    }
}

//====================================================
// LEVEL 2 COLLISION FUNCTIONS
//====================================================

void checkPlayerBulletCollision()
{
    for(int e=0;e<MAX_ENEMIES;e++)
    {
        if(enemyAlive[e]==1)
        {
            for(int i=0;i<MAX_BULLETS;i++)
            {
                if(bulletActive[i]==1)
                {
                    if(
                        bulletX[i]>enemyX[e]-0.08 &&
                        bulletX[i]<enemyX[e]+0.08 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.08
                      )
                    {
                        enemyHealth[e]=enemyHealth[e]-20;
                        score=score+20;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]-0.18 &&
                        bulletX[i]<enemyX[e]-0.07 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]+0.07 &&
                        bulletX[i]<enemyX[e]+0.18 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }

                    if(enemyHealth[e]<=0)
                    {
                        enemyHealth[e]=0;
                        enemyAlive[e]=0;
                        enemyBulletActive[e]=0;
                        startBlast(enemyX[e],enemyY[e],0.22);
                        checkLevel2Complete();
                        break;
                    }
                }
            }
        }
    }
}



void damagePlayer()
{
    if(gameOver==1)
    {
        return;
    }

    playerLife=playerLife-1;
    playerDamageDelay=30;

    if(playerLife<=0)
    {
        playerLife=0;
        gameOver=1;
        startBlast(playerX,getPlayerY(),0.22);
        leftKeyPressed=0;
        rightKeyPressed=0;
    }
}

void checkPlayerDamage()
{
    float playerY=getPlayerY();

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(
            enemyBulletActive[i]==1 &&
            playerDamageDelay==0
          )
        {
            if(
                enemyBulletX[i]>playerX-0.18 &&
                enemyBulletX[i]<playerX+0.18 &&
                enemyBulletY[i]>playerY-0.23 &&
                enemyBulletY[i]<playerY+0.17
              )
            {
                enemyBulletActive[i]=0;
                damagePlayer();
            }
        }
    }
}

void checkHeartPower()
{
    if(heartPowerActive==1)
    {
        if(
            heartPowerX>playerX-0.18 &&
            heartPowerX<playerX+0.18 &&
            heartPowerY>getPlayerY()-0.12 &&
            heartPowerY<getPlayerY()+0.18
          )
        {
            if(playerLife<3)
            {
                playerLife=playerLife+1;
                heartPowerActive=0;
            }
        }
    }
}






//====================================================
// LEVEL 2 UPDATE LOOP
//====================================================

void update(int value)
{
    // Stop this level's timer after another display becomes active.
    if(::activeScene!=2 || value!=::levelTimerVersion)
    {
        return;
    }

    updateBlasts();

    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        glutPostRedisplay();
        glutTimerFunc(30,update,value);
        return;
    }

    movePlayer();

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            bulletY[i]=bulletY[i]+bulletSpeed;

            if(bulletY[i]>1.0)
            {
                bulletActive[i]=0;
            }
        }
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            enemyBulletY[i]=enemyBulletY[i]-enemyBulletSpeed;

            if(enemyBulletY[i]<-1.0)
            {
                enemyBulletActive[i]=0;
            }
        }
    }

    moveEnemies();

    if(heartPowerActive==1)
    {
        heartPowerY=heartPowerY-heartPowerSpeed;

        if(heartPowerY<-1.0)
        {
            heartPowerActive=0;
        }
    }

    checkPlayerBulletCollision();
    checkPlayerDamage();
    checkHeartPower();

    if(playerDamageDelay>0)
    {
        playerDamageDelay--;
    }

    if(shootDelay>0)
    {
        shootDelay--;
    }

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            if(enemyShootDelay[i]>0)
            {
                enemyShootDelay[i]--;
            }
            else
            {
                fireEnemyBullet(i);
                enemyShootDelay[i]=90;
            }
        }
    }

    checkLevel2Complete();

    ::updateHighScore(score);

    glutPostRedisplay();
    glutTimerFunc(30,update,value);
}

//====================================================
// LEVEL 2 KEYBOARD CONTROL
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        ::showMainMenu();
        return;
    }

    if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }

    if(gameOver==1)
    {
        if(key=='r' || key=='R')
        {
            startLevel2();
            score=::carriedScore;
            playerLife=::carriedLife;
            glutPostRedisplay();
        }
        else if(key=='n' || key=='N')
        {
            ::showLevel1();
        }

        return;
    }

    if(levelComplete==1)
    {
        return;
    }

    if(key==' ')
    {
        if(shootDelay==0)
        {
            fireBullet();
            shootDelay=5;
        }
    }
}

void specialKey(int key,int x,int y)
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=1;
    }

    glutPostRedisplay();
}

void specialKeyUp(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=0;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=0;
    }
}

//====================================================
// LEVEL 2 DISPLAY
//====================================================

void display()
{
    char scoreText[30];

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawStars();

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            enemyShip(enemyX[i],enemyY[i]);
        }
    }

    if(gameOver==0)
    {
        playerShip();
    }

    drawPlayerBullets();
    drawEnemyBullets();

    if(heartPowerActive==1)
    {
        drawHeart(heartPowerX,heartPowerY);
    }

    drawBlasts();

    glColor3f(1,1,1);

    sprintf(scoreText,"SCORE: %03d",score);

    renderBitmapString(
        -0.9,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        scoreText
    );

    renderBitmapString(
        -0.05,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LEVEL: 2"
    );

    renderBitmapString(
        0.55,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LIFE:"
    );

    if(playerLife>=1)
    {
        drawHeart(0.72,0.91);
    }

    if(playerLife>=2)
    {
        drawHeart(0.82,0.91);
    }

    if(playerLife>=3)
    {
        drawHeart(0.92,0.91);
    }

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        -0.9,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Menu=Esc"
    );

    renderBitmapString(
        0.72,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "C=CLOSE"
    );

    if(gameOver==1)
    {
        drawCenterMessage(
            "GAME OVER",
            "R=RETRY  N=NEW",
            1,
            0,
            0
        );
    }

    if(
        levelComplete==1 &&
        enemyAlive[0]==0 &&
        enemyAlive[1]==0
      )
    {
        drawCenterMessage(
            "LEVEL 2 COMPLETE",
            "GOING TO NEXT LEVEL",
            0,
            1,
            0
        );
    }

    glFlush();
}

//====================================================
// SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    Opening::sound();
}

//====================================================
// SAME gluOrtho2D COORDINATE SYSTEM AS FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// LEVEL 2 ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Level2
// LEVEL2 FULL CPP SECTION END
#undef MAX_BLASTS
#undef BLAST_TIME
#undef PI
#undef MAX_ENEMIES
#undef MAX_BULLETS
#undef MAX_ENEMY_BULLETS

//============================================================
// LEVEL3 FULL CPP SECTION START
//============================================================
namespace Level3
{

#define PI 3.14159265358979323846

//====================================================
// MEMBER 3 : SNEHA DEB GUPTA
// ID       : 23-54496-3
// LEVEL 3  : ASTEROID AND SHIELD LEVEL
//====================================================

int currentLevel = 3;
int levelComplete = 0;
int gameOver = 0;

float playerX = 0.0;
int playerLife = 3;
int playerDamageDelay = 0;
int leftKeyPressed = 0;
int rightKeyPressed = 0;
int score = 0;

//====================================================
// LEVEL 3 ENEMIES
//====================================================

#define MAX_ENEMIES 2

float enemyX[MAX_ENEMIES];
float enemyY[MAX_ENEMIES];
float enemySpeed[MAX_ENEMIES];
int enemyDirection[MAX_ENEMIES];
int enemyAlive[MAX_ENEMIES];
int enemyHealth[MAX_ENEMIES];

//====================================================
// PLAYER BULLETS
//====================================================

#define MAX_BULLETS 12

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
float bulletSpeed = 0.02;
int shootDelay = 0;

//====================================================
// SIDE ENEMY BULLETS
//====================================================

#define MAX_ENEMY_BULLETS 2

float enemyBulletX[MAX_ENEMY_BULLETS];
float enemyBulletY[MAX_ENEMY_BULLETS];
int enemyBulletActive[MAX_ENEMY_BULLETS];
float enemyBulletSpeed = 0.015;
int enemyShootDelay[MAX_ENEMIES];

//====================================================
// LEVEL 3 ASTEROIDS
//====================================================

#define MAX_ASTEROIDS 4

float asteroidX[MAX_ASTEROIDS];
float asteroidY[MAX_ASTEROIDS];
float asteroidSize[MAX_ASTEROIDS];
float asteroidSpeed[MAX_ASTEROIDS];
float angle = 0.0;

//====================================================
// LEVEL 3 SHIELD POWER-UP
//====================================================

#define LEVEL3_SHIELD_ACTIVE_TIME 220

float shieldPowerX = 0.0;
float shieldPowerY = 1.05;
float shieldPowerSpeed = 0.003;
int shieldAvailable = 0;
int shieldActive = 0;
int shieldRewardGiven = 0;
int shieldTimer = 0;

//====================================================
// LEVEL 3 SOUND EFFECTS AND BLASTS
//====================================================

#define MAX_BLASTS 16
#define BLAST_TIME 16

float blastX[MAX_BLASTS];
float blastY[MAX_BLASTS];
float blastSize[MAX_BLASTS];
int blastTimer[MAX_BLASTS];

//====================================================
// COMMON SMALL HELPER FUNCTIONS USED BY LEVEL 3
//====================================================

float getPlayerY()
{
    return -0.65;
}

float getPlayerSpeed()
{
    return 0.015;
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(float y,void *font,const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

void drawCircle(GLfloat x,GLfloat y,GLfloat radius)
{
    int i;
    int triangleAmount=100;
    GLfloat twicePi=2.0f*PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(i=0;i<=triangleAmount;i++)
    {
        glVertex2f(
            x+(radius*cos(i*twicePi/triangleAmount)),
            y+(radius*sin(i*twicePi/triangleAmount))
        );
    }

    glEnd();
}

void drawCenterMessage(
    const char *line1,
    const char *line2,
    float r,
    float g,
    float b)
{
    glColor3f(0.04,0.04,0.04);

    glBegin(GL_QUADS);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);

    if(line2==NULL)
    {
        renderCenterBitmapString(
            0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );
    }
    else
    {
        renderCenterBitmapString(
            0.08,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );

        renderCenterBitmapString(
            -0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line2
        );
    }
}

//====================================================
// SIMPLE BLAST FUNCTIONS
//====================================================

void startBlast(float x,float y,float size)
{
    int slot=0;

    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]==0)
        {
            slot=i;
            break;
        }

        if(blastTimer[i]<blastTimer[slot])
        {
            slot=i;
        }
    }

    blastX[slot]=x;
    blastY[slot]=y;
    blastSize[slot]=size;
    blastTimer[slot]=BLAST_TIME;
    ::playBlastSound();
}

void updateBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            blastTimer[i]--;
        }
    }
}

void drawBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            float fade=(float)blastTimer[i]/BLAST_TIME;
            float radius=blastSize[i]*(0.25f+(1.0f-fade)*0.75f);

            glColor3f(fade,0.18f*fade,0);
            drawCircle(blastX[i],blastY[i],radius);

            glColor3f(fade,0.55f*fade,0.05f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.65f);

            glColor3f(fade,0.95f*fade,0.45f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.30f);

            glLineWidth(2);
            glColor3f(fade,0.70f*fade,0.10f*fade);

            glBegin(GL_LINES);

            for(int j=0;j<8;j++)
            {
                float direction=j*2.0f*PI/8;

                glVertex2f(
                    blastX[i]+radius*0.75f*cos(direction),
                    blastY[i]+radius*0.75f*sin(direction)
                );

                glVertex2f(
                    blastX[i]+radius*1.25f*cos(direction),
                    blastY[i]+radius*1.25f*sin(direction)
                );
            }

            glEnd();
        }
    }
}

//====================================================
// LEVEL 3 DRAWING FUNCTIONS
//====================================================

void drawHeart(float x,float y)
{
    glColor3f(1,0,0);

    drawCircle(x-0.025,y,0.025);
    drawCircle(x+0.025,y,0.025);

    glBegin(GL_TRIANGLES);
    glVertex2f(x-0.05,y);
    glVertex2f(x+0.05,y);
    glVertex2f(x,y-0.07);
    glEnd();
}

void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.2,0.7);
    glVertex2f(0.2,0.6);
    glVertex2f(0.6,0.8);
    glVertex2f(-0.7,0.2);
    glVertex2f(-0.3,0.3);
    glVertex2f(0.4,0.2);
    glVertex2f(0.8,-0.2);
    glVertex2f(-0.9,-0.6);
    glVertex2f(-0.4,-0.8);
    glVertex2f(0.2,-0.7);
    glVertex2f(0.7,-0.6);
    glEnd();
}

void playerShip()
{
    glPushMatrix();
    glTranslatef(playerX,getPlayerY(),0);

    glColor3f(0.8,0.8,0.8);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.15);
    glVertex2f(-0.08,-0.05);
    glVertex2f(0.08,-0.05);
    glEnd();

    glColor3f(0.3,0.3,0.3);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,-0.02);
    glVertex2f(-0.18,-0.10);
    glVertex2f(-0.07,-0.12);
    glVertex2f(0.07,-0.02);
    glVertex2f(0.18,-0.10);
    glVertex2f(0.07,-0.12);
    glEnd();

    glColor3f(0,0.5,1);

    glBegin(GL_QUADS);
    glVertex2f(-0.03,0.05);
    glVertex2f(0.03,0.05);
    glVertex2f(0.03,-0.02);
    glVertex2f(-0.03,-0.02);
    glEnd();

    glColor3f(0,0,1);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,-0.12);
    glVertex2f(0.03,-0.12);
    glVertex2f(0,-0.20);
    glEnd();

    glPopMatrix();
}

void enemyShip(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(0.9,0.1,0.1);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.12);
    glVertex2f(-0.08,0.08);
    glVertex2f(0.08,0.08);
    glEnd();

    glColor3f(0.5,0.5,0.5);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,0.03);
    glVertex2f(-0.18,0.10);
    glVertex2f(-0.08,0.12);
    glVertex2f(0.07,0.03);
    glVertex2f(0.18,0.10);
    glVertex2f(0.08,0.12);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.02,-0.06);
    glVertex2f(0.02,-0.06);
    glVertex2f(0.02,0.00);
    glVertex2f(-0.02,0.00);
    glEnd();

    glColor3f(1,0.5,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,0.12);
    glVertex2f(0.03,0.12);
    glVertex2f(0,0.20);
    glEnd();

    glPopMatrix();
}



void drawAsteroidShape(float size)
{
    glColor3f(0.45,0.45,0.45);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00*size,0.15*size);
    glVertex2f(-0.70*size,0.75*size);
    glVertex2f(-0.15*size,1.00*size);
    glVertex2f(0.55*size,0.80*size);
    glVertex2f(1.00*size,0.25*size);
    glVertex2f(0.85*size,-0.50*size);
    glVertex2f(0.30*size,-0.90*size);
    glVertex2f(-0.40*size,-0.85*size);
    glVertex2f(-0.90*size,-0.40*size);
    glEnd();

    glColor3f(0.25,0.25,0.25);
    drawCircle(-0.30*size,0.30*size,0.20*size);
    drawCircle(0.40*size,-0.20*size,0.15*size);

    glColor3f(0.33,0.33,0.33);
    drawCircle(0.30*size,0.45*size,0.10*size);
}

void asteroid(float x,float y,float size)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(angle,0,0,1);
    drawAsteroidShape(size);
    glPopMatrix();
}

void drawShieldPowerUp(float x,float y)
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            x+0.10*cos(i*2*PI/amount),
            y+0.10*sin(i*2*PI/amount)
        );
    }

    glEnd();
}

void drawActiveShield()
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            playerX+0.20*cos(i*2*PI/amount),
            getPlayerY()+0.20*sin(i*2*PI/amount)
        );
    }

    glEnd();
}


//====================================================
// LEVEL 3 BULLET DRAWING AND CREATION
//====================================================

void drawPlayerBullets()
{
    glColor3f(0,0.7,1);
    glLineWidth(4);

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bulletX[i],bulletY[i]);
            glVertex2f(bulletX[i],bulletY[i]+0.12);
            glEnd();
        }
    }
}

void createPlayerBullet(float x,float y)
{
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            bulletActive[i]=1;
            bulletX[i]=x;
            bulletY[i]=y;
            break;
        }
    }
}

void fireBullet()
{
    int freeBullet=0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    float startY=-0.35;

    createPlayerBullet(playerX,startY);

    ::playShootSound();
}

void drawEnemyBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]-0.12);
            glEnd();
        }
    }
}

void fireEnemyBullet(int enemyNumber)
{
    if(
        enemyAlive[enemyNumber]==1 &&
        enemyBulletActive[enemyNumber]==0
      )
    {
        enemyBulletActive[enemyNumber]=1;
        enemyBulletX[enemyNumber]=enemyX[enemyNumber];
        enemyBulletY[enemyNumber]=enemyY[enemyNumber]-0.12;
        ::playEnemyShootSound();
    }
}




//====================================================
// LEVEL 3 INITIAL SETUP
//====================================================

void startLevel3()
{
    ::initSoundEffects();
    ::stopSoundEffects();

    for(int i=0;i<MAX_BLASTS;i++)
    {
        blastTimer[i]=0;
    }

    currentLevel=3;
    levelComplete=0;
    gameOver=0;

    playerX=0.0;
    playerLife=3;
    playerDamageDelay=0;
    leftKeyPressed=0;
    rightKeyPressed=0;
    shootDelay=0;

    enemyX[0]=-0.35;
    enemyY[0]=0.48;
    enemySpeed[0]=0.0035;
    enemyDirection[0]=1;
    enemyAlive[0]=1;
    enemyHealth[0]=100;

    enemyX[1]=0.35;
    enemyY[1]=0.48;
    enemySpeed[1]=0.0035;
    enemyDirection[1]=-1;
    enemyAlive[1]=1;
    enemyHealth[1]=100;

    enemyShootDelay[0]=90;
    enemyShootDelay[1]=130;

    shieldAvailable=0;
    shieldActive=0;
    shieldRewardGiven=0;
    shieldTimer=0;
    shieldPowerX=0.0;
    shieldPowerY=1.05;
    shieldPowerSpeed=0.003;

    asteroidX[0]=-0.72;
    asteroidY[0]=0.90;
    asteroidSize[0]=0.09;
    asteroidSpeed[0]=0.002;

    asteroidX[1]=-0.25;
    asteroidY[1]=0.45;
    asteroidSize[1]=0.08;
    asteroidSpeed[1]=0.0025;

    asteroidX[2]=0.25;
    asteroidY[2]=0.10;
    asteroidSize[2]=0.07;
    asteroidSpeed[2]=0.0018;

    asteroidX[3]=0.72;
    asteroidY[3]=0.70;
    asteroidSize[3]=0.09;
    asteroidSpeed[3]=0.0022;

    angle=0.0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i]=0;
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        enemyBulletActive[i]=0;
    }

}

//====================================================
// LEVEL 3 MOVEMENT FUNCTIONS
//====================================================

void movePlayer()
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(leftKeyPressed==1)
    {
        playerX=playerX-getPlayerSpeed();

        if(playerX<-0.75)
        {
            playerX=-0.75;
        }
    }

    if(rightKeyPressed==1)
    {
        playerX=playerX+getPlayerSpeed();

        if(playerX>0.75)
        {
            playerX=0.75;
        }
    }
}

void moveEnemies()
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            enemyX[i]=
                enemyX[i]+enemySpeed[i]*enemyDirection[i];

            if(i==0)
            {
                if(enemyX[i]<-0.60)
                {
                    enemyX[i]=-0.60;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>-0.30)
                {
                    enemyX[i]=-0.30;
                    enemyDirection[i]=-1;
                }
            }

            if(i==1)
            {
                if(enemyX[i]<0.30)
                {
                    enemyX[i]=0.30;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>0.60)
                {
                    enemyX[i]=0.60;
                    enemyDirection[i]=-1;
                }
            }
        }
    }
}



void moveAsteroids()
{
    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        asteroidY[i]=asteroidY[i]-asteroidSpeed[i];

        if(asteroidY[i]<-1.10)
        {
            asteroidY[i]=1.05;
        }
    }
}

//====================================================
// LEVEL 3 COMPLETE CONDITION
//====================================================

void checkLevel3Complete()
{
    if(
        enemyAlive[0]==0 &&
        enemyAlive[1]==0 &&
        levelComplete==0
      )
    {
        levelComplete=1;

        enemyBulletActive[0]=0;
        enemyBulletActive[1]=0;
    }
}

//====================================================
// LEVEL 3 COLLISION FUNCTIONS
//====================================================

void checkPlayerBulletCollision()
{
    for(int e=0;e<MAX_ENEMIES;e++)
    {
        if(enemyAlive[e]==1)
        {
            for(int i=0;i<MAX_BULLETS;i++)
            {
                if(bulletActive[i]==1)
                {
                    if(
                        bulletX[i]>enemyX[e]-0.08 &&
                        bulletX[i]<enemyX[e]+0.08 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.08
                      )
                    {
                        enemyHealth[e]=enemyHealth[e]-20;
                        score=score+20;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]-0.18 &&
                        bulletX[i]<enemyX[e]-0.07 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]+0.07 &&
                        bulletX[i]<enemyX[e]+0.18 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }

                    if(enemyHealth[e]<=0)
                    {
                        enemyHealth[e]=0;
                        enemyAlive[e]=0;
                        enemyBulletActive[e]=0;
                        startBlast(enemyX[e],enemyY[e],0.22);
                        checkLevel3Complete();
                        break;
                    }
                }
            }
        }
    }
}


void checkAsteroidBulletCollision()
{
    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bulletActive[i]==1)
            {
                float dx=bulletX[i]-asteroidX[a];
                float dy=bulletY[i]-asteroidY[a];
                float distance=sqrt(dx*dx+dy*dy);

                if(distance<asteroidSize[a]+0.03)
                {
                    bulletActive[i]=0;
                    score=score+30;
                    ::showAsteroidFact(3,a);

                    // FIRST DESTROYED ASTEROID DROPS THE SHIELD

                    if(shieldRewardGiven==0)
                    {
                        shieldRewardGiven=1;
                        shieldAvailable=1;
                        shieldActive=0;
                        shieldTimer=0;
                        shieldPowerX=asteroidX[a];
                        shieldPowerY=1.05;
                    }

                    startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
                    asteroidY[a]=1.05;
                    break;
                }
            }
        }
    }
}

void damagePlayer()
{
    if(gameOver==1)
    {
        return;
    }

    playerLife=playerLife-1;
    playerDamageDelay=30;

    if(playerLife<=0)
    {
        playerLife=0;
        gameOver=1;
        startBlast(playerX,getPlayerY(),0.22);
        leftKeyPressed=0;
        rightKeyPressed=0;
    }
}

void checkPlayerDamage()
{
    float playerY=getPlayerY();

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(
            enemyBulletActive[i]==1 &&
            playerDamageDelay==0
          )
        {
            if(
                enemyBulletX[i]>playerX-0.18 &&
                enemyBulletX[i]<playerX+0.18 &&
                enemyBulletY[i]>playerY-0.23 &&
                enemyBulletY[i]<playerY+0.17
              )
            {
                enemyBulletActive[i]=0;

                if(shieldActive==1)
                {
                    playerDamageDelay=10;
                }
                else
                {
                    damagePlayer();
                }
            }
        }
    }
}


void checkAsteroidPlayerCollision()
{
    if(playerDamageDelay>0)
    {
        return;
    }

    float playerY=getPlayerY();

    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        float dx=asteroidX[a]-playerX;
        float dy=asteroidY[a]-playerY;
        float distance=sqrt(dx*dx+dy*dy);

        if(distance<asteroidSize[a]+0.16)
        {
            startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
            asteroidY[a]=1.05;

            if(shieldActive==1)
            {
                playerDamageDelay=10;
            }
            else
            {
                damagePlayer();
            }

            break;
        }
    }
}


void checkShieldPower()
{
    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        if(
            shieldPowerX>playerX-0.18 &&
            shieldPowerX<playerX+0.18 &&
            shieldPowerY>getPlayerY()-0.12 &&
            shieldPowerY<getPlayerY()+0.18
          )
        {
            shieldAvailable=0;
            shieldActive=1;
            shieldTimer=LEVEL3_SHIELD_ACTIVE_TIME;
        }
    }
}


//====================================================
// LEVEL 3 UPDATE LOOP
//====================================================

void update(int value)
{
    // Stop this level's timer after another display becomes active.
    if(::activeScene!=3 || value!=::levelTimerVersion)
    {
        return;
    }

    updateBlasts();

    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        glutPostRedisplay();
        glutTimerFunc(30,update,value);
        return;
    }

    movePlayer();

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            bulletY[i]=bulletY[i]+bulletSpeed;

            if(bulletY[i]>1.0)
            {
                bulletActive[i]=0;
            }
        }
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            enemyBulletY[i]=enemyBulletY[i]-enemyBulletSpeed;

            if(enemyBulletY[i]<-1.0)
            {
                enemyBulletActive[i]=0;
            }
        }
    }

    moveEnemies();
    moveAsteroids();

    angle=angle+2;

    if(angle>360)
    {
        angle=0;
    }

    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        shieldPowerY=shieldPowerY-shieldPowerSpeed;

        if(shieldPowerY<-1.0)
        {
            shieldAvailable=0;
        }
    }

    if(shieldActive==1)
    {
        if(shieldTimer>0)
        {
            shieldTimer--;
        }
        else
        {
            shieldActive=0;
        }
    }

    checkPlayerBulletCollision();
    checkAsteroidBulletCollision();
    checkPlayerDamage();
    checkAsteroidPlayerCollision();
    checkShieldPower();

    if(playerDamageDelay>0)
    {
        playerDamageDelay--;
    }

    if(shootDelay>0)
    {
        shootDelay--;
    }

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            if(enemyShootDelay[i]>0)
            {
                enemyShootDelay[i]--;
            }
            else
            {
                fireEnemyBullet(i);
                enemyShootDelay[i]=90;
            }
        }
    }

    checkLevel3Complete();

    ::updateHighScore(score);

    glutPostRedisplay();
    glutTimerFunc(30,update,value);
}

//====================================================
// LEVEL 3 KEYBOARD CONTROL
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        ::showMainMenu();
        return;
    }

    if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }

    if(gameOver==1)
    {
        if(key=='r' || key=='R')
        {
            startLevel3();
            score=::carriedScore;
            playerLife=::carriedLife;
            glutPostRedisplay();
        }
        else if(key=='n' || key=='N')
        {
            ::showLevel1();
        }

        return;
    }

    if(levelComplete==1)
    {
        return;
    }

    if(key==' ')
    {
        if(shootDelay==0)
        {
            fireBullet();
            shootDelay=5;
        }
    }
}

void specialKey(int key,int x,int y)
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=1;
    }

    glutPostRedisplay();
}

void specialKeyUp(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=0;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=0;
    }
}

//====================================================
// LEVEL 3 DISPLAY
//====================================================

void display()
{
    char scoreText[30];

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawStars();

    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        asteroid(
            asteroidX[i],
            asteroidY[i],
            asteroidSize[i]
        );
        ::drawAsteroidLabel(3,i,asteroidX[i],asteroidY[i]-asteroidSize[i]-0.06f);
    }

    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            enemyShip(enemyX[i],enemyY[i]);
        }
    }

    if(gameOver==0)
    {
        playerShip();
    }

    if(shieldActive==1)
    {
        drawActiveShield();
    }

    drawPlayerBullets();
    drawEnemyBullets();

    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        drawShieldPowerUp(shieldPowerX,shieldPowerY);
    }

    drawBlasts();

    glColor3f(1,1,1);

    sprintf(scoreText,"SCORE: %03d",score);

    renderBitmapString(
        -0.9,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        scoreText
    );

    renderBitmapString(
        -0.05,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LEVEL: 3"
    );

    renderBitmapString(
        0.55,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LIFE:"
    );

    if(playerLife>=1)
    {
        drawHeart(0.72,0.91);
    }

    if(playerLife>=2)
    {
        drawHeart(0.82,0.91);
    }

    if(playerLife>=3)
    {
        drawHeart(0.92,0.91);
    }

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        -0.9,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Menu=Esc"
    );

    renderBitmapString(
        0.72,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "C=CLOSE"
    );

    if(shieldActive==1)
    {
        glColor3f(0,0.8,1);

        renderBitmapString(
            0.30,
            0.86,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "SHIELD ACTIVE"
        );
    }

    if(gameOver==1)
    {
        drawCenterMessage(
            "GAME OVER",
            "R=RETRY  N=NEW",
            1,
            0,
            0
        );
    }

    if(
        levelComplete==1 &&
        enemyAlive[0]==0 &&
        enemyAlive[1]==0
      )
    {
        drawCenterMessage(
            "LEVEL 3 COMPLETE",
            "GOING TO NEXT LEVEL",
            0,
            1,
            0
        );
    }

    ::drawFactPopup();

    glFlush();
}

//====================================================
// SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    Opening::sound();
}

//====================================================
// SAME gluOrtho2D COORDINATE SYSTEM AS FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// LEVEL 3 ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Level3
// LEVEL3 FULL CPP SECTION END
#undef MAX_BLASTS
#undef BLAST_TIME
#undef PI
#undef MAX_ENEMIES
#undef MAX_BULLETS
#undef MAX_ENEMY_BULLETS
#undef MAX_ASTEROIDS
#undef LEVEL3_SHIELD_ACTIVE_TIME

//============================================================
// LEVEL4 FULL CPP SECTION START
//============================================================
namespace Level4
{

#define PI 3.14159265358979323846

//====================================================
// MEMBER 4 : LIYAN GHOSH
// ID       : 23-54016-3
// LEVEL 4  : BOSS BATTLE LEVEL
//====================================================

int currentLevel = 4;
int levelComplete = 0;
int gameOver = 0;

float playerX = 0.0;
int playerLife = 3;
int playerDamageDelay = 0;
int leftKeyPressed = 0;
int rightKeyPressed = 0;
int score = 0;

//====================================================
// PLAYER BULLETS
//====================================================

#define MAX_BULLETS 12

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
float bulletSpeed = 0.02;
int shootDelay = 0;

//====================================================
// LEVEL 4 ASTEROIDS
//====================================================

#define MAX_ASTEROIDS 4

float asteroidX[MAX_ASTEROIDS];
float asteroidY[MAX_ASTEROIDS];
float asteroidSize[MAX_ASTEROIDS];
float asteroidSpeed[MAX_ASTEROIDS];
float angle = 0.0;

//====================================================
// LEVEL 4 HEART POWER-UP
//====================================================

float heartPowerX = 0.0;
float heartPowerY = 1.05;
float heartPowerSpeed = 0.004;
int heartPowerActive = 0;

//====================================================
// LEVEL 4 SHIELD POWER-UP
//====================================================

#define LEVEL4_SHIELD_ACTIVE_TIME 360

float shieldPowerX = 0.0;
float shieldPowerY = 1.05;
float shieldPowerSpeed = 0.004;
int shieldAvailable = 0;
int shieldActive = 0;
int shieldTimer = 0;

//====================================================
// LEVEL 4 BOSS
//====================================================

float bossX = 0.0;
float bossY = 0.42;
float bossSpeed = 0.004;
int bossDirection = 1;
int bossHealth = 300;
int bossMaxHealth = 300;
int bossAlive = 1;
int bossShootDelay = 70;

//====================================================
// LEVEL 4 BOSS BULLETS
//====================================================

#define MAX_BOSS_BULLETS 15

float bossBulletX[MAX_BOSS_BULLETS];
float bossBulletY[MAX_BOSS_BULLETS];
float bossBulletDX[MAX_BOSS_BULLETS];
int bossBulletActive[MAX_BOSS_BULLETS];
float bossBulletSpeed = 0.010;

//====================================================
// LEVEL 4 BULLET POWER-UP
//====================================================

#define BULLET_POWER_ACTIVE_TIME 240

float bulletPowerX = 0.0;
float bulletPowerY = 1.05;
float bulletPowerSpeed = 0.004;
int bulletPowerAvailable = 0;
int bulletPowerActive = 0;
int bulletPowerTimer = 0;
int asteroidDestroyCount = 0;

//====================================================
// LEVEL 4 SOUND EFFECTS AND BLASTS
//====================================================

#define MAX_BLASTS 16
#define BLAST_TIME 16

float blastX[MAX_BLASTS];
float blastY[MAX_BLASTS];
float blastSize[MAX_BLASTS];
int blastTimer[MAX_BLASTS];

//====================================================
// COMMON SMALL HELPER FUNCTIONS USED BY LEVEL 4
//====================================================

float getPlayerY()
{
    return -0.68;
}

float getPlayerSpeed()
{
    return 0.015;
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(float y,void *font,const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

void drawCircle(GLfloat x,GLfloat y,GLfloat radius)
{
    int i;
    int triangleAmount=100;
    GLfloat twicePi=2.0f*PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(i=0;i<=triangleAmount;i++)
    {
        glVertex2f(
            x+(radius*cos(i*twicePi/triangleAmount)),
            y+(radius*sin(i*twicePi/triangleAmount))
        );
    }

    glEnd();
}

void drawCenterMessage(
    const char *line1,
    const char *line2,
    float r,
    float g,
    float b)
{
    glColor3f(0.04,0.04,0.04);

    glBegin(GL_QUADS);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);

    if(line2==NULL)
    {
        renderCenterBitmapString(
            0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );
    }
    else
    {
        renderCenterBitmapString(
            0.08,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );

        renderCenterBitmapString(
            -0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line2
        );
    }
}

//====================================================
// SIMPLE BLAST FUNCTIONS
//====================================================

void startBlast(float x,float y,float size)
{
    int slot=0;

    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]==0)
        {
            slot=i;
            break;
        }

        if(blastTimer[i]<blastTimer[slot])
        {
            slot=i;
        }
    }

    blastX[slot]=x;
    blastY[slot]=y;
    blastSize[slot]=size;
    blastTimer[slot]=BLAST_TIME;
    ::playBlastSound();
}

void updateBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            blastTimer[i]--;
        }
    }
}

void drawBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            float fade=(float)blastTimer[i]/BLAST_TIME;
            float radius=blastSize[i]*(0.25f+(1.0f-fade)*0.75f);

            glColor3f(fade,0.18f*fade,0);
            drawCircle(blastX[i],blastY[i],radius);

            glColor3f(fade,0.55f*fade,0.05f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.65f);

            glColor3f(fade,0.95f*fade,0.45f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.30f);

            glLineWidth(2);
            glColor3f(fade,0.70f*fade,0.10f*fade);

            glBegin(GL_LINES);

            for(int j=0;j<8;j++)
            {
                float direction=j*2.0f*PI/8;

                glVertex2f(
                    blastX[i]+radius*0.75f*cos(direction),
                    blastY[i]+radius*0.75f*sin(direction)
                );

                glVertex2f(
                    blastX[i]+radius*1.25f*cos(direction),
                    blastY[i]+radius*1.25f*sin(direction)
                );
            }

            glEnd();
        }
    }
}

//====================================================
// LEVEL 4 DRAWING FUNCTIONS
//====================================================

void drawHeart(float x,float y)
{
    glColor3f(1,0,0);

    drawCircle(x-0.025,y,0.025);
    drawCircle(x+0.025,y,0.025);

    glBegin(GL_TRIANGLES);
    glVertex2f(x-0.05,y);
    glVertex2f(x+0.05,y);
    glVertex2f(x,y-0.07);
    glEnd();
}

void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.2,0.7);
    glVertex2f(0.2,0.6);
    glVertex2f(0.6,0.8);
    glVertex2f(-0.7,0.2);
    glVertex2f(-0.3,0.3);
    glVertex2f(0.4,0.2);
    glVertex2f(0.8,-0.2);
    glVertex2f(-0.9,-0.6);
    glVertex2f(-0.4,-0.8);
    glVertex2f(0.2,-0.7);
    glVertex2f(0.7,-0.6);
    glEnd();
}

void playerShip()
{
    glPushMatrix();
    glTranslatef(playerX,getPlayerY(),0);

    glColor3f(0.8,0.8,0.8);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.15);
    glVertex2f(-0.08,-0.05);
    glVertex2f(0.08,-0.05);
    glEnd();

    glColor3f(0.3,0.3,0.3);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,-0.02);
    glVertex2f(-0.18,-0.10);
    glVertex2f(-0.07,-0.12);
    glVertex2f(0.07,-0.02);
    glVertex2f(0.18,-0.10);
    glVertex2f(0.07,-0.12);
    glEnd();

    glColor3f(0,0.5,1);

    glBegin(GL_QUADS);
    glVertex2f(-0.03,0.05);
    glVertex2f(0.03,0.05);
    glVertex2f(0.03,-0.02);
    glVertex2f(-0.03,-0.02);
    glEnd();

    glColor3f(0,0,1);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,-0.12);
    glVertex2f(0.03,-0.12);
    glVertex2f(0,-0.20);
    glEnd();

    glPopMatrix();
}


void bossShip()
{
    glPushMatrix();
    glTranslatef(bossX,bossY,0);

    // MAIN BODY - FACING DOWN

    glColor3f(0.35,0.35,0.35);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.25);
    glVertex2f(-0.22,0.10);
    glVertex2f(0.22,0.10);
    glEnd();

    // WINGS

    glColor3f(0.15,0.15,0.15);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15,-0.05);
    glVertex2f(-0.36,0.15);
    glVertex2f(-0.15,0.18);
    glVertex2f(0.15,-0.05);
    glVertex2f(0.36,0.15);
    glVertex2f(0.15,0.18);
    glEnd();

    // RED CORE

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.04,-0.12);
    glVertex2f(0.04,-0.12);
    glVertex2f(0.04,0.02);
    glVertex2f(-0.04,0.02);
    glEnd();

    // ENGINE FLAME - LEFT

    glColor3f(1,0.4,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.13,0.16);
    glVertex2f(-0.07,0.16);
    glVertex2f(-0.10,0.25);
    glEnd();

    // ENGINE FLAME - RIGHT

    glBegin(GL_TRIANGLES);
    glVertex2f(0.07,0.16);
    glVertex2f(0.13,0.16);
    glVertex2f(0.10,0.25);
    glEnd();

    glPopMatrix();
}

void drawBossHealth()
{
    float healthWidth=
        ((float)bossHealth/(float)bossMaxHealth)*0.69;

    if(healthWidth<0)
    {
        healthWidth=0;
    }

    glColor3f(1,1,1);

    renderBitmapString(
        -0.13,
        0.82,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "BOSS HEALTH"
    );

    glColor3f(1,1,1);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.35,0.78);
    glVertex2f(0.35,0.78);
    glVertex2f(0.35,0.74);
    glVertex2f(-0.35,0.74);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.345,0.775);
    glVertex2f(-0.345+healthWidth,0.775);
    glVertex2f(-0.345+healthWidth,0.745);
    glVertex2f(-0.345,0.745);
    glEnd();
}

void drawAsteroidShape(float size)
{
    glColor3f(0.45,0.45,0.45);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00*size,0.15*size);
    glVertex2f(-0.70*size,0.75*size);
    glVertex2f(-0.15*size,1.00*size);
    glVertex2f(0.55*size,0.80*size);
    glVertex2f(1.00*size,0.25*size);
    glVertex2f(0.85*size,-0.50*size);
    glVertex2f(0.30*size,-0.90*size);
    glVertex2f(-0.40*size,-0.85*size);
    glVertex2f(-0.90*size,-0.40*size);
    glEnd();

    glColor3f(0.25,0.25,0.25);
    drawCircle(-0.30*size,0.30*size,0.20*size);
    drawCircle(0.40*size,-0.20*size,0.15*size);

    glColor3f(0.33,0.33,0.33);
    drawCircle(0.30*size,0.45*size,0.10*size);
}

void asteroid(float x,float y,float size)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(angle,0,0,1);
    drawAsteroidShape(size);
    glPopMatrix();
}

void drawShieldPowerUp(float x,float y)
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            x+0.10*cos(i*2*PI/amount),
            y+0.10*sin(i*2*PI/amount)
        );
    }

    glEnd();
}

void drawActiveShield()
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            playerX+0.20*cos(i*2*PI/amount),
            getPlayerY()+0.20*sin(i*2*PI/amount)
        );
    }

    glEnd();
}

void drawBulletPowerUp(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(1,1,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.06);
    glVertex2f(-0.025,0.02);
    glVertex2f(0.025,0.02);
    glEnd();

    glColor3f(1,0.7,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.025,0.02);
    glVertex2f(0.025,0.02);
    glVertex2f(0.025,-0.06);
    glVertex2f(-0.025,-0.06);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.025,-0.06);
    glVertex2f(0.025,-0.06);
    glVertex2f(0,-0.09);
    glEnd();

    glPopMatrix();
}

//====================================================
// LEVEL 4 BULLET DRAWING AND CREATION
//====================================================

void drawPlayerBullets()
{
    glColor3f(0,0.7,1);
    glLineWidth(4);

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bulletX[i],bulletY[i]);
            glVertex2f(bulletX[i],bulletY[i]+0.12);
            glEnd();
        }
    }
}

void createPlayerBullet(float x,float y)
{
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            bulletActive[i]=1;
            bulletX[i]=x;
            bulletY[i]=y;
            break;
        }
    }
}

void fireBullet()
{
    int freeBullet=0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    float startY=-0.38;

    createPlayerBullet(playerX,startY);

    ::playShootSound();
}



void drawBossBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bossBulletX[i],bossBulletY[i]);
            glVertex2f(bossBulletX[i],bossBulletY[i]-0.14);
            glEnd();
        }
    }
}

void createBossBullet(float x,float y,float dx)
{
    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==0)
        {
            bossBulletActive[i]=1;
            bossBulletX[i]=x;
            bossBulletY[i]=y;
            bossBulletDX[i]=dx;
            break;
        }
    }
}

void fireBossBullets()
{
    if(bossAlive==0)
    {
        return;
    }

    int freeBullet=0;

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    // PHASE 1: HP 300 TO 151 = SINGLE BULLET

    if(bossHealth>150)
    {
        createBossBullet(
            bossX,
            bossY-0.22,
            0.0
        );
    }

    // PHASE 2: HP 150 OR BELOW = THREE BULLETS

    else
    {
        createBossBullet(
            bossX-0.10,
            bossY-0.22,
            -0.003
        );

        createBossBullet(
            bossX,
            bossY-0.22,
            0.0
        );

        createBossBullet(
            bossX+0.10,
            bossY-0.22,
            0.003
        );
    }

    // Play once for the whole volley.
    ::playEnemyShootSound();
}

//====================================================
// LEVEL 4 INITIAL SETUP
//====================================================

void startLevel4()
{
    ::initSoundEffects();
    ::stopSoundEffects();

    for(int i=0;i<MAX_BLASTS;i++)
    {
        blastTimer[i]=0;
    }

    currentLevel=4;
    levelComplete=0;
    gameOver=0;

    playerX=0.0;
    playerLife=3;
    playerDamageDelay=0;
    leftKeyPressed=0;
    rightKeyPressed=0;
    shootDelay=0;

    bossX=0.0;
    bossY=0.42;
    bossSpeed=0.004;
    bossDirection=1;
    bossHealth=300;
    bossMaxHealth=300;
    bossAlive=1;
    bossShootDelay=70;
    bossBulletSpeed=0.010;

    heartPowerActive=0;
    shieldAvailable=0;
    shieldActive=0;
    shieldTimer=0;
    bulletPowerAvailable=0;
    bulletPowerActive=0;
    bulletPowerTimer=0;
    asteroidDestroyCount=0;

    asteroidX[0]=-0.72;
    asteroidY[0]=0.95;
    asteroidSize[0]=0.08;
    asteroidSpeed[0]=0.0026;

    asteroidX[1]=-0.25;
    asteroidY[1]=0.50;
    asteroidSize[1]=0.08;
    asteroidSpeed[1]=0.0030;

    asteroidX[2]=0.25;
    asteroidY[2]=0.15;
    asteroidSize[2]=0.07;
    asteroidSpeed[2]=0.0027;

    asteroidX[3]=0.72;
    asteroidY[3]=0.75;
    asteroidSize[3]=0.09;
    asteroidSpeed[3]=0.0032;

    angle=0.0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i]=0;
    }

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        bossBulletActive[i]=0;
    }
}

//====================================================
// LEVEL 4 MOVEMENT FUNCTIONS
//====================================================

void movePlayer()
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(leftKeyPressed==1)
    {
        playerX=playerX-getPlayerSpeed();

        if(playerX<-0.75)
        {
            playerX=-0.75;
        }
    }

    if(rightKeyPressed==1)
    {
        playerX=playerX+getPlayerSpeed();

        if(playerX>0.75)
        {
            playerX=0.75;
        }
    }
}


void moveBoss()
{
    if(bossAlive==0)
    {
        return;
    }

    if(bossHealth<=150)
    {
        bossSpeed=0.006;
    }
    else
    {
        bossSpeed=0.004;
    }

    bossX=bossX+bossSpeed*bossDirection;

    if(bossX>0.45)
    {
        bossX=0.45;
        bossDirection=-1;
    }

    if(bossX<-0.45)
    {
        bossX=-0.45;
        bossDirection=1;
    }
}

void moveBossBullets()
{
    if(bossHealth<=150)
    {
        bossBulletSpeed=0.013;
    }
    else
    {
        bossBulletSpeed=0.010;
    }

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            bossBulletX[i]=bossBulletX[i]+bossBulletDX[i];
            bossBulletY[i]=bossBulletY[i]-bossBulletSpeed;

            if(
                bossBulletY[i]<-1.0 ||
                bossBulletX[i]<-1.0 ||
                bossBulletX[i]>1.0
              )
            {
                bossBulletActive[i]=0;
            }
        }
    }
}

void moveAsteroids()
{
    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        float currentAsteroidSpeed=asteroidSpeed[i];

        if(bossHealth<=150)
        {
            currentAsteroidSpeed=currentAsteroidSpeed+0.001;
        }

        asteroidY[i]=asteroidY[i]-currentAsteroidSpeed;

        if(asteroidY[i]<-1.10)
        {
            asteroidY[i]=1.05;
        }
    }
}

//====================================================
// LEVEL 4 COLLISION FUNCTIONS
//====================================================


void checkBossBulletCollision()
{
    if(bossAlive==0)
    {
        return;
    }

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            if(
                bulletX[i]>bossX-0.36 &&
                bulletX[i]<bossX+0.36 &&
                bulletY[i]>bossY-0.28 &&
                bulletY[i]<bossY+0.22
              )
            {
                int damage;

                if(bulletPowerActive==1)
                {
                    damage=25;
                }
                else
                {
                    damage=10;
                }

                bossHealth=bossHealth-damage;
                score=score+damage;
                bulletActive[i]=0;

                if(bossHealth<=0)
                {
                    bossHealth=0;
                    bossAlive=0;
                    startBlast(bossX,bossY,0.38);
                    levelComplete=1;
                    break;
                }
            }
        }
    }
}

void checkAsteroidBulletCollision()
{
    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bulletActive[i]==1)
            {
                float dx=bulletX[i]-asteroidX[a];
                float dy=bulletY[i]-asteroidY[a];
                float distance=sqrt(dx*dx+dy*dy);

                if(distance<asteroidSize[a]+0.03)
                {
                    bulletActive[i]=0;
                    score=score+30;
                    ::showAsteroidFact(4,a);

                    if(
                        heartPowerActive==0 &&
                        shieldAvailable==0 &&
                        bulletPowerAvailable==0
                      )
                    {
                        asteroidDestroyCount++;

                        if(asteroidDestroyCount%3==1)
                        {
                            if(playerLife<3)
                            {
                                heartPowerActive=1;
                                heartPowerX=asteroidX[a];
                                heartPowerY=1.05;
                            }
                            else
                            {
                                bulletPowerAvailable=1;
                                bulletPowerX=asteroidX[a];
                                bulletPowerY=1.05;
                            }
                        }
                        else if(asteroidDestroyCount%3==2)
                        {
                            shieldAvailable=1;
                            shieldActive=0;
                            shieldTimer=0;
                            shieldPowerX=asteroidX[a];
                            shieldPowerY=1.05;
                        }
                        else
                        {
                            bulletPowerAvailable=1;
                            bulletPowerX=asteroidX[a];
                            bulletPowerY=1.05;
                        }
                    }

                    startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
                    asteroidY[a]=1.05;
                    break;
                }
            }
        }
    }
}

void damagePlayer()
{
    if(gameOver==1)
    {
        return;
    }

    playerLife=playerLife-1;
    playerDamageDelay=30;

    if(playerLife<=0)
    {
        playerLife=0;
        gameOver=1;
        startBlast(playerX,getPlayerY(),0.22);
        leftKeyPressed=0;
        rightKeyPressed=0;
    }
}


void checkBossPlayerDamage()
{
    if(playerDamageDelay>0)
    {
        return;
    }

    float playerY=getPlayerY();

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            if(
                bossBulletX[i]>playerX-0.18 &&
                bossBulletX[i]<playerX+0.18 &&
                bossBulletY[i]>playerY-0.23 &&
                bossBulletY[i]<playerY+0.17
              )
            {
                bossBulletActive[i]=0;

                if(shieldActive==1)
                {
                    playerDamageDelay=10;
                }
                else
                {
                    damagePlayer();
                }

                break;
            }
        }
    }
}

void checkAsteroidPlayerCollision()
{
    if(playerDamageDelay>0)
    {
        return;
    }

    float playerY=getPlayerY();

    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        float dx=asteroidX[a]-playerX;
        float dy=asteroidY[a]-playerY;
        float distance=sqrt(dx*dx+dy*dy);

        if(distance<asteroidSize[a]+0.16)
        {
            startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
            asteroidY[a]=1.05;

            if(shieldActive==1)
            {
                playerDamageDelay=10;
            }
            else
            {
                damagePlayer();
            }

            break;
        }
    }
}

void checkHeartPower()
{
    if(heartPowerActive==1)
    {
        if(
            heartPowerX>playerX-0.18 &&
            heartPowerX<playerX+0.18 &&
            heartPowerY>getPlayerY()-0.12 &&
            heartPowerY<getPlayerY()+0.18
          )
        {
            if(playerLife<3)
            {
                playerLife=playerLife+1;
                heartPowerActive=0;
            }
        }
    }
}

void checkShieldPower()
{
    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        if(
            shieldPowerX>playerX-0.18 &&
            shieldPowerX<playerX+0.18 &&
            shieldPowerY>getPlayerY()-0.12 &&
            shieldPowerY<getPlayerY()+0.18
          )
        {
            shieldAvailable=0;
            shieldActive=1;
            shieldTimer=LEVEL4_SHIELD_ACTIVE_TIME;
        }
    }
}

void checkBulletPower()
{
    if(
        bulletPowerAvailable==1 &&
        bulletPowerActive==0
      )
    {
        if(
            bulletPowerX>playerX-0.18 &&
            bulletPowerX<playerX+0.18 &&
            bulletPowerY>getPlayerY()-0.12 &&
            bulletPowerY<getPlayerY()+0.18
          )
        {
            bulletPowerAvailable=0;
            bulletPowerActive=1;
            bulletPowerTimer=BULLET_POWER_ACTIVE_TIME;
        }
    }
}

//====================================================
// LEVEL 4 UPDATE LOOP
//====================================================

void update(int value)
{
    // Stop this level's timer after another display becomes active.
    if(::activeScene!=4 || value!=::levelTimerVersion)
    {
        return;
    }

    updateBlasts();

    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        glutPostRedisplay();
        glutTimerFunc(30,update,value);
        return;
    }

    movePlayer();

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            bulletY[i]=bulletY[i]+bulletSpeed;

            if(bulletY[i]>1.0)
            {
                bulletActive[i]=0;
            }
        }
    }

    moveBossBullets();
    moveBoss();
    moveAsteroids();

    angle=angle+2;

    if(angle>360)
    {
        angle=0;
    }

    if(bossAlive==1)
    {
        if(bossShootDelay>0)
        {
            bossShootDelay--;
        }
        else
        {
            fireBossBullets();

            if(bossHealth<=150)
            {
                bossShootDelay=45;
            }
            else
            {
                bossShootDelay=70;
            }
        }
    }

    if(heartPowerActive==1)
    {
        heartPowerY=heartPowerY-heartPowerSpeed;

        if(heartPowerY<-1.0)
        {
            heartPowerActive=0;
        }
    }

    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        shieldPowerY=shieldPowerY-shieldPowerSpeed;

        if(shieldPowerY<-1.0)
        {
            shieldAvailable=0;
        }
    }

    if(shieldActive==1)
    {
        if(shieldTimer>0)
        {
            shieldTimer--;
        }
        else
        {
            shieldActive=0;
        }
    }

    if(
        bulletPowerAvailable==1 &&
        bulletPowerActive==0
      )
    {
        bulletPowerY=bulletPowerY-bulletPowerSpeed;

        if(bulletPowerY<-1.0)
        {
            bulletPowerAvailable=0;
        }
    }

    if(bulletPowerActive==1)
    {
        if(bulletPowerTimer>0)
        {
            bulletPowerTimer--;
        }
        else
        {
            bulletPowerActive=0;
        }
    }

    checkBossBulletCollision();
    checkAsteroidBulletCollision();
    checkBossPlayerDamage();
    checkAsteroidPlayerCollision();
    checkHeartPower();
    checkShieldPower();
    checkBulletPower();

    if(playerDamageDelay>0)
    {
        playerDamageDelay--;
    }

    if(shootDelay>0)
    {
        shootDelay--;
    }

    ::updateHighScore(score);

    glutPostRedisplay();
    glutTimerFunc(30,update,value);
}

//====================================================
// LEVEL 4 KEYBOARD CONTROL
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        ::showMainMenu();
        return;
    }

    if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }

    if(gameOver==1)
    {
        if(key=='r' || key=='R')
        {
            startLevel4();
            score=::carriedScore;
            playerLife=::carriedLife;
            glutPostRedisplay();
        }
        else if(key=='n' || key=='N')
        {
            ::showLevel1();
        }

        return;
    }

    if(levelComplete==1)
    {
        return;
    }

    if(key==' ')
    {
        if(shootDelay==0)
        {
            fireBullet();
            shootDelay=5;
        }
    }
}

void specialKey(int key,int x,int y)
{
    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        return;
    }

    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=1;
    }

    glutPostRedisplay();
}

void specialKeyUp(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=0;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=0;
    }
}

//====================================================
// LEVEL 4 DISPLAY
//====================================================

void display()
{
    char scoreText[30];

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawStars();

    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        asteroid(
            asteroidX[i],
            asteroidY[i],
            asteroidSize[i]
        );
        ::drawAsteroidLabel(4,i,asteroidX[i],asteroidY[i]-asteroidSize[i]-0.06f);
    }

    drawBossHealth();

    if(bossAlive==1)
    {
        bossShip();
    }

    if(gameOver==0)
    {
        playerShip();
    }

    if(shieldActive==1)
    {
        drawActiveShield();
    }

    drawPlayerBullets();
    drawBossBullets();

    if(heartPowerActive==1)
    {
        drawHeart(heartPowerX,heartPowerY);
    }

    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        drawShieldPowerUp(shieldPowerX,shieldPowerY);
    }

    if(
        bulletPowerAvailable==1 &&
        bulletPowerActive==0
      )
    {
        drawBulletPowerUp(bulletPowerX,bulletPowerY);
    }

    drawBlasts();

    glColor3f(1,1,1);

    sprintf(scoreText,"SCORE: %03d",score);

    renderBitmapString(
        -0.9,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        scoreText
    );

    renderBitmapString(
        -0.05,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LEVEL: 4"
    );

    renderBitmapString(
        0.55,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LIFE:"
    );

    if(playerLife>=1)
    {
        drawHeart(0.72,0.91);
    }

    if(playerLife>=2)
    {
        drawHeart(0.82,0.91);
    }

    if(playerLife>=3)
    {
        drawHeart(0.92,0.91);
    }

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        -0.9,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Menu=Esc"
    );

    renderBitmapString(
        0.72,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "C=CLOSE"
    );

    if(bulletPowerActive==1)
    {
        glColor3f(1,1,0);

        renderBitmapString(
            -0.54,
            0.86,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "BULLET POWER ACTIVE"
        );
    }

    if(shieldActive==1)
    {
        glColor3f(0,0.8,1);

        renderBitmapString(
            0.30,
            0.86,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "SHIELD ACTIVE"
        );
    }

    if(gameOver==1)
    {
        drawCenterMessage(
            "GAME OVER",
            "R=RETRY  N=NEW",
            1,
            0,
            0
        );
    }

    if(
        levelComplete==1 &&
        bossAlive==0
      )
    {
        drawCenterMessage(
            "LEVEL 4 COMPLETE",
            "GOING TO NEXT LEVEL",
            0,
            1,
            0
        );
    }

    ::drawFactPopup();

    glFlush();
}

//====================================================
// SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    Opening::sound();
}

//====================================================
// SAME gluOrtho2D COORDINATE SYSTEM AS FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// LEVEL 4 ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Level4
// LEVEL4 FULL CPP SECTION END
#undef MAX_BLASTS
#undef BLAST_TIME
#undef PI
#undef MAX_BULLETS
#undef MAX_ASTEROIDS
#undef LEVEL4_SHIELD_ACTIVE_TIME
#undef MAX_BOSS_BULLETS
#undef BULLET_POWER_ACTIVE_TIME

//============================================================
// LEVEL5 FULL CPP SECTION START
//============================================================
namespace Level5
{

#define PI 3.14159265358979323846

//====================================================
// MEMBER 5 : MD. TAUHIDUL ISLAM SIAM
// ID       : 23-53927-3
// LEVEL 5  : FINAL BOSS LEVEL
//====================================================

int currentLevel = 5;
int levelComplete = 0;
int gameOver = 0;
int gameWin = 0;

float playerX = 0.0;
int playerLife = 3;
int playerDamageDelay = 0;
int leftKeyPressed = 0;
int rightKeyPressed = 0;
int score = 0;

//====================================================
// LEVEL 5 SIDE ENEMIES
//====================================================

#define MAX_ENEMIES 2

float enemyX[MAX_ENEMIES];
float enemyY[MAX_ENEMIES];
float enemySpeed[MAX_ENEMIES];
int enemyDirection[MAX_ENEMIES];
int enemyAlive[MAX_ENEMIES];
int enemyHealth[MAX_ENEMIES];

//====================================================
// PLAYER BULLETS
//====================================================

#define MAX_BULLETS 12

float bulletX[MAX_BULLETS];
float bulletY[MAX_BULLETS];
int bulletActive[MAX_BULLETS];
float bulletSpeed = 0.02;
int shootDelay = 0;

//====================================================
// SIDE ENEMY BULLETS
//====================================================

#define MAX_ENEMY_BULLETS 2

float enemyBulletX[MAX_ENEMY_BULLETS];
float enemyBulletY[MAX_ENEMY_BULLETS];
int enemyBulletActive[MAX_ENEMY_BULLETS];
float enemyBulletSpeed = 0.008;
int enemyShootDelay[MAX_ENEMIES];
int level5SideEnemyTurn = 0;

//====================================================
// LEVEL 5 ASTEROIDS
//====================================================

#define MAX_ASTEROIDS 4

float asteroidX[MAX_ASTEROIDS];
float asteroidY[MAX_ASTEROIDS];
float asteroidSize[MAX_ASTEROIDS];
float asteroidSpeed[MAX_ASTEROIDS];
float angle = 0.0;

//====================================================
// LEVEL 5 HEART POWER-UP
//====================================================

float heartPowerX = 0.0;
float heartPowerY = 1.05;
float heartPowerSpeed = 0.006;
int heartPowerActive = 0;

//====================================================
// LEVEL 5 SHIELD POWER-UP
//====================================================

#define LEVEL5_SHIELD_ACTIVE_TIME 600

float shieldPowerX = 0.0;
float shieldPowerY = 1.05;
float shieldPowerSpeed = 0.006;
int shieldAvailable = 0;
int shieldActive = 0;
int shieldTimer = 0;

//====================================================
// LEVEL 5 FINAL BOSS
//====================================================

float bossX = 0.0;
float bossY = 0.42;
float bossSpeed = 0.0035;
int bossDirection = 1;
int bossHealth = 500;
int bossMaxHealth = 500;
int bossAlive = 1;
int bossShootDelay = 90;

//====================================================
// FINAL BOSS BULLETS
//====================================================

#define MAX_BOSS_BULLETS 15

float bossBulletX[MAX_BOSS_BULLETS];
float bossBulletY[MAX_BOSS_BULLETS];
float bossBulletDX[MAX_BOSS_BULLETS];
int bossBulletActive[MAX_BOSS_BULLETS];
float bossBulletSpeed = 0.008;

//====================================================
// LEVEL 5 BULLET POWER-UP
//====================================================

#define BULLET_POWER_ACTIVE_TIME 240

float bulletPowerX = 0.0;
float bulletPowerY = 1.05;
float bulletPowerSpeed = 0.006;
int bulletPowerAvailable = 0;
int bulletPowerActive = 0;
int bulletPowerTimer = 0;
int asteroidDestroyCount = 0;

//====================================================
// LEVEL 5 SOUND EFFECTS AND BLASTS
//====================================================

#define MAX_BLASTS 16
#define BLAST_TIME 16

float blastX[MAX_BLASTS];
float blastY[MAX_BLASTS];
float blastSize[MAX_BLASTS];
int blastTimer[MAX_BLASTS];

//====================================================
// COMMON SMALL HELPER FUNCTIONS USED BY LEVEL 5
//====================================================

float getPlayerY()
{
    return -0.68;
}

float getPlayerSpeed()
{
    return 0.015;
}

void renderBitmapString(float x, float y, float z, void *font, const char *string)
{
    const char *c;

    glRasterPos3f(x,y,z);

    for(c=string; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

void renderCenterBitmapString(float y,void *font,const char *string)
{
    int width=0;
    const char *c;

    for(c=string; *c!='\0'; c++)
    {
        width=width+glutBitmapWidth(font,*c);
    }

    int windowWidth=glutGet(GLUT_WINDOW_WIDTH);

    if(windowWidth<=0)
    {
        windowWidth=1200;
    }

    float x=-((float)width/(float)windowWidth);

    renderBitmapString(x,y,0,font,string);
}

void drawCircle(GLfloat x,GLfloat y,GLfloat radius)
{
    int i;
    int triangleAmount=100;
    GLfloat twicePi=2.0f*PI;

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x,y);

    for(i=0;i<=triangleAmount;i++)
    {
        glVertex2f(
            x+(radius*cos(i*twicePi/triangleAmount)),
            y+(radius*sin(i*twicePi/triangleAmount))
        );
    }

    glEnd();
}

void drawCenterMessage(
    const char *line1,
    const char *line2,
    float r,
    float g,
    float b)
{
    glColor3f(0.04,0.04,0.04);

    glBegin(GL_QUADS);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.42,0.18);
    glVertex2f(0.42,0.18);
    glVertex2f(0.42,-0.10);
    glVertex2f(-0.42,-0.10);
    glEnd();

    glColor3f(r,g,b);

    if(line2==NULL)
    {
        renderCenterBitmapString(
            0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );
    }
    else
    {
        renderCenterBitmapString(
            0.08,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line1
        );

        renderCenterBitmapString(
            -0.03,
            GLUT_BITMAP_TIMES_ROMAN_24,
            line2
        );
    }
}

//====================================================
// LEVEL 5 SOUND AND BLAST HELPERS
// Sound functions are shared by all five levels.
//====================================================

void startBlast(float x,float y,float size)
{
    int slot=0;

    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]==0)
        {
            slot=i;
            break;
        }

        if(blastTimer[i]<blastTimer[slot])
        {
            slot=i;
        }
    }

    blastX[slot]=x;
    blastY[slot]=y;
    blastSize[slot]=size;
    blastTimer[slot]=BLAST_TIME;
    ::playBlastSound();
}

void updateBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            blastTimer[i]--;
        }
    }
}

void drawBlasts()
{
    for(int i=0;i<MAX_BLASTS;i++)
    {
        if(blastTimer[i]>0)
        {
            float fade=(float)blastTimer[i]/BLAST_TIME;
            float radius=blastSize[i]*(0.25f+(1.0f-fade)*0.75f);

            glColor3f(fade,0.18f*fade,0);
            drawCircle(blastX[i],blastY[i],radius);

            glColor3f(fade,0.55f*fade,0.05f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.65f);

            glColor3f(fade,0.95f*fade,0.45f*fade);
            drawCircle(blastX[i],blastY[i],radius*0.30f);

            glLineWidth(2);
            glColor3f(fade,0.70f*fade,0.10f*fade);

            glBegin(GL_LINES);

            for(int j=0;j<8;j++)
            {
                float direction=j*2.0f*PI/8;

                glVertex2f(
                    blastX[i]+radius*0.75f*cos(direction),
                    blastY[i]+radius*0.75f*sin(direction)
                );

                glVertex2f(
                    blastX[i]+radius*1.25f*cos(direction),
                    blastY[i]+radius*1.25f*sin(direction)
                );
            }

            glEnd();
        }
    }
}

//====================================================
// LEVEL 5 DRAWING FUNCTIONS
//====================================================

void drawHeart(float x,float y)
{
    glColor3f(1,0,0);

    drawCircle(x-0.025,y,0.025);
    drawCircle(x+0.025,y,0.025);

    glBegin(GL_TRIANGLES);
    glVertex2f(x-0.05,y);
    glVertex2f(x+0.05,y);
    glVertex2f(x,y-0.07);
    glEnd();
}

void drawStars()
{
    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);
    glVertex2f(-0.8,0.8);
    glVertex2f(-0.5,0.5);
    glVertex2f(-0.2,0.7);
    glVertex2f(0.2,0.6);
    glVertex2f(0.6,0.8);
    glVertex2f(-0.7,0.2);
    glVertex2f(-0.3,0.3);
    glVertex2f(0.4,0.2);
    glVertex2f(0.8,-0.2);
    glVertex2f(-0.9,-0.6);
    glVertex2f(-0.4,-0.8);
    glVertex2f(0.2,-0.7);
    glVertex2f(0.7,-0.6);
    glEnd();
}

void playerShip()
{
    glPushMatrix();
    glTranslatef(playerX,getPlayerY(),0);

    glColor3f(0.8,0.8,0.8);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.15);
    glVertex2f(-0.08,-0.05);
    glVertex2f(0.08,-0.05);
    glEnd();

    glColor3f(0.3,0.3,0.3);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,-0.02);
    glVertex2f(-0.18,-0.10);
    glVertex2f(-0.07,-0.12);
    glVertex2f(0.07,-0.02);
    glVertex2f(0.18,-0.10);
    glVertex2f(0.07,-0.12);
    glEnd();

    glColor3f(0,0.5,1);

    glBegin(GL_QUADS);
    glVertex2f(-0.03,0.05);
    glVertex2f(0.03,0.05);
    glVertex2f(0.03,-0.02);
    glVertex2f(-0.03,-0.02);
    glEnd();

    glColor3f(0,0,1);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,-0.12);
    glVertex2f(0.03,-0.12);
    glVertex2f(0,-0.20);
    glEnd();

    glPopMatrix();
}

void enemyShip(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(0.9,0.1,0.1);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.12);
    glVertex2f(-0.08,0.08);
    glVertex2f(0.08,0.08);
    glEnd();

    glColor3f(0.5,0.5,0.5);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.07,0.03);
    glVertex2f(-0.18,0.10);
    glVertex2f(-0.08,0.12);
    glVertex2f(0.07,0.03);
    glVertex2f(0.18,0.10);
    glVertex2f(0.08,0.12);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.02,-0.06);
    glVertex2f(0.02,-0.06);
    glVertex2f(0.02,0.00);
    glVertex2f(-0.02,0.00);
    glEnd();

    glColor3f(1,0.5,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.03,0.12);
    glVertex2f(0.03,0.12);
    glVertex2f(0,0.20);
    glEnd();

    glPopMatrix();
}

void finalBossShip()
{
    glPushMatrix();
    glTranslatef(bossX,bossY,0);
    glScalef(0.85,0.85,1);

    glColor3f(0.30,0.30,0.30);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,-0.27);
    glVertex2f(-0.25,0.12);
    glVertex2f(0.25,0.12);
    glEnd();

    glColor3f(0.12,0.12,0.12);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.18,-0.05);
    glVertex2f(-0.45,0.18);
    glVertex2f(-0.18,0.21);
    glVertex2f(0.18,-0.05);
    glVertex2f(0.45,0.18);
    glVertex2f(0.18,0.21);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.05,-0.13);
    glVertex2f(0.05,-0.13);
    glVertex2f(0.05,0.02);
    glVertex2f(-0.05,0.02);
    glEnd();

    glColor3f(1,0.4,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.14,0.19);
    glVertex2f(-0.08,0.19);
    glVertex2f(-0.11,0.28);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex2f(0.08,0.19);
    glVertex2f(0.14,0.19);
    glVertex2f(0.11,0.28);
    glEnd();

    glPopMatrix();
}

void drawBossHealth()
{
    float healthWidth=
        ((float)bossHealth/(float)bossMaxHealth)*0.55;

    if(healthWidth<0)
    {
        healthWidth=0;
    }

    glColor3f(1,1,1);

    renderBitmapString(
        -0.17,
        0.82,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "FINAL BOSS HEALTH"
    );

    glColor3f(1,1,1);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.28,0.76);
    glVertex2f(0.28,0.76);
    glVertex2f(0.28,0.72);
    glVertex2f(-0.28,0.72);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.275,0.755);
    glVertex2f(-0.275+healthWidth,0.755);
    glVertex2f(-0.275+healthWidth,0.725);
    glVertex2f(-0.275,0.725);
    glEnd();
}

void drawAsteroidShape(float size)
{
    glColor3f(0.45,0.45,0.45);

    glBegin(GL_POLYGON);
    glVertex2f(-1.00*size,0.15*size);
    glVertex2f(-0.70*size,0.75*size);
    glVertex2f(-0.15*size,1.00*size);
    glVertex2f(0.55*size,0.80*size);
    glVertex2f(1.00*size,0.25*size);
    glVertex2f(0.85*size,-0.50*size);
    glVertex2f(0.30*size,-0.90*size);
    glVertex2f(-0.40*size,-0.85*size);
    glVertex2f(-0.90*size,-0.40*size);
    glEnd();

    glColor3f(0.25,0.25,0.25);
    drawCircle(-0.30*size,0.30*size,0.20*size);
    drawCircle(0.40*size,-0.20*size,0.15*size);

    glColor3f(0.33,0.33,0.33);
    drawCircle(0.30*size,0.45*size,0.10*size);
}

void asteroid(float x,float y,float size)
{
    glPushMatrix();
    glTranslatef(x,y,0);
    glRotatef(angle,0,0,1);
    drawAsteroidShape(size);
    glPopMatrix();
}

void drawShieldPowerUp(float x,float y)
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            x+0.10*cos(i*2*PI/amount),
            y+0.10*sin(i*2*PI/amount)
        );
    }

    glEnd();
}

void drawActiveShield()
{
    glColor3f(0,0.8,1);
    glLineWidth(3);

    glBegin(GL_LINE_LOOP);

    int amount=100;

    for(int i=0;i<=amount;i++)
    {
        glVertex2f(
            playerX+0.20*cos(i*2*PI/amount),
            getPlayerY()+0.20*sin(i*2*PI/amount)
        );
    }

    glEnd();
}

void drawBulletPowerUp(float x,float y)
{
    glPushMatrix();
    glTranslatef(x,y,0);

    glColor3f(1,1,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(0,0.06);
    glVertex2f(-0.025,0.02);
    glVertex2f(0.025,0.02);
    glEnd();

    glColor3f(1,0.7,0);

    glBegin(GL_QUADS);
    glVertex2f(-0.025,0.02);
    glVertex2f(0.025,0.02);
    glVertex2f(0.025,-0.06);
    glVertex2f(-0.025,-0.06);
    glEnd();

    glColor3f(1,0,0);

    glBegin(GL_TRIANGLES);
    glVertex2f(-0.025,-0.06);
    glVertex2f(0.025,-0.06);
    glVertex2f(0,-0.09);
    glEnd();

    glPopMatrix();
}

//====================================================
// LEVEL 5 BULLET DRAWING AND CREATION
//====================================================

void drawPlayerBullets()
{
    if(bulletPowerActive==1)
    {
        glColor3f(1,1,0);
        glLineWidth(5);
    }
    else
    {
        glColor3f(0,0.7,1);
        glLineWidth(4);
    }

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bulletX[i],bulletY[i]);
            glVertex2f(bulletX[i],bulletY[i]+0.12);
            glEnd();
        }
    }
}

void createPlayerBullet(float x,float y)
{
    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            bulletActive[i]=1;
            bulletX[i]=x;
            bulletY[i]=y;
            break;
        }
    }
}

void fireBullet()
{
    int freeBullet=0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    float startY=-0.38;

    if(bulletPowerActive==1)
    {
        createPlayerBullet(playerX-0.07,startY);
        createPlayerBullet(playerX,startY);
        createPlayerBullet(playerX+0.07,startY);
    }
    else
    {
        createPlayerBullet(playerX,startY);
    }

    ::playShootSound();
}

void drawEnemyBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]);
            glVertex2f(enemyBulletX[i],enemyBulletY[i]-0.12);
            glEnd();
        }
    }
}

void fireEnemyBullet(int enemyNumber)
{
    if(
        enemyAlive[enemyNumber]==1 &&
        enemyBulletActive[enemyNumber]==0
      )
    {
        enemyBulletActive[enemyNumber]=1;
        enemyBulletX[enemyNumber]=enemyX[enemyNumber];
        enemyBulletY[enemyNumber]=enemyY[enemyNumber]-0.12;
        ::playEnemyShootSound();
    }
}

void drawBossBullets()
{
    glColor3f(1,0,0);
    glLineWidth(4);

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            glBegin(GL_LINES);
            glVertex2f(bossBulletX[i],bossBulletY[i]);
            glVertex2f(bossBulletX[i],bossBulletY[i]-0.14);
            glEnd();
        }
    }
}

void createBossBullet(float x,float y,float dx)
{
    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==0)
        {
            bossBulletActive[i]=1;
            bossBulletX[i]=x;
            bossBulletY[i]=y;
            bossBulletDX[i]=dx;
            break;
        }
    }
}

void fireBossBullets()
{
    if(bossAlive==0)
    {
        return;
    }

    int freeBullet=0;

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==0)
        {
            freeBullet=1;
            break;
        }
    }

    if(freeBullet==0)
    {
        return;
    }

    // PHASE 1: HP 500 TO 351 = ONE BULLET

    if(bossHealth>350)
    {
        createBossBullet(
            bossX,
            bossY-0.25,
            0.0
        );
    }

    // PHASE 2: HP 350 TO 201 = THREE BULLETS

    else if(bossHealth>200)
    {
        createBossBullet(
            bossX-0.12,
            bossY-0.24,
            -0.002
        );

        createBossBullet(
            bossX,
            bossY-0.25,
            0.0
        );

        createBossBullet(
            bossX+0.12,
            bossY-0.24,
            0.002
        );
    }

    // PHASE 3: HP 200 OR BELOW = FAST THREE BULLETS

    else
    {
        createBossBullet(
            bossX-0.14,
            bossY-0.24,
            -0.0025
        );

        createBossBullet(
            bossX,
            bossY-0.25,
            0.0
        );

        createBossBullet(
            bossX+0.14,
            bossY-0.24,
            0.0025
        );
    }

    // Play once for the whole volley.
    ::playEnemyShootSound();
}

//====================================================
// LEVEL 5 INITIAL SETUP
//====================================================

void startLevel5()
{
    ::initSoundEffects();
    ::stopSoundEffects();

    for(int i=0;i<MAX_BLASTS;i++)
    {
        blastTimer[i]=0;
    }

    currentLevel=5;
    levelComplete=0;
    gameOver=0;
    gameWin=0;

    playerX=0.0;
    playerLife=3;
    playerDamageDelay=0;
    leftKeyPressed=0;
    rightKeyPressed=0;
    shootDelay=0;

    bossX=0.0;
    bossY=0.42;
    bossSpeed=0.0035;
    bossDirection=1;
    bossHealth=500;
    bossMaxHealth=500;
    bossAlive=1;
    bossShootDelay=90;
    bossBulletSpeed=0.008;

    enemyX[0]=-0.48;
    enemyY[0]=0.62;
    enemySpeed[0]=0.002;
    enemyDirection[0]=1;
    enemyAlive[0]=1;
    enemyHealth[0]=100;

    enemyX[1]=0.48;
    enemyY[1]=0.62;
    enemySpeed[1]=0.002;
    enemyDirection[1]=-1;
    enemyAlive[1]=1;
    enemyHealth[1]=100;

    enemyShootDelay[0]=100;
    enemyShootDelay[1]=150;
    level5SideEnemyTurn=0;

    heartPowerActive=0;
    shieldAvailable=0;
    shieldActive=0;
    shieldTimer=0;
    bulletPowerAvailable=0;
    bulletPowerActive=0;
    bulletPowerTimer=0;
    asteroidDestroyCount=0;

    asteroidX[0]=-0.75;
    asteroidY[0]=0.95;
    asteroidSize[0]=0.08;
    asteroidSpeed[0]=0.0032;

    asteroidX[1]=-0.25;
    asteroidY[1]=0.55;
    asteroidSize[1]=0.08;
    asteroidSpeed[1]=0.0035;

    asteroidX[2]=0.25;
    asteroidY[2]=0.15;
    asteroidSize[2]=0.07;
    asteroidSpeed[2]=0.0033;

    asteroidX[3]=0.75;
    asteroidY[3]=0.75;
    asteroidSize[3]=0.09;
    asteroidSpeed[3]=0.0038;

    angle=0.0;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bulletActive[i]=0;
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        enemyBulletActive[i]=0;
    }

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        bossBulletActive[i]=0;
    }
}

//====================================================
// LEVEL 5 MOVEMENT FUNCTIONS
//====================================================

void movePlayer()
{
    if(
        gameOver==1 ||
        levelComplete==1 ||
        gameWin==1
      )
    {
        return;
    }

    if(leftKeyPressed==1)
    {
        playerX=playerX-getPlayerSpeed();

        if(playerX<-0.75)
        {
            playerX=-0.75;
        }
    }

    if(rightKeyPressed==1)
    {
        playerX=playerX+getPlayerSpeed();

        if(playerX>0.75)
        {
            playerX=0.75;
        }
    }
}

void moveEnemies()
{
    for(int i=0;i<MAX_ENEMIES;i++)
    {
        if(enemyAlive[i]==1)
        {
            if(bossHealth<=250)
            {
                enemySpeed[i]=0.003;
            }
            else
            {
                enemySpeed[i]=0.002;
            }

            enemyX[i]=
                enemyX[i]+enemySpeed[i]*enemyDirection[i];

            if(i==0)
            {
                if(enemyX[i]<-0.65)
                {
                    enemyX[i]=-0.65;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>-0.35)
                {
                    enemyX[i]=-0.35;
                    enemyDirection[i]=-1;
                }
            }

            if(i==1)
            {
                if(enemyX[i]<0.35)
                {
                    enemyX[i]=0.35;
                    enemyDirection[i]=1;
                }

                if(enemyX[i]>0.65)
                {
                    enemyX[i]=0.65;
                    enemyDirection[i]=-1;
                }
            }
        }
    }
}

void moveBoss()
{
    if(bossAlive==0)
    {
        return;
    }

    if(bossHealth<=250)
    {
        bossSpeed=0.0048;
    }
    else
    {
        bossSpeed=0.0035;
    }

    bossX=bossX+bossSpeed*bossDirection;

    if(bossX>0.35)
    {
        bossX=0.35;
        bossDirection=-1;
    }

    if(bossX<-0.35)
    {
        bossX=-0.35;
        bossDirection=1;
    }
}

void moveBossBullets()
{
    if(bossHealth<=200)
    {
        bossBulletSpeed=0.011;
    }
    else if(bossHealth<=350)
    {
        bossBulletSpeed=0.010;
    }
    else
    {
        bossBulletSpeed=0.008;
    }

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            bossBulletX[i]=bossBulletX[i]+bossBulletDX[i];
            bossBulletY[i]=bossBulletY[i]-bossBulletSpeed;

            if(
                bossBulletY[i]<-1.0 ||
                bossBulletX[i]<-1.0 ||
                bossBulletX[i]>1.0
              )
            {
                bossBulletActive[i]=0;
            }
        }
    }
}

void moveAsteroids()
{
    for(int i=0;i<MAX_ASTEROIDS;i++)
    {
        float currentAsteroidSpeed=asteroidSpeed[i];

        if(bossHealth<=250)
        {
            currentAsteroidSpeed=currentAsteroidSpeed+0.0012;
        }

        asteroidY[i]=asteroidY[i]-currentAsteroidSpeed;

        if(asteroidY[i]<-1.10)
        {
            asteroidY[i]=1.05;
        }
    }
}

//====================================================
// LEVEL 5 WIN CONDITION
//====================================================

void checkLevel5Complete()
{
    if(
        bossAlive==0 &&
        bossHealth<=0 &&
        enemyAlive[0]==0 &&
        enemyAlive[1]==0 &&
        enemyHealth[0]<=0 &&
        enemyHealth[1]<=0 &&
        levelComplete==0
      )
    {
        levelComplete=1;
        gameWin=1;

        enemyBulletActive[0]=0;
        enemyBulletActive[1]=0;

        for(int i=0;i<MAX_BOSS_BULLETS;i++)
        {
            bossBulletActive[i]=0;
        }
    }
}

//====================================================
// LEVEL 5 COLLISION FUNCTIONS
//====================================================

void checkPlayerBulletCollision()
{
    for(int e=0;e<MAX_ENEMIES;e++)
    {
        if(enemyAlive[e]==1)
        {
            for(int i=0;i<MAX_BULLETS;i++)
            {
                if(bulletActive[i]==1)
                {
                    if(
                        bulletX[i]>enemyX[e]-0.08 &&
                        bulletX[i]<enemyX[e]+0.08 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.08
                      )
                    {
                        enemyHealth[e]=enemyHealth[e]-20;
                        score=score+20;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]-0.18 &&
                        bulletX[i]<enemyX[e]-0.07 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }
                    else if(
                        bulletX[i]>enemyX[e]+0.07 &&
                        bulletX[i]<enemyX[e]+0.18 &&
                        bulletY[i]>enemyY[e]-0.12 &&
                        bulletY[i]<enemyY[e]+0.12
                           )
                    {
                        enemyHealth[e]=enemyHealth[e]-10;
                        score=score+10;
                        bulletActive[i]=0;
                    }

                    if(enemyHealth[e]<=0)
                    {
                        enemyHealth[e]=0;
                        enemyAlive[e]=0;
                        enemyBulletActive[e]=0;
                        startBlast(enemyX[e],enemyY[e],0.22);
                        checkLevel5Complete();
                        break;
                    }
                }
            }
        }
    }
}

void checkBossBulletCollision()
{
    if(bossAlive==0)
    {
        return;
    }

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            if(
                bulletX[i]>bossX-0.45 &&
                bulletX[i]<bossX+0.45 &&
                bulletY[i]>bossY-0.28 &&
                bulletY[i]<bossY+0.22
              )
            {
                int damage;

                if(bulletPowerActive==1)
                {
                    damage=30;
                }
                else
                {
                    damage=10;
                }

                bossHealth=bossHealth-damage;
                score=score+damage;
                bulletActive[i]=0;

                if(bossHealth<=0)
                {
                    bossHealth=0;
                    bossAlive=0;
                    startBlast(bossX,bossY,0.38);

                    for(int b=0;b<MAX_BOSS_BULLETS;b++)
                    {
                        bossBulletActive[b]=0;
                    }

                    checkLevel5Complete();
                    break;
                }
            }
        }
    }
}

void checkAsteroidBulletCollision()
{
    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bulletActive[i]==1)
            {
                float dx=bulletX[i]-asteroidX[a];
                float dy=bulletY[i]-asteroidY[a];
                float distance=sqrt(dx*dx+dy*dy);

                if(distance<asteroidSize[a]+0.03)
                {
                    bulletActive[i]=0;
                    score=score+30;
                    ::showAsteroidFact(5,a);

                    if(
                        heartPowerActive==0 &&
                        shieldAvailable==0 &&
                        bulletPowerAvailable==0
                      )
                    {
                        asteroidDestroyCount++;

                        if(asteroidDestroyCount%3==1)
                        {
                            if(playerLife<3)
                            {
                                heartPowerActive=1;
                                heartPowerX=asteroidX[a];
                                heartPowerY=1.05;
                            }
                            else
                            {
                                bulletPowerAvailable=1;
                                bulletPowerX=asteroidX[a];
                                bulletPowerY=1.05;
                            }
                        }
                        else if(asteroidDestroyCount%3==2)
                        {
                            shieldAvailable=1;
                            shieldActive=0;
                            shieldTimer=0;
                            shieldPowerX=asteroidX[a];
                            shieldPowerY=1.05;
                        }
                        else
                        {
                            bulletPowerAvailable=1;
                            bulletPowerX=asteroidX[a];
                            bulletPowerY=1.05;
                        }
                    }

                    startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
                    asteroidY[a]=1.05;
                    break;
                }
            }
        }
    }
}

void damagePlayer()
{
    if(gameOver==1)
    {
        return;
    }

    playerLife=playerLife-1;
    playerDamageDelay=30;

    if(playerLife<=0)
    {
        playerLife=0;
        gameOver=1;
        startBlast(playerX,getPlayerY(),0.22);
        leftKeyPressed=0;
        rightKeyPressed=0;
    }
}

void checkPlayerDamage()
{
    float playerY=getPlayerY();

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(
            enemyBulletActive[i]==1 &&
            playerDamageDelay==0
          )
        {
            if(
                enemyBulletX[i]>playerX-0.18 &&
                enemyBulletX[i]<playerX+0.18 &&
                enemyBulletY[i]>playerY-0.23 &&
                enemyBulletY[i]<playerY+0.17
              )
            {
                enemyBulletActive[i]=0;

                if(shieldActive==1)
                {
                    playerDamageDelay=10;
                }
                else
                {
                    damagePlayer();
                }
            }
        }
    }
}

void checkBossPlayerDamage()
{
    if(playerDamageDelay>0)
    {
        return;
    }

    float playerY=getPlayerY();

    for(int i=0;i<MAX_BOSS_BULLETS;i++)
    {
        if(bossBulletActive[i]==1)
        {
            if(
                bossBulletX[i]>playerX-0.18 &&
                bossBulletX[i]<playerX+0.18 &&
                bossBulletY[i]>playerY-0.23 &&
                bossBulletY[i]<playerY+0.17
              )
            {
                bossBulletActive[i]=0;

                if(shieldActive==1)
                {
                    playerDamageDelay=10;
                }
                else
                {
                    damagePlayer();
                }

                break;
            }
        }
    }
}

void checkAsteroidPlayerCollision()
{
    if(playerDamageDelay>0)
    {
        return;
    }

    float playerY=getPlayerY();

    for(int a=0;a<MAX_ASTEROIDS;a++)
    {
        float dx=asteroidX[a]-playerX;
        float dy=asteroidY[a]-playerY;
        float distance=sqrt(dx*dx+dy*dy);

        if(distance<asteroidSize[a]+0.16)
        {
            startBlast(asteroidX[a],asteroidY[a],asteroidSize[a]*1.5f);
            asteroidY[a]=1.05;

            if(shieldActive==1)
            {
                playerDamageDelay=10;
            }
            else
            {
                damagePlayer();
            }

            break;
        }
    }
}

void checkHeartPower()
{
    if(heartPowerActive==1)
    {
        if(
            heartPowerX>playerX-0.18 &&
            heartPowerX<playerX+0.18 &&
            heartPowerY>getPlayerY()-0.12 &&
            heartPowerY<getPlayerY()+0.18
          )
        {
            if(playerLife<3)
            {
                playerLife=playerLife+1;
                heartPowerActive=0;
            }
        }
    }
}

void checkShieldPower()
{
    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        if(
            shieldPowerX>playerX-0.18 &&
            shieldPowerX<playerX+0.18 &&
            shieldPowerY>getPlayerY()-0.12 &&
            shieldPowerY<getPlayerY()+0.18
          )
        {
            shieldAvailable=0;
            shieldActive=1;
            shieldTimer=LEVEL5_SHIELD_ACTIVE_TIME;
        }
    }
}

void checkBulletPower()
{
    if(
        bulletPowerAvailable==1 &&
        bulletPowerActive==0
      )
    {
        if(
            bulletPowerX>playerX-0.18 &&
            bulletPowerX<playerX+0.18 &&
            bulletPowerY>getPlayerY()-0.12 &&
            bulletPowerY<getPlayerY()+0.18
          )
        {
            bulletPowerAvailable=0;
            bulletPowerActive=1;
            bulletPowerTimer=BULLET_POWER_ACTIVE_TIME;
        }
    }
}

//====================================================
// LEVEL 5 UPDATE LOOP
//====================================================

void update(int value)
{
    // Stop this level's timer after another display becomes active.
    if(::activeScene!=5 || value!=::levelTimerVersion)
    {
        return;
    }

    updateBlasts();

    if(
        gameOver==1 ||
        levelComplete==1
      )
    {
        glutPostRedisplay();
        glutTimerFunc(30,update,value);
        return;
    }

    movePlayer();

    for(int i=0;i<MAX_BULLETS;i++)
    {
        if(bulletActive[i]==1)
        {
            bulletY[i]=bulletY[i]+bulletSpeed;

            if(bulletY[i]>1.0)
            {
                bulletActive[i]=0;
            }
        }
    }

    for(int i=0;i<MAX_ENEMY_BULLETS;i++)
    {
        if(enemyBulletActive[i]==1)
        {
            enemyBulletY[i]=enemyBulletY[i]-enemyBulletSpeed;

            if(enemyBulletY[i]<-1.0)
            {
                enemyBulletActive[i]=0;
            }
        }
    }

    moveBossBullets();
    moveEnemies();
    moveBoss();
    moveAsteroids();

    angle=angle+2;

    if(angle>360)
    {
        angle=0;
    }

    if(bossAlive==1)
    {
        if(bossShootDelay>0)
        {
            bossShootDelay--;
        }
        else
        {
            fireBossBullets();

            if(bossHealth<=200)
            {
                bossShootDelay=55;
            }
            else if(bossHealth<=350)
            {
                bossShootDelay=65;
            }
            else
            {
                bossShootDelay=90;
            }
        }
    }

    if(heartPowerActive==1)
    {
        heartPowerY=heartPowerY-heartPowerSpeed;

        if(heartPowerY<-1.0)
        {
            heartPowerActive=0;
        }
    }

    if(
        shieldAvailable==1 &&
        shieldActive==0
      )
    {
        shieldPowerY=shieldPowerY-shieldPowerSpeed;

        if(shieldPowerY<-1.0)
        {
            shieldAvailable=0;
        }
    }

    if(shieldActive==1)
    {
        if(shieldTimer>0)
        {
            shieldTimer--;
        }
        else
        {
            shieldActive=0;
        }
    }

    if(
        bulletPowerAvailable==1 &&
        bulletPowerActive==0
      )
    {
        bulletPowerY=bulletPowerY-bulletPowerSpeed;

        if(bulletPowerY<-1.0)
        {
            bulletPowerAvailable=0;
        }
    }

    if(bulletPowerActive==1)
    {
        if(bulletPowerTimer>0)
        {
            bulletPowerTimer--;
        }
        else
        {
            bulletPowerActive=0;
        }
    }

    checkPlayerBulletCollision();
    checkBossBulletCollision();
    checkAsteroidBulletCollision();
    checkPlayerDamage();
    checkBossPlayerDamage();
    checkAsteroidPlayerCollision();
    checkHeartPower();
    checkShieldPower();
    checkBulletPower();

    if(playerDamageDelay>0)
    {
        playerDamageDelay--;
    }

    if(shootDelay>0)
    {
        shootDelay--;
    }

    int i=level5SideEnemyTurn;

    if(enemyShootDelay[i]>0)
    {
        enemyShootDelay[i]--;
    }
    else
    {
        fireEnemyBullet(i);

        if(i==0)
        {
            level5SideEnemyTurn=1;
            enemyShootDelay[1]=110;
        }
        else
        {
            level5SideEnemyTurn=0;
            enemyShootDelay[0]=110;
        }
    }

    checkLevel5Complete();

    ::updateHighScore(score);

    glutPostRedisplay();
    glutTimerFunc(30,update,value);
}

//====================================================
// LEVEL 5 KEYBOARD CONTROL
//====================================================

void keyboard(unsigned char key,int x,int y)
{
    if(key==27)
    {
        ::showMainMenu();
        return;
    }

    if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }

    if(gameOver==1)
    {
        if(key=='r' || key=='R')
        {
            startLevel5();
            score=::carriedScore;
            playerLife=::carriedLife;
            glutPostRedisplay();
        }
        else if(key=='n' || key=='N')
        {
            ::showLevel1();
        }

        return;
    }

    if(levelComplete==1)
    {
        return;
    }

    if(key==' ')
    {
        if(shootDelay==0)
        {
            fireBullet();
            shootDelay=5;
        }
    }
}

void specialKey(int key,int x,int y)
{
    if(
        gameOver==1 ||
        levelComplete==1 ||
        gameWin==1
      )
    {
        return;
    }

    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=1;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=1;
    }

    glutPostRedisplay();
}

void specialKeyUp(int key,int x,int y)
{
    if(key==GLUT_KEY_LEFT)
    {
        leftKeyPressed=0;
    }

    if(key==GLUT_KEY_RIGHT)
    {
        rightKeyPressed=0;
    }
}

//====================================================
// LEVEL 5 DISPLAY
//====================================================

void display()
{
    char scoreText[30];

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawStars();

    if(gameWin==0)
    {
        for(int i=0;i<MAX_ASTEROIDS;i++)
        {
            asteroid(
                asteroidX[i],
                asteroidY[i],
                asteroidSize[i]
            );
            ::drawAsteroidLabel(5,i,asteroidX[i],asteroidY[i]-asteroidSize[i]-0.06f);
        }

        drawBossHealth();

        if(bossAlive==1)
        {
            finalBossShip();
        }

        for(int i=0;i<MAX_ENEMIES;i++)
        {
            if(enemyAlive[i]==1)
            {
                enemyShip(enemyX[i],enemyY[i]);
            }
        }

        if(gameOver==0)
        {
            playerShip();
        }

        if(shieldActive==1)
        {
            drawActiveShield();
        }

        drawPlayerBullets();
        drawEnemyBullets();
        drawBossBullets();

        if(heartPowerActive==1)
        {
            drawHeart(heartPowerX,heartPowerY);
        }

        if(
            shieldAvailable==1 &&
            shieldActive==0
          )
        {
            drawShieldPowerUp(shieldPowerX,shieldPowerY);
        }

        if(
            bulletPowerAvailable==1 &&
            bulletPowerActive==0
          )
        {
            drawBulletPowerUp(bulletPowerX,bulletPowerY);
        }
    }

    drawBlasts();

    glColor3f(1,1,1);

    sprintf(scoreText,"SCORE: %03d",score);

    renderBitmapString(
        -0.9,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        scoreText
    );

    renderBitmapString(
        -0.05,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LEVEL: 5"
    );

    renderBitmapString(
        0.55,
        0.9,
        0,
        GLUT_BITMAP_HELVETICA_18,
        "LIFE:"
    );

    if(playerLife>=1)
    {
        drawHeart(0.72,0.91);
    }

    if(playerLife>=2)
    {
        drawHeart(0.82,0.91);
    }

    if(playerLife>=3)
    {
        drawHeart(0.92,0.91);
    }

    glColor3f(0.9,0.9,0.7);

    renderBitmapString(
        -0.9,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "Menu=Esc"
    );

    renderBitmapString(
        0.72,
        -0.92,
        0,
        GLUT_BITMAP_HELVETICA_12,
        "C=CLOSE"
    );

    if(bulletPowerActive==1)
    {
        glColor3f(1,1,0);

        renderBitmapString(
            -0.54,
            0.86,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "BULLET POWER ACTIVE"
        );
    }

    if(shieldActive==1)
    {
        glColor3f(0,0.8,1);

        renderBitmapString(
            0.30,
            0.86,
            0,
            GLUT_BITMAP_HELVETICA_12,
            "SHIELD ACTIVE"
        );
    }

    if(gameOver==1)
    {
        drawCenterMessage(
            "GAME OVER",
            "R=RETRY  N=NEW",
            1,
            0,
            0
        );
    }

    if(
        levelComplete==1 &&
        gameWin==1 &&
        bossAlive==0 &&
        enemyAlive[0]==0 &&
        enemyAlive[1]==0
      )
    {
        drawCenterMessage(
            "YOU WIN",
            "MISSION COMPLETE",
            0,
            1,
            0
        );
    }

    ::drawFactPopup();

    glFlush();
}

//====================================================
// SOUND
// Keep a.wav in the same folder as the .exe file.
//====================================================

void sound()
{
    Opening::sound();
}

//====================================================
// SAME gluOrtho2D COORDINATE SYSTEM AS FULL PROJECT
//====================================================

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(
        -1.0,
        1.0,
        -1.0,
        1.0
    );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//====================================================
// LEVEL 5 ONLY MAIN FUNCTION
//====================================================

// Standalone main() removed during merge.
// The only final main() is placed after all six sections.

} // namespace Level5
// LEVEL5 FULL CPP SECTION END
#undef MAX_BLASTS
#undef BLAST_TIME
#undef PI
#undef MAX_ENEMIES
#undef MAX_BULLETS
#undef MAX_ENEMY_BULLETS
#undef MAX_ASTEROIDS
#undef LEVEL5_SHIELD_ACTIVE_TIME
#undef MAX_BOSS_BULLETS
#undef BULLET_POWER_ACTIVE_TIME


//============================================================
// HIGH SCORE FILE FUNCTIONS
// highscore.txt is kept beside the game .exe file.
//============================================================

void loadHighScore()
{
    highScore=0;

    int pathLength=GetModuleFileNameA(NULL,highScoreFile,MAX_PATH);

    if(pathLength>0 && pathLength<MAX_PATH)
    {
        int i=pathLength-1;

        while(i>=0 && highScoreFile[i]!='\\' && highScoreFile[i]!='/')
        {
            i--;
        }

        if(i>=0)
        {
            sprintf(highScoreFile+i+1,"highscore.txt");
        }
        else
        {
            sprintf(highScoreFile,"highscore.txt");
        }
    }
    else
    {
        sprintf(highScoreFile,"highscore.txt");
    }

    FILE *file=fopen(highScoreFile,"r");

    if(file!=NULL)
    {
        int savedScore=0;

        if(fscanf(file,"%d",&savedScore)==1 && savedScore>=0)
        {
            highScore=savedScore;
        }

        fclose(file);
    }
}

void saveHighScore()
{
    highScoreSaveFailed=0;

    FILE *file=fopen(highScoreFile,"w");

    if(file==NULL)
    {
        highScoreSaveFailed=1;
        return;
    }

    if(fprintf(file,"%d\n",highScore)<0)
    {
        highScoreSaveFailed=1;
    }

    if(fclose(file)!=0)
    {
        highScoreSaveFailed=1;
    }
}

void updateHighScore(int currentScore)
{
    if(currentScore>highScore)
    {
        highScore=currentScore;
        saveHighScore();
    }
}


//============================================================
// SHARED SOUND EFFECTS FOR LEVELS 1-5
// SETTINGS uses Opening::soundEnabled for every sound.
// Keep shoot.wav, blast.wav and enemy_shoot.wav beside the .exe.
//============================================================

void stopSoundEffects()
{
    if(shootSoundReady==1)
    {
        mciSendStringA("stop gameShoot",NULL,0,NULL);
    }

    if(blastSoundReady==1)
    {
        mciSendStringA("stop gameBlast",NULL,0,NULL);
    }

    if(enemyShootSoundReady==1)
    {
        mciSendStringA("stop gameEnemyShoot",NULL,0,NULL);
    }
}

void closeSoundEffects()
{
    stopSoundEffects();

    if(shootSoundReady==1)
    {
        mciSendStringA("close gameShoot",NULL,0,NULL);
        shootSoundReady=0;
    }

    if(blastSoundReady==1)
    {
        mciSendStringA("close gameBlast",NULL,0,NULL);
        blastSoundReady=0;
    }

    if(enemyShootSoundReady==1)
    {
        mciSendStringA("close gameEnemyShoot",NULL,0,NULL);
        enemyShootSoundReady=0;
    }
}

void initSoundEffects()
{
    char folder[MAX_PATH];
    char command[MAX_PATH+100];
    int length=GetModuleFileNameA(NULL,folder,MAX_PATH);

    if(length>0 && length<MAX_PATH)
    {
        int i=length-1;

        while(i>=0 && folder[i]!='\\' && folder[i]!='/')
        {
            i--;
        }

        folder[i+1]='\0';
    }
    else
    {
        folder[0]='\0';
    }

    if(shootSoundReady==0)
    {
        sprintf(
            command,
            "open \"%sshoot.wav\" type waveaudio alias gameShoot",
            folder
        );

        if(mciSendStringA(command,NULL,0,NULL)==0)
        {
            shootSoundReady=1;
        }
    }

    if(blastSoundReady==0)
    {
        sprintf(
            command,
            "open \"%sblast.wav\" type waveaudio alias gameBlast",
            folder
        );

        if(mciSendStringA(command,NULL,0,NULL)==0)
        {
            blastSoundReady=1;
        }
    }

    if(enemyShootSoundReady==0)
    {
        sprintf(
            command,
            "open \"%senemy_shoot.wav\" type waveaudio alias gameEnemyShoot",
            folder
        );

        if(mciSendStringA(command,NULL,0,NULL)==0)
        {
            enemyShootSoundReady=1;
        }
    }

    if(soundCleanupRegistered==0)
    {
        atexit(closeSoundEffects);
        soundCleanupRegistered=1;
    }
}

void playShootSound()
{
    if(Opening::soundEnabled==1 && shootSoundReady==1)
    {
        mciSendStringA("stop gameShoot",NULL,0,NULL);
        mciSendStringA("play gameShoot from 0",NULL,0,NULL);
    }
}

void playBlastSound()
{
    if(Opening::soundEnabled==1 && blastSoundReady==1)
    {
        mciSendStringA("stop gameBlast",NULL,0,NULL);
        mciSendStringA("play gameBlast from 0",NULL,0,NULL);
    }
}


void playEnemyShootSound()
{
    if(Opening::soundEnabled==1 && enemyShootSoundReady==1)
    {
        mciSendStringA("stop gameEnemyShoot",NULL,0,NULL);
        mciSendStringA("play gameEnemyShoot from 0",NULL,0,NULL);
    }
}


//============================================================
// MULTIPLE DISPLAY VIEW FUNCTIONS
// glutDisplayFunc(display_name);
//============================================================

void opening_display_view()
{
    Opening::display();
}

void member1_level1_display_view()
{
    Level1::display();
}

void member2_level2_display_view()
{
    Level2::display();
}

void member3_level3_display_view()
{
    Level3::display();
}

void member4_level4_display_view()
{
    Level4::display();
}

void member5_level5_display_view()
{
    Level5::display();
}


//============================================================
// INPUT WRAPPERS
//============================================================

void emptySpecialKey(int key,int x,int y)
{
    // Opening pages do not use special keys.
}

void openingKeyboard(unsigned char key,int x,int y)
{
    if(key==27 && pausedScene!=0)
    {
        resumeGame();
        return;
    }

    Opening::keyboard(key,x,y);

    if(Opening::playButtonPressed==1)
    {
        Opening::playButtonPressed=0;
        showBriefing(1);
    }
}


//============================================================
// DISPLAY SWITCH FUNCTIONS
//============================================================

void showOpening()
{
    activeScene=0;
    transitionCounter=0;

    Opening::gameScreen=0;
    Opening::menuSelection=0;
    Opening::playButtonPressed=0;

    glutDisplayFunc(opening_display_view);
    glutKeyboardFunc(openingKeyboard);
    glutSpecialFunc(emptySpecialKey);
    glutSpecialUpFunc(emptySpecialKey);

    glutPostRedisplay();
}

void showMainMenu()
{
    stopSoundEffects();
    pausedScene=activeScene;
    pausedTransitionCounter=transitionCounter;

    // Ignore pending level timers while paused or after resuming.
    levelTimerVersion++;

    showOpening();
    Opening::gameScreen=1;

    glutPostRedisplay();
}

void resumeGame()
{
    if(pausedScene==0)
    {
        return;
    }

    activeScene=pausedScene;
    transitionCounter=pausedTransitionCounter;
    pausedScene=0;
    pausedTransitionCounter=0;

    // Restore the same level without resetting its game state.
    // Released arrow keys in the menu must not keep the player moving.

    if(activeScene==1)
    {
        Level1::leftKeyPressed=0;
        Level1::rightKeyPressed=0;

        glutDisplayFunc(member1_level1_display_view);
        glutKeyboardFunc(Level1::keyboard);
        glutSpecialFunc(Level1::specialKey);
        glutSpecialUpFunc(Level1::specialKeyUp);

        glutTimerFunc(30,Level1::update,levelTimerVersion);
    }
    else if(activeScene==2)
    {
        Level2::leftKeyPressed=0;
        Level2::rightKeyPressed=0;

        glutDisplayFunc(member2_level2_display_view);
        glutKeyboardFunc(Level2::keyboard);
        glutSpecialFunc(Level2::specialKey);
        glutSpecialUpFunc(Level2::specialKeyUp);

        glutTimerFunc(30,Level2::update,levelTimerVersion);
    }
    else if(activeScene==3)
    {
        Level3::leftKeyPressed=0;
        Level3::rightKeyPressed=0;

        glutDisplayFunc(member3_level3_display_view);
        glutKeyboardFunc(Level3::keyboard);
        glutSpecialFunc(Level3::specialKey);
        glutSpecialUpFunc(Level3::specialKeyUp);

        glutTimerFunc(30,Level3::update,levelTimerVersion);
    }
    else if(activeScene==4)
    {
        Level4::leftKeyPressed=0;
        Level4::rightKeyPressed=0;

        glutDisplayFunc(member4_level4_display_view);
        glutKeyboardFunc(Level4::keyboard);
        glutSpecialFunc(Level4::specialKey);
        glutSpecialUpFunc(Level4::specialKeyUp);

        glutTimerFunc(30,Level4::update,levelTimerVersion);
    }
    else if(activeScene==5)
    {
        Level5::leftKeyPressed=0;
        Level5::rightKeyPressed=0;

        glutDisplayFunc(member5_level5_display_view);
        glutKeyboardFunc(Level5::keyboard);
        glutSpecialFunc(Level5::specialKey);
        glutSpecialUpFunc(Level5::specialKeyUp);

        glutTimerFunc(30,Level5::update,levelTimerVersion);
    }

    glutPostRedisplay();
}

void showLevel1()
{
    stopSoundEffects();
    pausedScene=0;
    pausedTransitionCounter=0;
    activeScene=1;
    transitionCounter=0;
    carriedScore=0;
    carriedLife=3;
    resetDiscoveries();

    Level1::score=0;
    Level1::startLevel1();

    glutDisplayFunc(member1_level1_display_view);
    glutKeyboardFunc(Level1::keyboard);
    glutSpecialFunc(Level1::specialKey);
    glutSpecialUpFunc(Level1::specialKeyUp);

    glutTimerFunc(30,Level1::update,levelTimerVersion);
    glutPostRedisplay();
}

void showLevel2()
{
    activeScene=2;
    transitionCounter=0;

    Level2::startLevel2();
    Level2::score=carriedScore;
    Level2::playerLife=carriedLife;

    glutDisplayFunc(member2_level2_display_view);
    glutKeyboardFunc(Level2::keyboard);
    glutSpecialFunc(Level2::specialKey);
    glutSpecialUpFunc(Level2::specialKeyUp);

    glutTimerFunc(30,Level2::update,levelTimerVersion);
    glutPostRedisplay();
}

void showLevel3()
{
    activeScene=3;
    transitionCounter=0;

    Level3::startLevel3();
    Level3::score=carriedScore;
    Level3::playerLife=carriedLife;

    glutDisplayFunc(member3_level3_display_view);
    glutKeyboardFunc(Level3::keyboard);
    glutSpecialFunc(Level3::specialKey);
    glutSpecialUpFunc(Level3::specialKeyUp);

    glutTimerFunc(30,Level3::update,levelTimerVersion);
    glutPostRedisplay();
}

void showLevel4()
{
    activeScene=4;
    transitionCounter=0;

    Level4::startLevel4();
    Level4::score=carriedScore;
    Level4::playerLife=carriedLife;

    glutDisplayFunc(member4_level4_display_view);
    glutKeyboardFunc(Level4::keyboard);
    glutSpecialFunc(Level4::specialKey);
    glutSpecialUpFunc(Level4::specialKeyUp);

    glutTimerFunc(30,Level4::update,levelTimerVersion);
    glutPostRedisplay();
}

void showLevel5()
{
    activeScene=5;
    transitionCounter=0;

    Level5::startLevel5();
    Level5::score=carriedScore;
    Level5::playerLife=carriedLife;

    glutDisplayFunc(member5_level5_display_view);
    glutKeyboardFunc(Level5::keyboard);
    glutSpecialFunc(Level5::specialKey);
    glutSpecialUpFunc(Level5::specialKeyUp);

    glutTimerFunc(30,Level5::update,levelTimerVersion);
    glutPostRedisplay();
}


//============================================================
// NASA SPACE APPS EDITION
// Mission briefings, real NASA asteroid facts, final summary.
// Facts come from: NASA / JPL Small-Body Database, NASA CNEOS,
// NASA planetary defense and mission pages (nasa.gov).
//============================================================

//------------------------------------------------------------
// Small text and box helpers (global, independent of levels)
//------------------------------------------------------------

void nsaText(float x,float y,void *font,const char *s)
{
    glRasterPos2f(x,y);

    for(const char *c=s; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

float nsaTextHalfWidth(void *font,const char *s)
{
    int w=0;

    for(const char *c=s; *c!='\0'; c++)
    {
        w=w+glutBitmapWidth(font,*c);
    }

    int ww=glutGet(GLUT_WINDOW_WIDTH);

    if(ww<=0)
    {
        ww=1200;
    }

    return (float)w/(float)ww;
}

void nsaCenter(float y,void *font,const char *s)
{
    nsaText(-nsaTextHalfWidth(font,s),y,font,s);
}

void nsaBox(float x1,float y1,float x2,float y2,float r,float g,float b)
{
    glColor3f(r,g,b);

    glBegin(GL_QUADS);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}

void nsaBoxOutline(float x1,float y1,float x2,float y2,float r,float g,float b)
{
    glColor3f(r,g,b);
    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x1,y1);
    glVertex2f(x2,y1);
    glVertex2f(x2,y2);
    glVertex2f(x1,y2);
    glEnd();
}

void nsaStars()
{
    unsigned int seed=12345u;

    glColor3f(1,1,1);
    glPointSize(2);

    glBegin(GL_POINTS);

    for(int i=0;i<70;i++)
    {
        seed=seed*1103515245u+12345u;
        float x=(float)((seed>>16)&1023)/511.5f-1.0f;

        seed=seed*1103515245u+12345u;
        float y=(float)((seed>>16)&1023)/511.5f-1.0f;

        glVertex2f(x,y);
    }

    glEnd();
}

//------------------------------------------------------------
// MISSION BRIEFINGS (one per level)
//------------------------------------------------------------

struct MissionBriefing
{
    const char *title;
    const char *objective;
    const char *fact[3];
};

static const MissionBriefing missionBriefings[5]=
{
    {
        "EARTH ORBIT",
        "Objective: Defend Earth orbit from the hostile ship.",
        {
            "The International Space Station orbits about 400 km above Earth.",
            "It travels at about 28,000 km/h and circles Earth every ~90 minutes.",
            "NASA tracks tens of thousands of pieces of orbital debris to stay safe."
        }
    },
    {
        "THE MOON",
        "Objective: Clear the way for lunar exploration.",
        {
            "NASA's Artemis program is returning astronauts to the Moon.",
            "The Moon is about 384,400 km from Earth on average.",
            "Moon gravity is only about one sixth of Earth's gravity."
        }
    },
    {
        "NEAR-EARTH ASTEROIDS",
        "Objective: Survive the asteroid field and destroy both ships.",
        {
            "Near-Earth objects have orbits that bring them close to Earth's path.",
            "NASA's Planetary Defense Coordination Office watches the sky for them.",
            "TIP: Shoot an asteroid to identify it and reveal real NASA data."
        }
    },
    {
        "THE ASTEROID BELT",
        "Objective: Defeat the Guardian boss while crossing the belt.",
        {
            "The main belt lies between Mars and Jupiter.",
            "All the asteroids in the belt together weigh less than our Moon.",
            "TIP: Shoot asteroids to identify them. NASA's Dawn visited; Psyche is on its way."
        }
    },
    {
        "MARS AND BEYOND",
        "Objective: Defeat the Final Boss and complete the mission.",
        {
            "Mars has two tiny moons, Phobos and Deimos.",
            "In 2022 NASA's DART mission changed the orbit of an asteroid moon.",
            "TIP: Identify all four space rocks to finish your mission log."
        }
    }
};

//------------------------------------------------------------
// REAL SPACE OBJECTS (levels 3, 4, 5 - four per level)
//------------------------------------------------------------

struct SpaceObject
{
    const char *fullName;
    const char *shortName;
    const char *line1;
    const char *line2;
};

static const SpaceObject spaceObjects[3][4]=
{
    {
        {
            "101955 BENNU","Bennu",
            "About 490 m wide. NASA's OSIRIS-REx brought a sample to Earth in 2023.",
            "NASA estimates about a 1-in-2,700 chance of Earth impact in 2182."
        },
        {
            "99942 APOPHIS","Apophis",
            "About 340 m wide. It will pass Earth on April 13, 2029.",
            "It will pass about 32,000 km above Earth - closer than many satellites."
        },
        {
            "433 EROS","Eros",
            "About 34 km long. NASA's NEAR Shoemaker landed on it in 2001.",
            "Eros was the first near-Earth asteroid ever discovered (1898)."
        },
        {
            "25143 ITOKAWA","Itokawa",
            "About 535 m long and shaped like a sea otter.",
            "Japan's Hayabusa mission returned dust from it to Earth in 2010."
        }
    },
    {
        {
            "1 CERES","Ceres",
            "About 940 km wide - the largest object in the asteroid belt.",
            "NASA's Dawn orbited Ceres in 2015 and found bright salt deposits."
        },
        {
            "4 VESTA","Vesta",
            "About 525 km wide. NASA's Dawn orbited Vesta from 2011 to 2012.",
            "It has a giant impact basin at its south pole."
        },
        {
            "16 PSYCHE","Psyche",
            "About 220 km wide and rich in metal - maybe a planet's exposed core.",
            "NASA's Psyche spacecraft launched in October 2023 and arrives in 2029."
        },
        {
            "243 IDA","Ida",
            "About 56 km long. NASA's Galileo flew past it in 1993.",
            "Its tiny moon Dactyl was the first asteroid moon ever found."
        }
    },
    {
        {
            "PHOBOS (MOON OF MARS)","Phobos",
            "Only about 22 km across, and it orbits just ~6,000 km above Mars.",
            "It circles Mars in about 7.7 hours - faster than Mars spins."
        },
        {
            "DEIMOS (MOON OF MARS)","Deimos",
            "The smaller moon of Mars, only about 12 km wide.",
            "It orbits Mars about once every 30 hours."
        },
        {
            "DIMORPHOS","Dimorphos",
            "A small moonlet about 160 m wide, orbiting the asteroid Didymos.",
            "In 2022 NASA's DART hit it and shortened its orbit by about 32 minutes."
        },
        {
            "ARROKOTH","Arrokoth",
            "An icy object about 35 km long in the Kuiper Belt, far beyond Neptune.",
            "NASA's New Horizons flew past it on January 1, 2019."
        }
    }
};

static const char *objectGroupName[3]=
{
    "NEAR-EARTH ASTEROIDS",
    "ASTEROID BELT",
    "MARS AND BEYOND"
};

int objectFound[3][4];
int activeFactLevel=0;
int activeFactIndex=0;
int factUntil=0;
int briefingLevel=1;

void resetDiscoveries()
{
    for(int l=0;l<3;l++)
    {
        for(int i=0;i<4;i++)
        {
            objectFound[l][i]=0;
        }
    }

    activeFactLevel=0;
    factUntil=0;
}

// Called by Levels 3-5 when the player shoots an asteroid.
void showAsteroidFact(int level,int index)
{
    if(level<3 || level>5 || index<0 || index>3)
    {
        return;
    }

    objectFound[level-3][index]=1;
    activeFactLevel=level;
    activeFactIndex=index;
    factUntil=glutGet(GLUT_ELAPSED_TIME)+4500;
}

// Small name tag drawn under each asteroid.
void drawAsteroidLabel(int level,int index,float x,float y)
{
    if(level<3 || level>5 || index<0 || index>3)
    {
        return;
    }

    const char *name=spaceObjects[level-3][index].shortName;

    glColor3f(0.75f,0.9f,1.0f);

    nsaText(
        x-nsaTextHalfWidth(GLUT_BITMAP_HELVETICA_12,name),
        y,
        GLUT_BITMAP_HELVETICA_12,
        name
    );
}

// Fact popup shown for a few seconds after an asteroid is destroyed.
void drawFactPopup()
{
    if(activeFactLevel==0)
    {
        return;
    }

    if(glutGet(GLUT_ELAPSED_TIME)>=factUntil)
    {
        activeFactLevel=0;
        return;
    }

    const SpaceObject &obj=spaceObjects[activeFactLevel-3][activeFactIndex];
    char title[96];

    sprintf(title,"TARGET IDENTIFIED: %s",obj.fullName);

    nsaBox(-0.86f,-0.02f,0.86f,-0.36f,0.02f,0.05f,0.12f);
    nsaBoxOutline(-0.86f,-0.02f,0.86f,-0.36f,0.2f,0.8f,1.0f);

    glColor3f(1.0f,0.9f,0.3f);
    nsaCenter(-0.10f,GLUT_BITMAP_HELVETICA_18,title);

    glColor3f(1,1,1);
    nsaCenter(-0.19f,GLUT_BITMAP_HELVETICA_18,obj.line1);
    nsaCenter(-0.27f,GLUT_BITMAP_HELVETICA_18,obj.line2);

    glColor3f(0.6f,0.7f,0.8f);
    nsaCenter(-0.33f,GLUT_BITMAP_HELVETICA_12,"Source: NASA / JPL");
}

//------------------------------------------------------------
// BRIEFING SCREEN (shown before every level)
//------------------------------------------------------------

void briefingDisplay()
{
    char line[80];
    const MissionBriefing &b=missionBriefings[briefingLevel-1];

    glClearColor(0.0f,0.0f,0.04f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    nsaStars();

    sprintf(line,"MISSION %d OF 5",briefingLevel);
    glColor3f(1.0f,0.9f,0.3f);
    nsaCenter(0.86f,GLUT_BITMAP_HELVETICA_18,line);

    glColor3f(1,1,1);
    nsaCenter(0.72f,GLUT_BITMAP_TIMES_ROMAN_24,b.title);

    glColor3f(0.4f,0.9f,1.0f);
    nsaCenter(0.58f,GLUT_BITMAP_HELVETICA_18,b.objective);

    nsaBox(-0.92f,0.46f,0.92f,-0.30f,0.04f,0.06f,0.14f);
    nsaBoxOutline(-0.92f,0.46f,0.92f,-0.30f,0.2f,0.8f,1.0f);

    glColor3f(1.0f,0.9f,0.3f);
    nsaCenter(0.36f,GLUT_BITMAP_HELVETICA_18,"DID YOU KNOW?  (NASA FACTS)");

    glColor3f(1,1,1);
    nsaText(-0.86f,0.20f,GLUT_BITMAP_HELVETICA_18,b.fact[0]);
    nsaText(-0.86f,0.04f,GLUT_BITMAP_HELVETICA_18,b.fact[1]);
    nsaText(-0.86f,-0.12f,GLUT_BITMAP_HELVETICA_18,b.fact[2]);

    glColor3f(0.8f,0.8f,0.8f);
    nsaCenter(-0.48f,GLUT_BITMAP_HELVETICA_12,"Move: LEFT / RIGHT arrows        Shoot: SPACE        Menu: ESC");

    glColor3f(0.4f,1.0f,0.5f);
    nsaCenter(-0.68f,GLUT_BITMAP_HELVETICA_18,"Press ENTER to launch");

    glColor3f(0.9f,0.9f,0.7f);
    nsaCenter(-0.88f,GLUT_BITMAP_HELVETICA_12,"Press C to Close Game");

    glFlush();
}

void briefingKeyboard(unsigned char key,int x,int y)
{
    if(key==13)
    {
        if(briefingLevel==1)
        {
            showLevel1();
        }
        else if(briefingLevel==2)
        {
            showLevel2();
        }
        else if(briefingLevel==3)
        {
            showLevel3();
        }
        else if(briefingLevel==4)
        {
            showLevel4();
        }
        else
        {
            showLevel5();
        }
    }
    else if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }
}

void showBriefing(int level)
{
    if(level<1)
    {
        level=1;
    }

    if(level>5)
    {
        level=5;
    }

    briefingLevel=level;

    if(level==1)
    {
        pausedScene=0;
        pausedTransitionCounter=0;
    }

    stopSoundEffects();

    // Stop any pending level timer.
    levelTimerVersion++;
    activeScene=6;
    transitionCounter=0;

    glutDisplayFunc(briefingDisplay);
    glutKeyboardFunc(briefingKeyboard);
    glutSpecialFunc(emptySpecialKey);
    glutSpecialUpFunc(emptySpecialKey);

    glutPostRedisplay();
}

//------------------------------------------------------------
// FINAL SCREEN: "WHAT YOU LEARNED"
//------------------------------------------------------------

void finalDisplay()
{
    char line[96];
    int found=0;

    for(int l=0;l<3;l++)
    {
        for(int i=0;i<4;i++)
        {
            found=found+objectFound[l][i];
        }
    }

    glClearColor(0.0f,0.0f,0.04f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    nsaStars();

    glColor3f(0.4f,1.0f,0.5f);
    nsaCenter(0.86f,GLUT_BITMAP_TIMES_ROMAN_24,"MISSION COMPLETE");

    sprintf(line,"Final Score: %03d      High Score: %03d",Level5::score,highScore);
    glColor3f(1,1,1);
    nsaCenter(0.74f,GLUT_BITMAP_HELVETICA_18,line);

    sprintf(line,"Space objects identified: %d / 12",found);
    glColor3f(1.0f,0.9f,0.3f);
    nsaCenter(0.62f,GLUT_BITMAP_HELVETICA_18,line);

    for(int l=0;l<3;l++)
    {
        float x=-0.90f+0.62f*(float)l;

        glColor3f(0.4f,0.9f,1.0f);
        nsaText(x,0.48f,GLUT_BITMAP_HELVETICA_18,objectGroupName[l]);

        for(int i=0;i<4;i++)
        {
            if(objectFound[l][i]==1)
            {
                glColor3f(0.4f,1.0f,0.5f);
                sprintf(line,"[X] %s",spaceObjects[l][i].shortName);
            }
            else
            {
                glColor3f(0.5f,0.5f,0.5f);
                sprintf(line,"[ ] %s",spaceObjects[l][i].shortName);
            }

            nsaText(x,0.36f-0.10f*(float)i,GLUT_BITMAP_HELVETICA_18,line);
        }
    }

    nsaBox(-0.92f,-0.04f,0.92f,-0.56f,0.04f,0.06f,0.14f);
    nsaBoxOutline(-0.92f,-0.04f,0.92f,-0.56f,0.2f,0.8f,1.0f);

    glColor3f(1.0f,0.9f,0.3f);
    nsaCenter(-0.12f,GLUT_BITMAP_HELVETICA_18,"WHAT YOU LEARNED");

    glColor3f(1,1,1);
    nsaText(-0.86f,-0.24f,GLUT_BITMAP_HELVETICA_18,"Asteroids are leftover building blocks from the birth of our solar system.");
    nsaText(-0.86f,-0.35f,GLUT_BITMAP_HELVETICA_18,"NASA's DART mission showed we can change an asteroid's path to protect Earth.");
    nsaText(-0.86f,-0.46f,GLUT_BITMAP_HELVETICA_18,"Missions like OSIRIS-REx, Dawn and Psyche explore these worlds up close.");

    glColor3f(0.6f,0.7f,0.8f);
    nsaCenter(-0.68f,GLUT_BITMAP_HELVETICA_12,"Data sources: NASA/JPL Small-Body Database, NASA CNEOS and NASA mission pages (nasa.gov)");

    glColor3f(0.9f,0.9f,0.7f);
    nsaCenter(-0.88f,GLUT_BITMAP_HELVETICA_12,"R = Play Again        ENTER = Main Menu        C = Close Game");

    glFlush();
}

void finalKeyboard(unsigned char key,int x,int y)
{
    if(key=='r' || key=='R')
    {
        showBriefing(1);
    }
    else if(key==13)
    {
        pausedScene=0;
        showOpening();
        Opening::gameScreen=1;
        glutPostRedisplay();
    }
    else if(key=='c' || key=='C')
    {
        PlaySound(NULL,NULL,0);
        exit(0);
    }
}

void showFinalScreen()
{
    stopSoundEffects();

    levelTimerVersion++;
    activeScene=7;
    transitionCounter=0;
    pausedScene=0;

    glutDisplayFunc(finalDisplay);
    glutKeyboardFunc(finalKeyboard);
    glutSpecialFunc(emptySpecialKey);
    glutSpecialUpFunc(emptySpecialKey);

    glutPostRedisplay();
}


//============================================================
// AUTOMATIC LEVEL DISPLAY CONTROLLER
// Waits briefly so each LEVEL COMPLETE message can be seen.
//============================================================

void sceneController(int value)
{
    if(
        activeScene==1 &&
        Level1::levelComplete==1 &&
        Level1::gameOver==0
      )
    {
        transitionCounter++;

        if(transitionCounter>=15)
        {
            carriedScore=Level1::score;
            carriedLife=Level1::playerLife;
            showBriefing(2);
        }
    }
    else if(
        activeScene==2 &&
        Level2::levelComplete==1 &&
        Level2::gameOver==0
      )
    {
        transitionCounter++;

        if(transitionCounter>=15)
        {
            carriedScore=Level2::score;
            carriedLife=Level2::playerLife;
            showBriefing(3);
        }
    }
    else if(
        activeScene==3 &&
        Level3::levelComplete==1 &&
        Level3::gameOver==0
      )
    {
        transitionCounter++;

        if(transitionCounter>=15)
        {
            carriedScore=Level3::score;
            carriedLife=Level3::playerLife;
            showBriefing(4);
        }
    }
    else if(
        activeScene==4 &&
        Level4::levelComplete==1 &&
        Level4::gameOver==0
      )
    {
        transitionCounter++;

        if(transitionCounter>=15)
        {
            carriedScore=Level4::score;
            carriedLife=Level4::playerLife;
            showBriefing(5);
        }
    }
    else if(
        activeScene==5 &&
        Level5::levelComplete==1 &&
        Level5::gameWin==1 &&
        Level5::gameOver==0
      )
    {
        transitionCounter++;

        if(transitionCounter>=25)
        {
            showFinalScreen();
        }
    }
    else
    {
        transitionCounter=0;
    }

    glutTimerFunc(100,sceneController,0);
}


//============================================================
// ONLY ONE FINAL MAIN FUNCTION
//============================================================

int main(int argc,char** argv)
{
    loadHighScore();

    glutInit(&argc,argv);

    glutInitWindowSize(
        1200,
        600
    );

    glutInitWindowPosition(
        80,
        50
    );

    glutCreateWindow(
        "SPACE MISSION - NASA Space Apps Challenge"
    );

    // The same projection is used for Opening and Levels 1-5.
    Opening::init();

    // Background sound starts only once.
    Opening::sound();

    showOpening();

    glutTimerFunc(100,sceneController,0);

    glutMainLoop();

    return 0;
}
