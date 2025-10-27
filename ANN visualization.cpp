#include <GL/freeglut.h>
#include <cmath>
#include <iostream>
#include <sstream>

// Window size
int winW = 1000, winH = 700;
float angle = 0.0f;
bool forwardPass = true;
float animProgress = 0.0f;
float errorValue = 0.25f;
int epoch = 0;

// Structure for neuron position
struct Neuron {
    float x, y, z;
};

// Layer setup
Neuron inputLayer[3];
Neuron hiddenLayer[4];
Neuron outputLayer[2];

// ==========================
// 🧩 Draw Text Function
// ==========================
void drawText(const char* text, int x, int y) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    gluOrtho2D(0, viewport[2], 0, viewport[3]);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2i(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ==========================
// 🎨 Draw a neuron (sphere)
// ==========================
void drawNeuron(Neuron n, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(n.x, n.y, n.z);
    glColor3f(r, g, b);
    glutSolidSphere(0.2, 20, 20);
    glPopMatrix();
}

// ==========================
// ⚡ Draw connection lines
// ==========================
void drawConnection(Neuron a, Neuron b, float intensity, bool forward) {
    glBegin(GL_LINES);
    if (forward)
        glColor3f(0.1f, intensity, 1.0f); // Blue glow
    else
        glColor3f(1.0f, 0.1f, intensity); // Red glow

    glVertex3f(a.x, a.y, a.z);
    glVertex3f(b.x, b.y, b.z);
    glEnd();
}

// ==========================
// 🧠 Setup layers position
// ==========================
void setupNetwork() {
    for (int i = 0; i < 3; i++)
        inputLayer[i] = { -4.0f, (float)(i - 1) * 1.5f, 0.0f };

    for (int i = 0; i < 4; i++)
        hiddenLayer[i] = { 0.0f, (float)(i - 1.5f) * 1.2f, 0.0f };

    for (int i = 0; i < 2; i++)
        outputLayer[i] = { 4.0f, (float)(i - 0.5f) * 1.5f, 0.0f };
}

// ==========================
// 🌀 Animation control
// ==========================
void update(int value) {
    animProgress += 0.02f;
    if (animProgress >= 1.0f) {
        animProgress = 0.0f;
        forwardPass = !forwardPass; // Switch direction
        epoch++;
        errorValue *= 0.95f; // Gradual error reduction
    }
    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

// ==========================
// 🪄 Render Scene
// ==========================
void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 0, 15, 0, 0, 0, 0, 1, 0);

    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // === Draw connections ===
    float intensity = fabs(sin(animProgress * 3.14f));

    // Input → Hidden
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 4; j++)
            drawConnection(inputLayer[i], hiddenLayer[j], intensity, forwardPass);

    // Hidden → Output
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 2; j++)
            drawConnection(hiddenLayer[i], outputLayer[j], intensity, forwardPass);

    // === Draw neurons ===
    for (int i = 0; i < 3; i++)
        drawNeuron(inputLayer[i], 0.2f, 0.8f, 0.2f); // Green input

    for (int i = 0; i < 4; i++)
        drawNeuron(hiddenLayer[i], 0.9f, 0.9f, 0.2f); // Yellow hidden

    for (int i = 0; i < 2; i++)
        drawNeuron(outputLayer[i], 0.8f, 0.2f, 0.2f); // Red output

    // === Overlay Info ===
    std::stringstream ss;
    ss << "Epoch: " << epoch << "   Error: " << errorValue;
    std::string s = ss.str();
    drawText(s.c_str(), 10, winH - 20);

    glutSwapBuffers();
}

// ==========================
// ⚙️ Initialize
// ==========================
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
    glShadeModel(GL_SMOOTH);
}

// ==========================
// 📏 Resize handler
// ==========================
void reshape(int w, int h) {
    winW = w; winH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (float)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ==========================
// 🧭 Keyboard control
// ==========================
void keyboard(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
    if (key == 'a') angle -= 5;
    if (key == 'd') angle += 5;
    glutPostRedisplay();
}

// ==========================
// 🚀 Main Function
// ==========================
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(winW, winH);
    glutCreateWindow("3D ANN Backpropagation Visualization");

    initGL();
    setupNetwork();

    glutDisplayFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50, update, 0);

    glutMainLoop();
    return 0;
}
