// story_scenes.cpp
// Single GLUT program with 10 story-type mini-scenes (press 1..9 and 0 for 10).
// Compile (Linux): g++ story_scenes.cpp -lGL -lGLU -lglut -o story_scenes

#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <cstdio>

// Globals
int windowW = 800, windowH = 600;
int currentScene = 1;   // 1..10 (0 key -> 10)
bool running = false;
int tcount = 0;

// Utility: draw text
void drawText(const char* s, float x, float y) {
    glColor3f(0, 0, 0);
    glRasterPos2f(x, y);
    for (int i = 0; s[i]; ++i) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
}

// Utility: circle
void drawCircle(float cx, float cy, float r, int num_segments = 64) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; ++i) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Scene 1: AI vs Human — two characters debate then cooperate
void scene1_draw() {
    // background
    glColor3f(0.9f, 0.95f, 1.0f);
    glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();

    // human (left)
    float hx = -0.6f + 0.2f * (sinf(tcount * 0.05f) * 0.2f);
    glColor3f(1.0f, 0.8f, 0.6f); drawCircle(hx, -0.1f, 0.08f); // head
    glColor3f(0.2f, 0.4f, 1.0f); glBegin(GL_LINES); glVertex2f(hx, -0.18f); glVertex2f(hx, -0.40f); glEnd(); // body
    // robot (right)
    float rx = 0.6f - 0.2f * (sinf(tcount * 0.05f) * 0.2f);
    glColor3f(0.7f, 0.8f, 0.9f); glBegin(GL_QUADS); glVertex2f(rx - 0.07f, -0.05f); glVertex2f(rx + 0.07f, -0.05f); glVertex2f(rx + 0.07f, -0.18f); glVertex2f(rx - 0.07f, -0.18f); glEnd(); // head
    glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_LINES); glVertex2f(rx, -0.18f); glVertex2f(rx, -0.40f); glEnd();

    // dialogue logic
    if (!running) {
        drawText("Scene 1: AI vs Human. Press 's' to start. Press 2..0 for other scenes.", -0.95f, 0.9f);
    }
    else {
        if (tcount < 80) drawText("Human: Machines will take our jobs!", -0.9f, 0.8f);
        else if (tcount < 160) drawText("Robot: I can augment your work, not replace it.", -0.9f, 0.8f);
        else drawText("They cooperate: Human + AI = Better outcomes", -0.9f, 0.8f);
    }
}

// Scene 2: Climate Change — pollution -> cleanup -> green
void scene2_draw() {
    // sky changes from gray to blue depending on tcount
    float mix = running ? fmin(1.0f, tcount / 200.0f) : 0.0f;
    glColor3f(0.6f * (1.0f - mix) + 0.53f * mix, 0.6f * (1.0f - mix) + 0.81f * mix, 0.6f * (1.0f - mix) + 0.92f * mix);
    glBegin(GL_QUADS); glVertex2f(-1, 0.2f); glVertex2f(1, 0.2f); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();

    // factories / pollution left
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS); glVertex2f(-0.95f, -0.3f); glVertex2f(-0.7f, -0.3f); glVertex2f(-0.7f, 0.2f); glVertex2f(-0.95f, 0.2f); glEnd();
    drawText("Factory", -0.92f, -0.35f);
    // smoke (animated)
    if (!running || tcount < 60) {
        glColor3f(0.15f, 0.15f, 0.15f);
        drawCircle(-0.82f, 0.33f + 0.02f * (sin(tcount * 0.1f)), 0.06f);
        drawCircle(-0.75f, 0.42f + 0.02f * (sin(tcount * 0.09f)), 0.05f);
    } // when running and tcount grows, smoke fades.

    // trees appear to the right
    float treeY = -0.6f;
    for (int i = 0;i < 6;i++) {
        float x = -0.3f + i * 0.2f;
        float green = 0.2f + 0.8f * fmin(1.0f, (running ? (tcount / 220.0f) : 0.0f));
        glColor3f(0.5f * green, 0.7f * green, 0.3f * green);
        drawCircle(x, treeY + 0.25f, 0.12f);
        glColor3f(0.45f, 0.27f, 0.07f); glBegin(GL_QUADS); glVertex2f(x - 0.02f, treeY + 0.1f); glVertex2f(x + 0.02f, treeY + 0.1f); glVertex2f(x + 0.02f, treeY - 0.12f); glVertex2f(x - 0.02f, treeY - 0.12f); glEnd();
    }

    if (!running) drawText("Scene 2: Climate Change. Press 's' to start cleanup.", -0.95f, 0.9f);
    else if (tcount < 200) drawText("People clean up and plant trees...", -0.9f, 0.85f);
    else drawText("Result: Cleaner air and more trees.", -0.9f, 0.85f);
}

// Scene 3: Public Health (dengue) — dirty water, mosquito -> cleanup
void scene3_draw() {
    // background
    glColor3f(0.8f, 0.95f, 1.0f); glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();

    // water puddle (breeding) on left that disappears after cleanup
    float puddleX = -0.6f;
    float alpha = running ? 1.0f - fmin(1.0f, tcount / 120.0f) : 1.0f;
    glColor4f(0.2f, 0.4f, 1.0f, alpha);
    drawCircle(puddleX, -0.5f, 0.12f);

    // mosquitoes (small moving points)
    glColor3f(0, 0, 0);
    for (int i = 0;i < 6;i++) {
        float mx = -0.7f + 0.15f * (sin(tcount * 0.05f + i));
        float my = -0.45f + 0.05f * cos(tcount * 0.07f + i);
        if (alpha > 0.05f) drawCircle(mx, my, 0.01f);
    }

    // people (right)
    for (int i = 0;i < 5;i++) {
        float px = 0.2f + i * 0.12f;
        glColor3f(1, 0.8f, 0.6f); drawCircle(px, -0.4f, 0.05f);
    }

    if (!running) drawText("Scene 3: Dengue Awareness. Press 's' to start clean-up.", -0.95f, 0.9f);
    else if (tcount < 120) drawText("Dirty water present -> mosquitoes breed", -0.9f, 0.85f);
    else drawText("People emptied water and cleaned. Mosquitoes gone!", -0.9f, 0.85f);
}

// Scene 4: Cybersecurity — hacker tries, firewall defends
void scene4_draw() {
    // dark background
    float bg = 0.07f + 0.4f * fmin(1.0f, tcount / 200.0f);
    glColor3f(bg, bg, bg + 0.1f); glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();

    // computer/server in center
    glColor3f(0.2f, 0.2f, 0.3f); glBegin(GL_QUADS); glVertex2f(-0.25f, -0.15f); glVertex2f(0.25f, -0.15f); glVertex2f(0.25f, 0.15f); glVertex2f(-0.25f, 0.15f); glEnd();
    drawText("Server", -0.05f, 0.02f);

    // hacker on left (red dot), data packet moves
    float hx = -0.9f + 0.5f * (sin(tcount * 0.03f));
    glColor3f(1, 0.2f, 0.2f); drawCircle(hx, 0.0f, 0.04f);
    // packets: red moving right
    glColor3f(1, 0.4f, 0.4f);
    for (int i = 0;i < 4;i++) {
        float px = -0.9f + ((tcount * 0.02f + i * 0.25f) - floor((tcount * 0.02f + i * 0.25f))) * 2.0f;
        glBegin(GL_QUADS); glVertex2f(px - 0.02f, -0.05f); glVertex2f(px + 0.02f, -0.05f); glVertex2f(px + 0.02f, 0.05f); glVertex2f(px - 0.02f, 0.05f); glEnd();
    }

    // firewall shield (appears when running)
    if (running) {
        float shield = 0.4f + 0.2f * sin(tcount * 0.12f);
        glColor3f(0.2f, 0.6f, 0.9f); glBegin(GL_LINE_LOOP);
        for (int i = 0;i < 64;i++) {
            float a = 2 * 3.14159f * i / 64.0f; glVertex2f(0.0f + shield * cos(a), 0.0f + shield * sin(a));
        }
        glEnd();
        drawText("Active Firewall", -0.12f, -0.25f);
    }
    else {
        drawText("Scene 4: Cybersecurity. Press 's' to enable defense (firewall).", -0.95f, 0.9f);
    }
}

// Scene 5: Smart City — moving cars, traffic light optimization
void scene5_draw() {
    // sky + buildings
    glColor3f(0.6f, 0.8f, 1.0f); glBegin(GL_QUADS); glVertex2f(-1, 0.0f); glVertex2f(1, 0.0f); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();
    glColor3f(0.9f, 0.9f, 0.9f);
    for (int i = 0;i < 4;i++) {
        float x = -0.9f + i * 0.6f;
        glBegin(GL_QUADS); glVertex2f(x, -0.1f); glVertex2f(x + 0.4f, -0.1f); glVertex2f(x + 0.4f, 0.6f); glVertex2f(x, 0.6f); glEnd();
    }

    // road
    glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_QUADS); glVertex2f(-1, -0.5f); glVertex2f(1, -0.5f); glVertex2f(1, -0.15f); glVertex2f(-1, -0.15f); glEnd();
    // cars (moving) - more organized when running
    for (int i = 0;i < 6;i++) {
        float speed = running ? 0.01f : 0.005f;
        float x = -1.2f + fmod(tcount * speed + i * 0.35f, 3.0f) - 1.0f;
        glColor3f((i % 2) ? 0.9f : 0.2f, 0.2f, (i % 2) ? 0.2f : 0.9f);
        glBegin(GL_QUADS); glVertex2f(x, -0.45f); glVertex2f(x + 0.2f, -0.45f); glVertex2f(x + 0.2f, -0.33f); glVertex2f(x, -0.33f); glEnd();
    }

    if (!running) drawText("Scene 5: Smart City (traffic). Press 's' to enable smart control.", -0.95f, 0.9f);
    else drawText("Smart control active: traffic flows smoothly.", -0.95f, 0.9f);
}

// Scene 6: Renewable Energy — solar panels and wind turbines
void scene6_draw() {
    // sky
    glColor3f(0.5f, 0.8f, 1.0f); glBegin(GL_QUADS); glVertex2f(-1, 0.1f); glVertex2f(1, 0.1f); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();
    // sun
    glColor3f(1, 0.9f, 0.0f); drawCircle(0.7f, 0.8f, 0.12f);

    // solar panels (left)
    for (int i = 0;i < 3;i++) {
        float x = -0.9f + i * 0.35f;
        glColor3f(0.1f, 0.1f, 0.4f); glBegin(GL_QUADS); glVertex2f(x, -0.1f); glVertex2f(x + 0.25f, -0.1f); glVertex2f(x + 0.25f, 0.05f); glVertex2f(x, -0.05f); glEnd();
    }
    // wind turbines (right)
    for (int i = 0;i < 3;i++) {
        float x = 0.2f + i * 0.25f;
        glColor3f(0.9f, 0.9f, 0.9f); glBegin(GL_LINES); glVertex2f(x, -0.1f); glVertex2f(x, 0.4f); glEnd();
        // blades rotate
        glPushMatrix();
        glTranslatef(x, 0.4f, 0);
        glRotatef(tcount * 3.0f + i * 30.0f, 0, 0, 1);
        glColor3f(0.95f, 0.95f, 0.95f);
        glBegin(GL_TRIANGLES); glVertex2f(0, 0); glVertex2f(0.15f, 0.03f); glVertex2f(0.05f, 0.06f); glEnd();
        glBegin(GL_TRIANGLES); glVertex2f(0, 0); glVertex2f(-0.15f, 0.03f); glVertex2f(-0.05f, 0.06f); glEnd();
        glBegin(GL_TRIANGLES); glVertex2f(0, 0); glVertex2f(0.0f, -0.15f); glVertex2f(0.06f, -0.05f); glEnd();
        glPopMatrix();
    }

    if (!running) drawText("Scene 6: Renewable Energy. Press 's' to animate turbines.", -0.95f, 0.9f);
    else drawText("Solar + Wind generating clean energy.", -0.95f, 0.9f);
}

// Scene 7: Space Exploration — rocket launch and planets
void scene7_draw() {
    // star background
    glColor3f(0.02f, 0.02f, 0.08f); glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();
    // stars
    glColor3f(1, 1, 1);
    for (int i = 0;i < 40;i++) {
        float sx = -1.0f + (i * 0.137f);
        float sy = -0.9f + fmod(i * 0.213f + tcount * 0.001f, 1.8f);
        drawCircle(sx, sy, 0.004f);
    }
    // rocket (launch when running)
    float ry = running ? -0.9f + fmin(1.8f, tcount * 0.02f) : -0.9f;
    glColor3f(0.9f, 0.1f, 0.1f); glBegin(GL_TRIANGLES); glVertex2f(-0.05f, ry + 0.1f); glVertex2f(0.05f, ry + 0.1f); glVertex2f(0, ry + 0.35f); glEnd();
    glColor3f(0.7f, 0.7f, 0.7f); glBegin(GL_QUADS); glVertex2f(-0.04f, ry - 0.1f); glVertex2f(0.04f, ry - 0.1f); glVertex2f(0.04f, ry + 0.1f); glVertex2f(-0.04f, ry + 0.1f); glEnd();
    if (!running) drawText("Scene 7: Space Exploration. Press 's' to launch rocket.", -0.95f, 0.9f);
    else if (ry < 1.1f) drawText("Rocket launching...", -0.95f, 0.9f);
    else drawText("Rocket reached space! Explore planets.", -0.95f, 0.9f);
}

// Scene 8: Mental Health — stressed to calm transition
void scene8_draw() {
    // background color transitions from hot to calm
    float mix = running ? fmin(1.0f, tcount / 200.0f) : 0.0f;
    glColor3f(1.0f * (1 - mix) + 0.7f * mix, 0.5f * (1 - mix) + 0.9f * mix, 0.3f * (1 - mix) + 1.0f * mix);
    glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();

    // person
    glColor3f(1, 0.8f, 0.6f); drawCircle(0, -0.1f, 0.12f);
    // stress lines
    if (!running || tcount < 80) {
        glColor3f(0.8f, 0.1f, 0.1f);
        glBegin(GL_LINES); glVertex2f(0.2f, 0.2f); glVertex2f(0.05f, 0.05f); glVertex2f(-0.2f, 0.2f); glVertex2f(-0.05f, 0.05f); glEnd();
        drawText("Stressed", -0.12f, -0.4f);
    }
    else {
        // calm waves
        for (int i = 0;i < 4;i++) {
            glColor3f(0.0f, 0.3f + 0.2f * i, 0.5f);
            glBegin(GL_LINE_STRIP);
            for (int a = 0;a < 180;a += 10) {
                float ang = a * 3.14159f / 180.0f;
                glVertex2f(-0.5f + i * 0.25f + 0.2f * cos(ang + tcount * 0.02f), -0.6f + 0.05f * sin(ang + tcount * 0.02f));
            }
            glEnd();
        }
        drawText("Calm achieved: breathe, meditate", -0.4f, -0.4f);
    }

    if (!running) drawText("Scene 8: Mental Health. Press 's' to calm down.", -0.95f, 0.9f);
}

// Scene 9: Evolution of Technology — timeline
void scene9_draw() {
    // timeline across x axis
    glColor3f(0.95f, 0.95f, 0.95f); glBegin(GL_QUADS); glVertex2f(-1, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(-1, 1); glEnd();
    glColor3f(0.2f, 0.2f, 0.2f); glBegin(GL_LINES); glVertex2f(-0.9f, 0.0f); glVertex2f(0.9f, 0.0f); glEnd();
    // markers: stone, steam, computer, ai
    float pos[4] = { -0.8f, -0.25f, 0.25f, 0.7f };
    // stone
    glColor3f(0.5f, 0.4f, 0.3f); drawCircle(pos[0], 0.0f, 0.06f); drawText("Stone Age", pos[0] - 0.07f, -0.15f);
    // steam (chimney)
    glColor3f(0.3f, 0.3f, 0.3f); glBegin(GL_QUADS); glVertex2f(pos[1] - 0.04f, -0.05f); glVertex2f(pos[1] + 0.04f, -0.05f); glVertex2f(pos[1] + 0.04f, 0.15f); glVertex2f(pos[1] - 0.04f, 0.15f); glEnd(); drawText("Industrial", pos[1] - 0.07f, -0.15f);
    // computer
    glColor3f(0.2f, 0.2f, 0.5f); glBegin(GL_QUADS); glVertex2f(pos[2] - 0.06f, -0.05f); glVertex2f(pos[2] + 0.06f, -0.05f); glVertex2f(pos[2] + 0.06f, 0.08f); glVertex2f(pos[2] - 0.06f, 0.08f); glEnd(); drawText("Digital", pos[2] - 0.05f, -0.15f);
    // AI (brain)
    glColor3f(0.9f, 0.6f, 0.2f); drawCircle(pos[3], 0.05f, 0.07f); drawText("AI Future", pos[3] - 0.05f, -0.15f);

    if (!running) drawText("Scene 9: Evolution of Technology. Press 's' to animate.", -0.95f, 0.9f);
    else {
        // highlight moving cursor along timeline
        float cursorX = -0.9f + fmin(1.8f, tcount * 0.01f);
        glColor3f(1, 0, 0); drawCircle(cursorX, 0.0f, 0.02f);
        drawText("Progress ->", 0.5f, 0.4f);
    }
}

// Scene 10: War vs Peace — conflict then reconciliation
void scene10_draw() {
    // split screen color: left red-ish (war), right green-ish (peace)
    glBegin(GL_QUADS);
    glColor3f(0.6f, 0.2f, 0.2f); glVertex2f(-1, -1); glVertex2f(0, -1); glVertex2f(0, 1); glVertex2f(-1, 1);
    glColor3f(0.3f, 0.7f, 0.3f); glVertex2f(0, -1); glVertex2f(1, -1); glVertex2f(1, 1); glVertex2f(0, 1);
    glEnd();

    // two armies approaching center when running
    float leftX = -0.9f + (running ? fmin(0.8f, tcount * 0.004f) : 0.0f);
    float rightX = 0.9f - (running ? fmin(0.8f, tcount * 0.004f) : 0.0f);
    // soldiers as small circles
    for (int i = 0;i < 5;i++) {
        drawCircle(leftX + i * 0.06f, -0.3f + (i % 2) * 0.03f, 0.025f);
        drawCircle(rightX - i * 0.06f, -0.3f + (i % 2) * 0.03f, 0.025f);
    }

    if (!running) drawText("Scene 10: War vs Peace. Press 's' to start conflict -> resolution.", -0.95f, 0.9f);
    else if (tcount < 200) drawText("Conflict escalates...", -0.5f, 0.6f);
    else drawText("Peace achieved: They reconcile and children play", -0.3f, 0.6f);
    // after enough time show children playing in center (peace)
    if (running && tcount > 260) {
        drawCircle(0.0f, -0.4f, 0.03f);
        drawCircle(0.08f, -0.42f, 0.03f);
        drawCircle(-0.08f, -0.42f, 0.03f);
    }
}

// Main display
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (currentScene) {
    case 1: scene1_draw(); break;
    case 2: scene2_draw(); break;
    case 3: scene3_draw(); break;
    case 4: scene4_draw(); break;
    case 5: scene5_draw(); break;
    case 6: scene6_draw(); break;
    case 7: scene7_draw(); break;
    case 8: scene8_draw(); break;
    case 9: scene9_draw(); break;
    case 10: scene10_draw(); break;
    default: break;
    }

    // footer instructions
    glColor3f(0, 0, 0);
    char footer[256];
    sprintf(footer, "Scene %d. Keys: 1..9,0 -> switch scenes | s:start | r:reset", currentScene);
    drawText(footer, -0.95f, -0.95f);

    glutSwapBuffers();
}

// Timer
void timerFunc(int v) {
    if (running) tcount++;
    glutPostRedisplay();
    glutTimerFunc(33, timerFunc, 0); // ~30 FPS
}

// Keyboard input
void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '9') {
        currentScene = key - '0';
        running = false; tcount = 0;
    }
    else if (key == '0') { // 0 -> scene 10
        currentScene = 10; running = false; tcount = 0;
    }
    else if (key == 's' || key == 'S') {
        running = true; tcount = 0;
    }
    else if (key == 'r' || key == 'R') {
        running = false; tcount = 0;
    }
    else if (key == 27) { // ESC
        exit(0);
    }
}

// Init & reshape
void init() {
    glClearColor(1, 1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

void reshape(int w, int h) {
    windowW = w; windowH = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowW, windowH);
    glutCreateWindow("Multi-Scene Storyboard: 10 Trending Topics");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(33, timerFunc, 0);

    printf("Multi-scene demo. Keys: 1..9,0 switch scenes; s start; r reset; ESC exit\n");
    glutMainLoop();
    return 0;
}
