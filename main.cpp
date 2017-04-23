#define OUTPUT_ANIMATION 0

#include <stdlib.h>
#include <stdio.h>

#include "Particles.h"

#if OUTPUT_ANIMATION
#include <opencv2/opencv.hpp>
#endif


inline float clip(const float& n, const float& lower, const float& upper) 
{
    return glm::max(lower, glm::min(n, upper));
}

float theta = M_PI/8;
float phi = -M_PI/8+M_PI_2;
float dist = 2.5;
int width = 800;
int height = 800;
int frame = 0;
const int render_step = 3;
int mx, my;

Particles particles;
Plane ground(glm::dvec3(0, 0, 0), glm::dvec3(0, 1, 0), 0);
Plane side0(glm::dvec3(1, 1, 0), glm::dvec3(1, 0, 0), 0);
Plane side1(glm::dvec3(0, 0, 1), glm::dvec3(0, 0, 1), 0);
Plane side2(glm::dvec3(-1, 0, 0), glm::dvec3(1, 0, 0), 0);
Plane side3(glm::dvec3(0, 0, -1), glm::dvec3(0, 0, 1), 0);
Plane side4(glm::dvec3(0, 2, 0), glm::dvec3(0, 1, 0), 0);
std::vector<Plane *> tank;


void display(void);

void reshape(int width, int height);

void idle(void)
{
    particles.step(0.0083, tank);
    glutPostRedisplay();
    if(frame/render_step >= 300)
        return;
    if(frame%render_step == 0)
    {
        #if OUTPUT_ANIMATION
        cv::Mat3b image(height, width);
        glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        cv::flip(image, image, 0);
        char fn[512];
        sprintf(fn, "result/%04d.png", frame/render_step);
        cv::imwrite(fn, image);
        #endif
    }
    frame++;
}

void mouse(int button, int state, int x, int y);

void motion(int x, int y);

void keyboard(unsigned char c, int x, int y)
{
    switch(c)
    {
    case 'o' :
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);


    tank.push_back(&ground);
    tank.push_back(&side0);
    tank.push_back(&side1);
    tank.push_back(&side2);
    tank.push_back(&side3);
    tank.push_back(&side4);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glClearDepth(1.0f);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    (void)glutCreateWindow("GLUT Program");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    glutKeyboardFunc(keyboard);


    return EXIT_SUCCESS;
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // your drawing code goes here
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.01, 100);
    gluLookAt(dist*sin(phi)*cos(theta), dist*cos(phi), dist*sin(phi)*sin(theta),
            0, 0, 0, 
            0, 1, 0);

    for (Plane *p : tank)
        p->render();
    particles.render();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        mx = x;
        my = y;
    }
}

void motion(int x, int y)
{
    int dx = x - mx;
    int dy = y - my;
    mx = x;
    my = y;
    if(abs(dx) > abs(dy))
        theta += dx*0.005;
    else
        phi -= dy*0.005;
    if(theta > 2*M_PI)
        theta -= 2*M_PI;
    if(theta < -2*M_PI)
        theta += 2*M_PI;
    phi = clip(phi, M_PI/12, M_PI*11/12);
    glutPostRedisplay();
}
