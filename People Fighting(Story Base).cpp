#include <GL/glut.h>
#include <cmath>
#include <cstring>

// ---------------- Variables ----------------
float man1X = -0.6f, man1Y = -0.3f;
float man2X = 0.6f, man2Y = -0.3f;
bool fighting = false;
bool collided = false;
int dialogueStep = 0;
int timerCount = 0;

// ---------------- Text Display ----------------
void displayText(const char* text, float x, float y) {
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; i < strlen(text); i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

// ---------------- Stickman ----------------
void drawMan(float x, float y, float r, float g, float b) {
    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float angle = i * 3.1416f / 180.0f;
        glVertex2f(x + 0.05f * cos(angle), y + 0.05f * sin(angle));
    }
    glEnd();

    // Body
    glColor3f(r, g, b);
    glBegin(GL_LINES);
    glVertex2f(x, y - 0.05f);
    glVertex2f(x, y - 0.25f);
    glEnd();

    // Arms
    glBegin(GL_LINES);
    glVertex2f(x, y - 0.1f);
    glVertex2f(x - 0.1f, y - 0.15f);
    glVertex2f(x, y - 0.1f);
    glVertex2f(x + 0.1f, y - 0.15f);
    glEnd();

    // Legs
    glBegin(GL_LINES);
    glVertex2f(x, y - 0.25f);
    glVertex2f(x - 0.08f, y - 0.35f);
    glVertex2f(x, y - 0.25f);
    glVertex2f(x + 0.08f, y - 0.35f);
    glEnd();
}

// ---------------- Crowd ----------------
void drawCrowd() {
    glColor3f(0.2f, 0.2f, 0.2f);
    for (float i = -0.9f; i <= 0.9f; i += 0.1f) {
        glBegin(GL_POLYGON);
        for (int j = 0; j < 360; j++) {
            float angle = j * 3.1416f / 180.0f;
            glVertex2f(i + 0.02f * cos(angle), -0.1f + 0.02f * sin(angle));
        }
        glEnd();
    }
}

// ---------------- Background ----------------
void drawBackground() {
    // Sky
    glColor3f(0.53f, 0.81f, 0.92f);
    glBegin(GL_QUADS);
    glVertex2f(-1, 0);
    glVertex2f(1, 0);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glEnd();

    // Ground
    glColor3f(0.4f, 0.8f, 0.4f);
    glBegin(GL_QUADS);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glVertex2f(1, 0);
    glVertex2f(-1, 0);
    glEnd();
}

// ---------------- Fighting Animation ----------------
void updateFight() {
    if (fighting && !collided) {
        man1X += 0.01f;
        man2X -= 0.01f;

        if (fabs(man1X - man2X) < 0.15f) {
            collided = true;
            fighting = false;
            dialogueStep = 2; // Move to next dialogue
        }
    }

    if (collided) {
        // Simple fighting movement
        man1X += 0.005f * sin(timerCount * 0.2f);
        man2X -= 0.005f * sin(timerCount * 0.2f);
    }
}

// ---------------- Display ----------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawBackground();
    drawCrowd();

    // Draw Men
    drawMan(man1X, man1Y, 0, 0, 1); // Blue man
    drawMan(man2X, man2Y, 1, 0, 0); // Red man

    // Display dialogues
    if (dialogueStep == 0)
        displayText("Man 1: You stole my idea!", -0.9f, 0.85f);
    else if (dialogueStep == 1)
        displayText("Man 2: It was mine first!", -0.9f, 0.85f);
    else if (dialogueStep == 2)
        displayText("They start fighting!", -0.9f, 0.85f);
    else if (dialogueStep == 3)
        displayText("Crowd: Fight! Fight! Fight!", -0.9f, 0.85f);

    glutSwapBuffers();
}

// ---------------- Timer ----------------
void timer(int value) {
    timerCount++;

    // Dialogue timing control
    if (timerCount == 100) dialogueStep = 1;
    if (timerCount == 200 && !fighting) fighting = true;
    if (timerCount == 400) dialogueStep = 3;

    updateFight();

    glutPostRedisplay();
    glutTimerFunc(50, timer, 0);
}

// ---------------- Keyboard ----------------
void keyboard(unsigned char key, int x, int y) {
    if (key == 'f' || key == 'F') {
        fighting = true;
        collided = false;
        dialogueStep = 1;
        timerCount = 0;
    }
}

// ---------------- Init ----------------
void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

// ---------------- Main ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Two Men Fighting - OpenGL Story Animation");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, timer, 0);

    glutMainLoop();
    return 0;
}
