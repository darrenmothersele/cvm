#include <stack>
#include <iostream>
#include <GLFW/glfw3.h>

#define I_PUSH_F        1
#define I_CLEAR         2
#define I_LOAD          3
#define I_START_SHAPE   4
#define I_END_SHAPE     5
#define I_COLOUR        6
#define I_VERTEX        7
#define I_MULT          8
#define I_ROTATE        9

#define R_TIME 0

float registers[1];

// CLEAR 0,0,0,0
// ROTATE TIME * 50, 0,0,1
// SHAPE
//  COL 1,0,0
//  VERTEX -0.6, -0.4, 0
//  COL 0,1,0
//  VERTEX 0.6, -0.4, 0
//  COL 0,0,1
//  VERTEX 0, 0.6, 0
// END_SHAPE


using namespace std;

struct prog_data_t {
    int instr;
    float value;
};

stack<float> stk;


void error_callback(int error, const char* message)
{
    cerr << message << endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    // you can treat the code point argument as native endian UTF-32
    cout << (char)codepoint << endl;
}

int main() {

    if (!glfwInit())
    {
        cerr << "Failed to initialize GLFW" << endl;
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
//    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
//    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
//    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
//    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height,    "Cyril", monitor, NULL);

//    glfwWindowHint(GLFW_DECORATED, false);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Cyril", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    ratio = width / (float) height;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);

    struct prog_data_t prog[] = {
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.0},
            {I_CLEAR, 0.0}, // 5
            {I_PUSH_F, 0.5},
            {I_LOAD, R_TIME},
            {I_MULT, 0.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.5},
            {I_PUSH_F, 1.0},
            {I_ROTATE, 0.0}, // 12
            {I_START_SHAPE, 0},

            {I_PUSH_F, 1.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 1.0},
            {I_COLOUR, 0},
            {I_PUSH_F, -0.6f},
            {I_PUSH_F, -0.4f},
            {I_PUSH_F, 0.0},
            {I_VERTEX, 0}, // 22

            {I_PUSH_F, 0.0},
            {I_PUSH_F, 1.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 1.0},
            {I_COLOUR, 0},
            {I_PUSH_F, 0.6f},
            {I_PUSH_F, -0.4f},
            {I_PUSH_F, 0.0},
            {I_VERTEX, 0}, // 31

            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 1.0},
            {I_PUSH_F, 1.0},
            {I_COLOUR, 0},
            {I_PUSH_F, 0.0},
            {I_PUSH_F, 0.6f},
            {I_PUSH_F, 0.0},
            {I_VERTEX, 0}, // 40

            {I_END_SHAPE, 0}
    };
    int prog_size = 41;

    while (!glfwWindowShouldClose(window))
    {
        int pc = 0;

        registers[R_TIME] = (float) glfwGetTime();

//        double ypos;
//        glfwGetCursorPos(window, &time, &ypos);

//
//        glBegin(GL_TRIANGLES);
//        glColor3f(1.f, 0.f, 0.f);
//        glVertex3f(-0.6f, -0.4f, 0.f);
//        glColor3f(0.f, 1.f, 0.f);
//        glVertex3f(0.6f, -0.4f, 0.f);
//        glColor3f(0.f, 0.f, 1.f);
//        glVertex3f(0.f, 0.6f, 0.f);
//        glEnd();


        // CLEAR 0,0,0,0
        // ROTATE TIME * 50, 0,0,1
        // SHAPE
        //  COL 1,0,0
        //  VERTEX -0.6, -0.4, 0
        //  COL 0,1,0
        //  VERTEX 0.6, -0.4, 0
        //  COL 0,0,1
        //  VERTEX 0, 0.6, 0
        // END_SHAPE


        float r,g,b,a;
        float x,y,z;

        while (pc < prog_size)
        {

            switch (prog[pc].instr)
            {
                case I_PUSH_F:
                    stk.push(prog[pc].value);
                    break;
                case I_CLEAR:
                    a = stk.top(); stk.pop();
                    b = stk.top(); stk.pop();
                    g = stk.top(); stk.pop();
                    r = stk.top(); stk.pop();
                    glClearColor(r, g, b, a);
                    glClear(GL_COLOR_BUFFER_BIT);
                    break;
                case I_LOAD:
                    stk.push(registers[(int) prog[pc].value]);
                    break;
                case I_MULT:
                    b = stk.top(); stk.pop();
                    a = stk.top(); stk.pop();
                    stk.push(a * b);
                    cout << a << " * " << b << endl;
                    break;
                case I_ROTATE:
                    z = stk.top(); stk.pop();
                    y = stk.top(); stk.pop();
                    x = stk.top(); stk.pop();
                    a = stk.top(); stk.pop();
                    glRotatef(a, x, y, z);
                    cout << "Rotate " << a << "," << x << "," << y << "," << z << endl;
                    break;
                case I_START_SHAPE:
                    glBegin(GL_TRIANGLES);
                    break;
                case I_END_SHAPE:
                    glEnd();
                    break;
                case I_COLOUR:
                    a = stk.top(); stk.pop();
                    b = stk.top(); stk.pop();
                    g = stk.top(); stk.pop();
                    r = stk.top(); stk.pop();
                    glColor4f(r,g,b,a);
                    cout << "Color " << r << "," << g << "," << b << "," << a << endl;
                    break;
                case I_VERTEX:
                    z = stk.top(); stk.pop();
                    y = stk.top(); stk.pop();
                    x = stk.top(); stk.pop();
                    glVertex3f(x,y,z);
                    cout << "Vertex " << x << "," << y << "," << z << endl;
                    break;
                default:
                    break;
            }

//            glClearColor(0,0,0,0);
//            glClear(GL_COLOR_BUFFER_BIT);
//            glRotatef(registers[R_TIME] * 0.5f, 0.f, 0.f, 1.f);

            pc++;

        }

//        glBegin(GL_TRIANGLES);
//        glColor3f(1.f, 0.f, 0.f);
//        glVertex3f(-0.6f, -0.4f, 0.f);
//        glColor3f(0.f, 1.f, 0.f);
//        glVertex3f(0.6f, -0.4f, 0.f);
//        glColor3f(0.f, 0.f, 1.f);
//        glVertex3f(0.f, 0.6f, 0.f);
//        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}