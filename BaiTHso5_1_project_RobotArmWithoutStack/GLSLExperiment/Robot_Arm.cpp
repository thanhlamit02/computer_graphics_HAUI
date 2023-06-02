
#include "Angel.h"

void drawWheel(float x, float y, float z, float radius) {
    int numSpokes = 10;
    float angleStep = 360.0 / numSpokes;
    float cosAngle, sinAngle;
    float angle = 0;

    // Draw the wheel rim
    glColor3f(0.2, 0.2, 0.2);
    glBegin(GL_POLYGON);
    for (int i = 0; i < numSpokes; i++) {
        cosAngle = cos(angle * 3.14159 / 180.0);
        sinAngle = sin(angle * 3.14159 / 180.0);
        float dx = radius * cosAngle;
        float dy = radius * sinAngle;
        glVertex3f(x + dx, y + dy, z);
        angle += angleStep;
    }
    glEnd();

    // Draw the wheel spokes
    glColor3f(0.7, 0.7, 0.7);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    angle = 0;
    for (int i = 0; i < numSpokes; i++) {
        cosAngle = cos(angle * 3.14159 / 180.0);
        sinAngle = sin(angle * 3.14159 / 180.0);
        float dx = radius * cosAngle;
        float dy = radius * sinAngle;
        glVertex3f(x, y, z);
        glVertex3f(x + dx, y + dy, z);
        angle += angleStep;
    }
    glEnd();
}

void drawTruck() {
    // Draw the body
    glColor3f(0.8, 0.2, 0.2);
    glPushMatrix();
    glTranslatef(0, 0, -1);
    glScalef(2, 1, 1);
    glutSolidCube(2);
    glPopMatrix();

    // Draw the wheels
    drawWheel(-1, -1, -1, 0.5);
    drawWheel(1, -1, -1, 0.5);
    drawWheel(-1, -1, 1, 0.5);
    drawWheel(1, -1, 1, 0.5);

    // Draw the windows
    glColor3f(0.8, 0.8, 1.0);
    glPushMatrix();
    glTranslatef(0, 0.5, -0.9);
    glScalef(1.8, 0.8, 0.1);
    glutSolidCube(2);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 5, 5, 0, 0, 0, 0, 1, 0);
    drawTruck();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Truck");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
