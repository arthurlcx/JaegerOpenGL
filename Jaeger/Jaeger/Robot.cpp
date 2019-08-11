#include <Windows.h>
#include <Windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

float reactor = 1.0;

// mouse movement
float lastX = 0.0f, lastY = 0.0f;

float xRotated = 1.0f, yRotated = 1.0f, zRotated = -30.0f;

float x = 0.0f, y = 0.0f, z = 7.0f;

float radius = 1.0f;
float angle = 0.01f;
float baseRadius = 0.1f;
int slices = 20, stacks = 20;

float headAngle1 = 180.0f;

float fingersAngle = 150.0f;
float leftLegAngle1 = 180.0f, leftLegAngle2 = 180.0f, leftLegAngle3 = 180.0f;
float rightLegAngle1 = 180.0f, rightLegAngle2 = 180.0f, rightLegAngle3 = 180.0f;
float leftArmAngle1 = 180.0f, leftArmAngle2 = 180.0f, leftArmAngle3 = 180.0f;
float rightArmAngle1 = 180.0f, rightArmAngle2 = 180.0f, rightArmAngle3 = 180.0f;

float bodyJointAngle = 0.0f, upperBodyJointAngle = 0.0f, hipsJointAngle = 1.0f;

float leftLegAxis[3][3] = { { 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f } };
float rightLegAxis[3][3] = { { 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f },{ 1.0f,0.0f,0.0f } };

float movingDirection = 0.0f;
bool leftFrontMax = false, rightFrontMax = false, leftBackMax = false, rightBackMax = false;
bool firstWalk = true, leftFrontWalked = false, rightFrontWalked = false, leftBackWalked = false, rightBackWalked = true;

float reactorRotateAngle = 0.0f;
float cannonRotateAngle = 0.0f;

float xPosition = 0.0f, yPosition = 0.0f, zPosition = 0.05f;

float zoomLevel = -7.0f;
float seaWaveMovement = 1.0f;


float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
float mat_ambient_color[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float mat_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f };
float mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

bool lightOn = 0;

bool ambientOn = 1;
bool diffuseOn = 1;
bool specularOn = 1;

bool weather = 0;


float boneLength = 2.0f;

bool textureOn = false;

//Texture variable declaration
GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		switch (wParam) {
		case MK_LBUTTON:
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			zRotated += xPos - lastX;
			xRotated += yPos - lastY;
			lastX = xPos;
			lastY = yPos;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		lastX = GET_X_LPARAM(lParam);
		lastY = GET_Y_LPARAM(lParam);
		break;
	case WM_MOUSEWHEEL:
		zoomLevel += GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f;
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
		else if (wParam == VK_LEFT)
			reactor += 10.0;
		break;


	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

//Funtion to load bmp image as texture
void loadBitmapImage(const char *filename) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);


	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
}

//Function to end texture
void endTexture() {
	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP);
	glDeleteTextures(1, &texture);
}

void drawSphere(float radius, float slices, float stacks)
{
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_LINE);
	gluSphere(sphere, radius, slices, stacks);
	gluDeleteQuadric(sphere);
}

void drawCylinder(float base, float top, float height, float slices, float stacks)
{
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricNormals(cylinder, GLU_SMOOTH);
	gluQuadricTexture(cylinder, GLU_TRUE);
	gluCylinder(cylinder, base, top, height, slices, stacks);    //gluCylinder(GLUquadric obj *, baseRadius,topRadius, height,slices, stacks);
	gluDeleteQuadric(cylinder);
}

void drawFilledCube()
{
	glBegin(GL_QUADS);
	{
		// Top Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);

		// Left Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Back Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);

		// Right Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, 1.0f);

		// Bottom Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Front Face 
		glColor3f(1.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	}
	glEnd();
}

void drawFilledCube1()
{
	glBegin(GL_QUADS);
	{
		// Top Face 
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);

		// Left Face 
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Back Face 
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);

		// Right Face 
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, 1.0f);

		// Bottom Face 
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Front Face 
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	}
	glEnd();
}

void drawFilledTriangle()
{
	glBegin(GL_POLYGON);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 1.0, -0.1);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, 0.0);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(0.0, 0.0, -0.1);
	glVertex3f(0.0, 1.0, -0.1);
	glVertex3f(1.0, 0.0, -0.1);
	glVertex3f(1.0, 0.0, 0.0);
	glEnd();
}

void drawCircle(float radius)
{
	float xc = 0.0, yc = 0.0, xc2 = 0.0, yc2 = 0.0;
	float angleC = 0.0;

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(xc, yc);

	for (angleC - 0.1; angleC <= 360; angleC += 0.2)
	{
		xc2 = xc + sin(angleC) * radius;
		yc2 = yc + cos(angleC) * radius;
		glVertex2f(xc2, yc2);
	}
	glEnd();
}

void drawInnerLeftHand()
{

}

void drawInnerRightHand()
{

}

void drawInnerLeftLeg()
{
	// Left Leg
	glPushMatrix();     // Two horizontal cube (Front)
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(0.55, -0.28, 0.4);
	glScalef(0.38, 0.08, 0.08);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();            //(Back)
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(0.55, -0.28, -0.2);
	glScalef(0.38, 0.08, 0.08);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();       //Four thin cylinder
	glTranslatef(0.5, -0.28, 0.41);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.88, -0.28, 0.41);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5, -0.28, -0.20);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.88, -0.28, -0.20);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();          //Left thin cube after top 4 cylinder
	glTranslatef(0.689, -0.9, 0.106);
	glScalef(0.24, 0.02, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();          //Left thick cube before joint
	glTranslatef(0.689, -1.37, 0.106);
	glScalef(0.24, 0.45, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();       //Left Joint
	glTranslatef(0.42, -2.07, 0.1);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.25, 0.25, 0.55, 30, 30);
	glPopMatrix();

	glPushMatrix();          //Left thick cube after joint
	glTranslatef(0.689, -3.07, 0.106);
	glScalef(0.24, 0.75, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();          //Left ankle
	glTranslatef(0.69, -3.832, 0.02);
	glRotatef(45, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.347, 0.1, 0.3, 4, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.878, -4.116, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.28, 4.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.68, -3.98, 0.48);
	glScalef(0.2, 0.15, 0.18);
	drawFilledCube1();
	glPopMatrix();
}

void drawInnerRightLeg()
{
	//Right leg
	glPushMatrix();             //Two Horizontal cube
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(-0.55, -0.28, 0.4);
	glScalef(0.38, 0.08, 0.08);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(-0.55, -0.28, -0.2);
	glScalef(0.38, 0.08, 0.08);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();       //Four thin cylinder
	glTranslatef(-0.5, -0.28, 0.41);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.88, -0.28, 0.41);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.5, -0.28, -0.20);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.88, -0.28, -0.20);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.05, 0.05, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();          //Left thin cube after top 4 cylinder
	glTranslatef(-0.689, -0.9, 0.106);
	glScalef(0.24, 0.02, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();          //Right thick cube before joint
	glTranslatef(-0.689, -1.37, 0.106);
	glScalef(0.24, 0.45, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();       //Right Joint
	glTranslatef(-0.42, -2.07, 0.1);
	glRotatef(-90, 0.0, 1.0, 0.0);
	drawCylinder(0.25, 0.25, 0.55, 30, 30);
	glPopMatrix();

	glPushMatrix();          //Right thick cube after joint
	glTranslatef(-0.689, -3.07, 0.106);
	glScalef(0.24, 0.75, 0.36);
	drawFilledCube1();
	glPopMatrix();

	glPushMatrix();          //Left ankle
	glTranslatef(-0.69, -3.832, 0.02);
	glRotatef(45, 0.0, 1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.347, 0.1, 0.3, 4, 4);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.49, -4.116, 0.3);
	glRotatef(90, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.28, 4.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.68, -3.98, 0.48);
	glScalef(0.2, 0.15, 0.18);
	drawFilledCube1();
	glPopMatrix();
}

void drawInnerRobotPart()
{
	//Inner top body part
	//Head
	glPushMatrix();  // Inner Head
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(0.0, 3.4, 0.0);
	drawSphere(0.25, 60, 60);
	glPopMatrix();

	glPushMatrix(); // Inner Neck
	glColor3f(0.753, 0.753, 0.753);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 2.8);
	drawCylinder(0.2, 0.2, 0.5, 30, 30);
	glPopMatrix();

	//Body
	glPushMatrix(); //Top cylinder
	glColor3f(0.753, 0.753, 0.753);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 1.3);
	glScalef(0.5, 0.3, 0.7);
	drawCylinder(1.5, 2.0, 2.2, 30, 30);
	glPopMatrix();

	glPushMatrix(); //Bottom cylinder
	glColor3f(0.753, 0.753, 0.753);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, 0.0);
	glScalef(0.5, 0.3, 0.6);
	drawCylinder(1.3, 1.3, 2.3, 30, 30);
	glPopMatrix();

	glPushMatrix();  //Cube under bottom cylinder, between two leg
	glColor3f(0.753, 0.753, 0.753);
	glTranslatef(0.0, -0.15, 0.2);
	glScalef(0.2, 0.3, 0.6);
	drawFilledCube1();
	glPopMatrix();

	//Hand
		//Left Hand
	drawInnerLeftHand();

	//Right
	drawInnerRightHand();

	//Leg
		//Left Leg
	drawInnerLeftLeg();

	//Right Leg 
	drawInnerRightLeg();
}

void drawBackBone()
{
	//10 straight back pieces
	glColor3f(0.431, 0.431, 0.431);
	glPushMatrix();
	glTranslatef(0.0, 2.2, -0.68);
	glRotatef(-4, 1.0, 0.0, 0.0);
	glScalef(0.18, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.9, -0.66);
	glRotatef(-6, 1.0, 0.0, 0.0);
	glScalef(0.17, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.6, -0.63);
	glRotatef(-8, 1.0, 0.0, 0.0);
	glScalef(0.16, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.3, -0.60);
	glRotatef(-8, 1.0, 0.0, 0.0);
	glScalef(0.15, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.0, -0.57);
	glRotatef(-8, 1.0, 0.0, 0.0);
	glScalef(0.14, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.7, -0.54);
	glRotatef(-8, 1.0, 0.0, 0.0);
	glScalef(0.13, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.4, -0.51);
	glRotatef(-8, 1.0, 0.0, 0.0);
	glScalef(0.12, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 0.1, -0.48);
	glRotatef(-6, 1.0, 0.0, 0.0);
	glScalef(0.11, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -0.2, -0.45);
	glRotatef(-2, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.20, 0.03);
	drawFilledCube();
	glPopMatrix();

	//side pieces
	glColor3f(0.588, 0.588, 0.588);
	glPushMatrix();
	glTranslatef(-0.3, 2.2, -0.68);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.6, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.28, 1.9, -0.66);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.55, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.25, 1.6, -0.63);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.5, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.23, 1.3, -0.60);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.45, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.21, 1.0, -0.57);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.18, 0.7, -0.54);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.35, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.16, 0.4, -0.51);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.32, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.14, 0.1, -0.48);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.28, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.12, -0.2, -0.45);
	glRotatef(90, 0.0, 1.0, 0.0);
	drawCylinder(0.08, 0.08, 0.25, 20, 20);
	glPopMatrix();
}

void drawFrontArmour()
{

	//Front Laser + Reactor
	glPushMatrix();
		glTranslatef(0.0, 2.2, 1.2);
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.5);
		glVertex3f(-0.1, 0.1, 0.0);
		glVertex3f(0.1, 0.1, 0.0);
		glVertex3f(0.20, 0.0, 0.0);
		glVertex3f(-0.20, 0.0, 0.0);
		glEnd();
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.5);
		glVertex3f(-0.2, 0.0, 0.0);
		glVertex3f(0.2, 0.0, 0.0);
		glVertex3f(0.2, -0.2, 0.0);
		glVertex3f(-0.2, -0.2, 0.0);
		glEnd();
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 0.5);
		glVertex3f(-0.2, -0.2, 0.0);
		glVertex3f(0.2, -0.2, 0.0);
		glVertex3f(0.1, -0.3, 0.0);
		glVertex3f(-0.1, -0.3, 0.0);
		glEnd();
	glPopMatrix();

	glPushMatrix();             //Ironman Reactor       (how to rotate automatically in glPushMatrix)
		glColor3f(0.0, 1.0, 1.0);
		glTranslatef(0.0, 2.1, 0.85);
		glRotatef(reactor, 1.0, 1.0, 1.0);
		drawSphere(0.3, 30, 30);
	glPopMatrix();

	// Left Upper Shoulder (with missle)
	glPushMatrix();                 //Largest piece (Beside neck)
		glTranslatef(0.52, 3.0, 0.05);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.4, 0.02, 0.55);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Missle  (front cube for easy texture)
		glTranslatef(0.6, 3.1, 0.18);
		glScalef(0.2, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //back cube for easier texture
		glTranslatef(0.6, 3.1, -0.02);
		glScalef(0.2, 0.12, 0.18);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //Second largest piece (Steep piece after largest piece)
		glTranslatef(0.26, 2.78, 0.83);
		glRotatef(28, 1.0, 0.0, 0.0);
		glScalef(0.26, 0.02, 0.27);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //Triangle piece beside second largest piece
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.52, 2.89, 0.58);
		glRotatef(118, 1.0, 0.0, 0.0);
		glScalef(0.4, 0.55, 0.45);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();                 //Third piece
		glTranslatef(0.2, 2.48, 1.12);
		glRotatef(70, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.02, 0.19);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                  //Triangle piece beside third piece
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.31, 2.66, 1.05);
		glRotatef(160, 1.0, 0.0, 0.0);
		glScalef(0.22, 0.38, 0.45);
		drawFilledTriangle();
	glPopMatrix();


	glPushMatrix();                 //small cube piece at top right of laser
		glTranslatef(0.2, 2.3, 1.19);
		glRotatef(-10, 1.0, 0.0, 0.0);
		glRotatef(46, 0.0, 0.0, 1.0);
		glScalef(0.07, 0.07, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //small cube piece at bottom right of laser
		glTranslatef(0.2, 1.9, 1.19);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(46, 0.0, 0.0, 1.0);
		glScalef(0.07, 0.07, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //small piece at the right of laser
		glTranslatef(0.3, 2.11, 1.17);
		glRotatef(20, 0.0, 1.0, 0.0);
		glScalef(0.1, 0.2, 0.01);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //right piece beside the small piece beside laser
		glTranslatef(0.5, 2.0, 1.08);
		glRotatef(25, 0.0, 1.0, 0.0);
		glScalef(0.15, 0.4, 0.01);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //horizontal piece of it
		glTranslatef(0.36, 1.6, 1.13);
		glRotatef(20, 0.0, 1.0, 0.0);
		glRotatef(-75, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.45, 0.2);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.8, 2.13, 0.7);
		glRotatef(60, 0.0, 1.0, 0.0);
		glScalef(0.36, 0.55, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Left Side piece
		glTranslatef(0.98, 1.9, 0.03);
		glScalef(0.02, 0.3, 0.37);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.83, 2.06, -0.45);
		glRotatef(140, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.45, 0.02);
		drawFilledCube();
	glPopMatrix();

	//Right Upper Shoulder (with missle)
	glPushMatrix();                 //Largest piece (Beside neck)
		glTranslatef(-0.52, 3.0, 0.05);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.4, 0.02, 0.55);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Missle  (front cube for easy texture)
		glTranslatef(-0.6, 3.1, 0.18);
		glScalef(0.2, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //back cube for easier texture
		glTranslatef(-0.6, 3.1, -0.02);
		glScalef(0.2, 0.12, 0.18);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //Second largest piece  (After largest piece)
		glTranslatef(-0.26, 2.78, 0.83);
		glRotatef(28, 1.0, 0.0, 0.0);
		glScalef(0.26, 0.02, 0.27);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Triangle piece beside second largest piece
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(-0.52, 2.94, 0.59);
		glRotatef(118, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		glScalef(0.41, 0.55, 0.45);
		drawFilledTriangle();
	glPopMatrix();


	glPushMatrix();                 //Third piece
		glTranslatef(-0.2, 2.48, 1.12);
		glRotatef(70, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.02, 0.19);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //Triangle piece beside third piece
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(-0.32, 2.68, 1.08);
		glRotatef(160, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		glScalef(0.21, 0.38, 0.45);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();                 //small cube piece at top left of laser
		glTranslatef(-0.2, 2.3, 1.19);
		glRotatef(-10, 1.0, 0.0, 0.0);
		glRotatef(46, 0.0, 0.0, 1.0);
		glScalef(0.07, 0.07, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //small cube piece at bottom left of laser
		glTranslatef(-0.2, 1.9, 1.19);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(46, 0.0, 0.0, 1.0);
		glScalef(0.07, 0.07, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //small piece at the left of laser
		glTranslatef(-0.3, 2.11, 1.17);
		glRotatef(-20, 0.0, 1.0, 0.0);
		glScalef(0.1, 0.2, 0.01);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //left piece beside the small piece beside laser
		glTranslatef(-0.5, 2.0, 1.08);
		glRotatef(-25, 0.0, 1.0, 0.0);
		glScalef(0.15, 0.4, 0.01);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();               //horizontal piece of it
		glTranslatef(-0.36, 1.58, 1.13);
		glRotatef(-20, 0.0, 1.0, 0.0);
		glRotatef(105, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.45, 0.2);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.8, 2.13, 0.7);
		glRotatef(-60, 0.0, 1.0, 0.0);
		glScalef(0.36, 0.55, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Right Side piece
		glTranslatef(-0.98, 1.9, 0.03);
		glScalef(0.02, 0.3, 0.37);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.83, 2.06, -0.45);
		glRotatef(-140, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.45, 0.02);
		drawFilledCube();
	glPopMatrix();

	//Laser side
	glPushMatrix();                 //small piece at the bottom of laser (steep)
		glTranslatef(0.0, 1.8, 1.14);
		glRotatef(30, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.12, 0.01);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //small piece at the bottom of laser (horizontal)
		glTranslatef(0.0, 1.7, 0.9);
		glScalef(0.2, 0.02, 0.2);
		drawFilledCube();
	glPopMatrix();
}

void drawFrontBelly()
{
	//Belly part
	glPushMatrix();               //First
	glTranslatef(0.0, 1.43, 0.9);
	glScalef(0.3, 0.23, 0.16);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //First left
	glTranslatef(0.4, 1.43, 0.8);
	glScalef(0.15, 0.15, 0.16);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(0.555, 1.28, 0.645);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.32, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(0.835, 1.43, 0.46);
	glRotatef(70, 0.0, 1.0, 0.0);
	glScalef(0.20, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //First Right
	glTranslatef(-0.4, 1.43, 0.8);
	glScalef(0.15, 0.15, 0.16);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(-0.555, 1.58, 0.645);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.32, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(-0.835, 1.43, 0.46);
	glRotatef(-70, 0.0, 1.0, 0.0);
	glScalef(0.20, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Second
	glTranslatef(0.0, 1.12, 0.85);
	glScalef(0.26, 0.23, 0.15);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Second left
	glTranslatef(0.37, 1.12, 0.77);
	glScalef(0.12, 0.15, 0.14);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(0.495, 0.97, 0.638);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.28, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(0.77, 1.12, 0.45);
	glRotatef(70, 0.0, 1.0, 0.0);
	glScalef(0.22, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Second Right
	glTranslatef(-0.37, 1.12, 0.77);
	glScalef(0.12, 0.15, 0.14);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(-0.495, 1.27, 0.638);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.28, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(-0.77, 1.12, 0.45);
	glRotatef(-70, 0.0, 1.0, 0.0);
	glScalef(0.22, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Third
	glTranslatef(0.0, 0.81, 0.8);
	glScalef(0.22, 0.23, 0.14);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Third left
	glTranslatef(0.34, 0.81, 0.74);
	glScalef(0.12, 0.15, 0.12);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(0.464, 0.658, 0.625);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.236, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(0.755, 0.81, 0.41);
	glRotatef(70, 0.0, 1.0, 0.0);
	glScalef(0.23, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Third Right
	glTranslatef(-0.34, 0.81, 0.74);
	glScalef(0.12, 0.15, 0.12);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(-0.464, 0.96, 0.625);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.236, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(-0.755, 0.81, 0.41);
	glRotatef(-70, 0.0, 1.0, 0.0);
	glScalef(0.23, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Fourth
	glTranslatef(0.0, 0.5, 0.75);
	glScalef(0.18, 0.23, 0.13);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Fourth left
	glTranslatef(0.3, 0.5, 0.71);
	glScalef(0.12, 0.15, 0.10);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(0.425, 0.35, 0.615);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.2, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(0.72, 0.5, 0.40);
	glRotatef(70, 0.0, 1.0, 0.0);
	glScalef(0.23, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Fourth Right
	glTranslatef(-0.3, 0.5, 0.71);
	glScalef(0.12, 0.15, 0.10);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();               //Triangle
	glTranslatef(-0.425, 0.648, 0.615);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	glScalef(0.22, 0.2, 3.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();               //Side piece
	glTranslatef(-0.72, 0.5, 0.40);
	glRotatef(-70, 0.0, 1.0, 0.0);
	glScalef(0.23, 0.15, 0.01);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Fifth
	glTranslatef(0.0, 0.12, 0.75);
	glScalef(0.20, 0.15, 0.16);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Fifth Left
	glTranslatef(0.205, 0.35, 0.6);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.26, 0.38, 2.3);
	drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              //Fifth Right
	glTranslatef(-0.205, 0.35, 0.83);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.26, 0.38, 2.3);
	drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              //Left side piece
	glTranslatef(0.85, 1.0, 0.0);
	glRotatef(-6, 0.0, 0.0, 1.0);
	glScalef(0.03, 0.65, 0.3);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();              //Right side piece
	glTranslatef(-0.85, 1.0, 0.0);
	glRotatef(6, 0.0, 0.0, 1.0);
	glScalef(0.03, 0.65, 0.3);
	drawFilledCube();
	glPopMatrix();

	//Under part
	glPushMatrix();                //Front part  (kuku)
	glTranslatef(0.0, -0.08, 0.92);
	glRotatef(-20, 1.0, 0.0, 0.0);
	glScalef(0.13, 0.05, 0.01);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, -0.241, 0.725);
	glRotatef(10, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.15, 0.2);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();               //Left Triangle
	glTranslatef(0.105, -0.03, 0.6);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.3, 2.0);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();                //Right Triangle
	glTranslatef(-0.105, -0.03, 0.8);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.3, 2.0);
	drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();                //Left part of (kuku)
	glTranslatef(0.32, -0.1, 0.7);
	glRotatef(-20, 1.0, 0.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glScalef(0.25, 0.1, 0.1);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();                //Horizontal piece
	glTranslatef(0.7, 0.09, 0.4);
	glRotatef(20, 0.0, 0.0, 1.0);
	glRotatef(10, 1.0, 0.0, 0.0);
	glRotatef(35, 0.0, 1.0, 0.0);
	glScalef(0.35, 0.1, 0.08);
	drawFilledCube();
	glPopMatrix();


	glPushMatrix();                //Right part of (kuku)
	glTranslatef(-0.32, -0.1, 0.7);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(20, 1.0, 0.0, 0.0);
	glRotatef(60, 0.0, 0.0, 1.0);
	glScalef(0.25, 0.1, 0.1);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();                //Horizontal piece
	glTranslatef(-0.7, 0.09, 0.4);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glRotatef(10, 1.0, 0.0, 0.0);
	glRotatef(-35, 0.0, 1.0, 0.0);
	glScalef(0.35, 0.1, 0.08);
	drawFilledCube();
	glPopMatrix();
}

void drawLeftLeg()
{
	//1 (thigh)
	glPushMatrix();     //Top steep cube
	glTranslatef(0.7, -0.5, 0.7);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();    //Left small Triangle piece
	glTranslatef(0.9, -0.395, 0.6);
	glRotatef(140, 1.0, 0.0, 0.0);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();    //Right small Triangle piece
	glTranslatef(0.48, -0.375, 0.61);
	glRotatef(140, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	//2  (thigh)
	glPushMatrix();         //Thigh front rectangle piece
	glTranslatef(0.7, -1.12, 0.8);
	glScalef(0.2, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Thigh front left rectangle piece
	glTranslatef(1.0, -1.12, 0.68);
	glRotatef(53, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh left rectangle piece
	glTranslatef(1.11, -1.12, 0.15);
	glScalef(0.02, 0.7, 0.4);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh back left rectangle piece
	glTranslatef(0.91, -1.12, -0.3);
	glRotatef(75, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.6, 0.2);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Thigh front right rectangle piece
	glTranslatef(0.4, -1.12, 0.68);
	glRotatef(-53, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh left rectangle piece
	glTranslatef(0.31, -1.12, 0.15);
	glScalef(0.02, 0.7, 0.4);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh back left rectangle piece
	glTranslatef(0.51, -1.12, -0.3);
	glRotatef(-75, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.6, 0.2);
	drawFilledCube();
	glPopMatrix();

	//3  (thigh)
	glPushMatrix();     //Top steep cube
	glTranslatef(0.7, -1.77, 0.68);
	glRotatef(40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.2, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();    //Left small Triangle piece
	glTranslatef(0.9, -1.67, 0.72);
	glRotatef(-140, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();    //Right small Triangle piece
	glTranslatef(0.465, -1.705, 0.78);
	glRotatef(-140, 1.0, 0.0, 0.0);
	glRotatef(50, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	//4   (knee)
	glPushMatrix();      //Front piece 
	glTranslatef(0.7, -2.15, 0.50);
	glRotatef(15, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();      //Left front piece 
	glTranslatef(0.93, -2.13, 0.42);
	glRotatef(15, 1.0, 0.0, 0.0);
	glRotatef(-28, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.09);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();      //Left piece 
	glTranslatef(1.02, -2.08, 0.12);
	glRotatef(8, 1.0, 0.0, 0.0);
	glRotatef(-10, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.3);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();      //Right front piece 
	glTranslatef(0.463, -2.13, 0.42);
	glRotatef(15, 1.0, 0.0, 0.0);
	glRotatef(28, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.09);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();      //Right piece 
	glTranslatef(0.37, -2.08, 0.12);
	glRotatef(8, 1.0, 0.0, 0.0);
	glRotatef(10, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.3);
	drawFilledCube();
	glPopMatrix();


	//5 (shin)
	glPushMatrix();     //Top steep cube
	glTranslatef(0.7, -2.45, 0.56);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();

	//6  (shin)
	glPushMatrix();         //Shin front rectangle piece
	glTranslatef(0.7, -3.07, 0.66);
	glScalef(0.2, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Shin front left rectangle piece
	glTranslatef(0.93, -3.07, 0.53);
	glRotatef(58, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Shin left rectangle piece
	glTranslatef(1.02, -3.17, 0.1);
	glScalef(0.02, 0.6, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();          //Left behind rocket
	glTranslatef(0.9, -3.0, -0.42);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.15, 0.15, 0.7, 30, 30);
	glPopMatrix();

	glPushMatrix();         //Shin front right rectangle piece
	glTranslatef(0.468, -3.07, 0.53);
	glRotatef(-58, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Shin right rectangle piece
	glTranslatef(0.38, -3.17, 0.1);
	glScalef(0.02, 0.6, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();          //Right behind rocket
	glTranslatef(0.5, -3.0, -0.42);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.15, 0.15, 0.7, 30, 30);
	glPopMatrix();

	//7  (ankle)
	glPushMatrix();     //Top steep cube
	glTranslatef(0.7, -3.67, 0.742);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.12, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();     //Second steep cube
	glTranslatef(0.7, -3.82, 0.94);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.14, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Second steep left triangle
	glTranslatef(0.799, -3.79, 0.81);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.27, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();     //Second steep right triangle
	glTranslatef(0.6, -3.749, 0.83);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.27, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();     //Third front cube
	glTranslatef(0.7, -3.96, 1.06);
	glScalef(0.1, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third front bottom steep cube
	glTranslatef(0.7, -4.09, 0.86);
	glRotatef(-10, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.02, 0.21);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Bottom cube
	glTranslatef(0.69, -4.115, 0.35);
	glScalef(0.215, 0.02, 0.3);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();     //Third front left cube
	glTranslatef(0.852, -3.96, 0.885);
	glRotatef(70, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third left side cube
	glTranslatef(0.92, -3.99, 0.4);
	glScalef(0.03, 0.13, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third left back cube
	glTranslatef(0.955, -3.94, 0.06);
	glScalef(0.04, 0.172, 0.28);
	drawFilledCube();
	glPopMatrix();


	glPushMatrix();     //Third front right cube
	glTranslatef(0.54, -3.96, 0.885);
	glRotatef(-67, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third right side cube
	glTranslatef(0.46, -3.99, 0.4);
	glScalef(0.03, 0.13, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third right back cube
	glTranslatef(0.43, -3.94, 0.06);
	glScalef(0.04, 0.172, 0.28);
	drawFilledCube();
	glPopMatrix();

}

void drawRightLeg()
{
	//1  (thigh)
	glPushMatrix();     //Top steep cube
	glTranslatef(-0.7, -0.5, 0.7);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();    //Left small Triangle piece
	glTranslatef(-0.5, -0.395, 0.6);
	glRotatef(140, 1.0, 0.0, 0.0);
	glRotatef(-30, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();    //Right small Triangle piece
	glTranslatef(-0.92, -0.375, 0.61);
	glRotatef(140, 1.0, 0.0, 0.0);
	glRotatef(30, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	//2  (thigh)
	glPushMatrix();         //Thigh front rectangle piece
	glTranslatef(-0.7, -1.12, 0.8);
	glScalef(0.2, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Thigh front left rectangle piece
	glTranslatef(-0.4, -1.12, 0.68);
	glRotatef(53, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh left rectangle piece
	glTranslatef(-0.31, -1.12, 0.15);
	glScalef(0.02, 0.7, 0.4);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh back left rectangle piece
	glTranslatef(-0.51, -1.12, -0.3);
	glRotatef(75, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.6, 0.2);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Thigh front right rectangle piece
	glTranslatef(-1.0, -1.12, 0.68);
	glRotatef(-53, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh left rectangle piece
	glTranslatef(-1.11, -1.12, 0.15);
	glScalef(0.02, 0.7, 0.4);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Thigh back left rectangle piece
	glTranslatef(-0.91, -1.12, -0.3);
	glRotatef(-75, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.6, 0.2);
	drawFilledCube();
	glPopMatrix();

	//3  (thigh)
	glPushMatrix();     //Top steep cube
	glTranslatef(-0.7, -1.77, 0.68);
	glRotatef(40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.2, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();    //Left small Triangle piece
	glTranslatef(-0.5, -1.67, 0.72);
	glRotatef(-140, 1.0, 0.0, 0.0);
	glRotatef(-40, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();    //Right small Triangle piece
	glTranslatef(-0.935, -1.705, 0.78);
	glRotatef(-140, 1.0, 0.0, 0.0);
	glRotatef(50, 0.0, 1.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.3, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	//4  (knee)
	glPushMatrix();      //Front piece 
	glTranslatef(-0.7, -2.15, 0.50);
	glRotatef(15, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();      //Left front piece 
	glTranslatef(-0.463, -2.13, 0.42);
	glRotatef(15, 1.0, 0.0, 0.0);
	glRotatef(-28, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.09);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();      //Left piece 
	glTranslatef(-0.37, -2.08, 0.12);
	glRotatef(8, 1.0, 0.0, 0.0);
	glRotatef(-10, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.3);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();      //Right front piece 
	glTranslatef(-0.93, -2.13, 0.42);
	glRotatef(15, 1.0, 0.0, 0.0);
	glRotatef(28, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.09);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();      //Right piece 
	glTranslatef(-1.02, -2.08, 0.12);
	glRotatef(8, 1.0, 0.0, 0.0);
	glRotatef(10, 0.0, 1.0, 0.0);
	glScalef(0.02, 0.15, 0.3);
	drawFilledCube();
	glPopMatrix();

	//5   (shin)
	glPushMatrix();     //Top steep cube
	glTranslatef(-0.7, -2.45, 0.56);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.15, 0.02);
	drawFilledCube();
	glPopMatrix();

	//6  (shin)
	glPushMatrix();         //Shin front rectangle piece
	glTranslatef(-0.7, -3.07, 0.66);
	glScalef(0.2, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();         //Shin front left rectangle piece
	glTranslatef(-0.47, -3.07, 0.53);
	glRotatef(58, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Shin left rectangle piece
	glTranslatef(-0.37, -3.17, 0.1);
	glScalef(0.02, 0.6, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();          //Left behind rocket
	glTranslatef(-0.5, -3.0, -0.42);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.15, 0.15, 0.7, 30, 30);
	glPopMatrix();

	glPushMatrix();         //Shin front right rectangle piece
	glTranslatef(-0.928, -3.07, 0.53);
	glRotatef(-58, 0.0, 1.0, 0.0);
	glScalef(0.16, 0.5, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();         //Shin right rectangle piece
	glTranslatef(-1.01, -3.17, 0.1);
	glScalef(0.02, 0.6, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();          //Right behind rocket
	glTranslatef(-0.9, -3.0, -0.42);
	glRotatef(90, 1.0, 0.0, 0.0);
	drawCylinder(0.15, 0.15, 0.7, 30, 30);
	glPopMatrix();

	//7  (ankle)
	glPushMatrix();     //Top steep cube
	glTranslatef(-0.7, -3.67, 0.742);
	glRotatef(-40, 1.0, 0.0, 0.0);
	glScalef(0.2, 0.12, 0.02);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();     //Second steep cube
	glTranslatef(-0.7, -3.82, 0.94);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.14, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Second steep left triangle
	glTranslatef(-0.6, -3.79, 0.81);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.27, 0.4);
	drawFilledTriangle();
	glPopMatrix();
	glPushMatrix();     //Second steep right triangle
	glTranslatef(-0.8, -3.749, 0.83);
	glRotatef(-65, 1.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glRotatef(180, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.27, 0.4);
	drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();     //Third front cube
	glTranslatef(-0.7, -3.96, 1.06);
	glScalef(0.1, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third front bottom steep cube
	glTranslatef(-0.7, -4.09, 0.86);
	glRotatef(-10, 1.0, 0.0, 0.0);
	glScalef(0.1, 0.02, 0.21);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Bottom cube
	glTranslatef(-0.69, -4.115, 0.35);
	glScalef(0.215, 0.02, 0.3);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();     //Third front left cube
	glTranslatef(-0.54, -3.96, 0.885);
	glRotatef(67, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third left side cube
	glTranslatef(-0.46, -3.99, 0.4);
	glScalef(0.03, 0.13, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third left back cube
	glTranslatef(-0.43, -3.94, 0.06);
	glScalef(0.04, 0.172, 0.28);
	drawFilledCube();
	glPopMatrix();

	glPushMatrix();     //Third front right cube
	glTranslatef(-0.851, -3.96, 0.885);
	glRotatef(-70, 0.0, 1.0, 0.0);
	glScalef(0.2, 0.1, 0.02);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third right side cube
	glTranslatef(-0.92, -3.99, 0.4);
	glScalef(0.03, 0.13, 0.3);
	drawFilledCube();
	glPopMatrix();
	glPushMatrix();     //Third right back cube
	glTranslatef(-0.955, -3.94, 0.06);
	glScalef(0.04, 0.172, 0.28);
	drawFilledCube();
	glPopMatrix();
}

void jaegerRobot()
{
	//Body

	glRotatef(90, 1.0, 0.0, 0.0);

	//Inner part of robot
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/darkMetal_inner.bmp");
		drawInnerRobotPart();
		endTexture();
	glPopMatrix();

	//Backbone part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/chrome_backbone.bmp");
		drawBackBone();
		endTexture();
	glPopMatrix();

	// Armor part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/blue_armor.bmp");
		drawFrontArmour();
		endTexture();
	glPopMatrix();

	//Front Belly
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/blue_armor.bmp");
		drawFrontBelly();
		endTexture();
	glPopMatrix();

	//Leg part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/blue_armor.bmp");
		drawLeftLeg();
		drawRightLeg();
		endTexture();
	glPopMatrix();

	//Testing Area

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0, 0.0, zoomLevel);

	glRotatef(-90, 1.0, 0.0, 0.0);                    //Must turn it 90 degree so can move using mouse 

	glRotatef(xRotated, 1.0, 0.0, 0.0);

	glRotatef(yRotated, 0.0, 1.0, 0.0);

	glRotatef(zRotated, 0.0, 0.0, 1.0);

	jaegerRobot();
}
//--------------------------------------------------------------------

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, 1, 0.1, 60.0);

	//Enable texture


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------


//Tips
//Object
//Move objects in -ve z direction, or move camera in +ve z direction

//Color

//Texture




//Cautions
// 1. Rememeber to remove the color in drawFilledCube
// 2. How to rotate automatically in glPushMatrix for ironman reactor