#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>
#include "tga.h"

static int day = 0,month=0,year = 0;

struct Star
{
    float x, y;
    unsigned char r, g, b, a;
};
std::vector< Star > stars;

TGA* sun;
TGA* earth;
TGA* moon;

void init(void)
{
    glClearColor(0.0,0.0,0.0,0.0);
    glShadeModel(GL_FLAT);
    sun = new TGA("images/sun.tga");
    moon = new TGA("images/moon.tga");
    earth = new TGA("images/earth.tga");
    //glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);

    //Lighting set up
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //Set lighting intensity and color
    GLfloat qaAmbientLight[] = {0.2,0.2,0.2,1.0};
    GLfloat qaDiffuseLight[] = {0.8,0.8,0.8,1.0};
    GLfloat qaSpecularLight[] = {1.0,1.0,1.0,1.0};
    glLightfv(GL_LIGHT0,GL_AMBIENT,qaAmbientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,qaDiffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,qaSpecularLight);

    //Set the light position
    GLfloat qaLightPosition[] = {0.0,0.0,0.0,1.0};
    glLightfv(GL_LIGHT0,GL_POSITION,qaLightPosition);

}

void display(void)
{

    //Set material properties
    GLfloat qaWhite[] = {1.0,1.0,1.0,1.0};

    glMaterialfv(GL_FRONT,GL_AMBIENT,qaWhite);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,qaWhite);
    glMaterialfv(GL_FRONT,GL_SPECULAR,qaWhite);
    glMaterialf(GL_FRONT,GL_SHININESS,60.0);

   // glNormal3f(1.0,0.0,1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    //Draw Stars
    glColor3ub( 255, 255, 255 );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );
    glVertexPointer( 2, GL_FLOAT, sizeof(Star), &stars[0].x );
    glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof(Star), &stars[0].r );
    glPointSize( 3.0 );
    glDrawArrays( GL_POINTS, 0, stars.size() );
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );


    //Draw Sun
    glRotatef((GLfloat) year,0.0,1.0,0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, sun->getTextureHandle());
    GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, true);
	gluQuadricNormals(quadric, GLU_SMOOTH);
    gluSphere(quadric, 4.0, 30, 30);


    //Draw Earth
    glRotatef((GLfloat) year,0.0,1.0,0.0);
    glTranslatef(10.0,0.0,0.0);
    glRotatef((GLfloat) day,0.0,1.0,0.0);
    glBindTexture(GL_TEXTURE_2D, earth->getTextureHandle());
    gluSphere(quadric,0.7, 30, 30);//EARTH

    //Draw Moon
    glTranslatef(1.0,0.0,0.0);
    glRotatef((GLfloat)month,0.0,1.0,0.0);
    glBindTexture(GL_TEXTURE_2D, moon->getTextureHandle());
    gluSphere(quadric,0.2, 30, 30);//Moon

    glPopMatrix();
    glPushMatrix();
    glFlush();
    glutSwapBuffers();
}

void reshape(int w,int h)
{
    if(h==0)h=1;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0,0,w,h);
    gluPerspective(50.0,w/(GLfloat)h,3.0,90.0);
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(2.0,4.0,20.0,0.0,0.0,0.0,0.0,1.0,0.0);
}

void idleFunc()
{
    year = (year+1) % 360;
    day = (day+10) % 360;
    month = (month+3) % 360;
    glutPostRedisplay();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1200,800);
    glutInitWindowPosition(300,150);
    glutCreateWindow("Solar system");


    //Stars
    for( size_t i = 0; i < 1000; ++i )
    {
        Star star;
        star.x = -50 + (rand() % 100);
        star.y = -50 + (rand() % 100);
        star.r = 255;
        star.g = 255;
        star.b = 255;
        star.a = 255;
        stars.push_back(star);
    }

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idleFunc);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}
