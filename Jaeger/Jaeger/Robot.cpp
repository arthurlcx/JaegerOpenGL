#include <Windows.h>
#include <Windowsx.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <string>

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
float electricEffectMovement = 1.0f;

float xPosition = 0.0f, yPosition = 0.0f, zPosition = 0.05f;

float zoomLevel = -7.0f;


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
bool dayTime = true;


//Ours
float LeftHandPalmAngle = 0.0, LeftHandForearmAngle = 0.0, LeftHandWholeArmAngle = 0.0;
float LeftHandWholeArmAnglez = 0.0, LeftHandForearmAnglez = 0.0;
float RightHandPalmAngle = 0.0, RightHandForearmAngle = 0.0, RightHandWholeArmAngle = 0.0;
float RightHandWholeArmAnglez = 0.0, RightHandForearmAnglez = 0.0;
float leftWristRotatey = 0.0;
bool leftArmRotateDS = false, leftWristRotateyDS = false, leftWristRotatezDS = false, leftHandPalmRotatez = false;
bool drawDSCircle = false;

float LeftLegThighAngle = 0.0, LeftLegKneeShinAngle = 0.0, LeftLegAnkleAngle = 0.0;
float RightLegThighAngle = 0.0, RightLegKneeShinAngle = 0.0, RightLegAnkleAngle = 0.0;

float robotMoveFront = 0.0, robotMoveLeft = 0.0, robotMoveRight = 0.0;
float fingerAngle1 = 0.0, fingerAngle2 = 0.0, fingerAngle3 = 0.0, fingerAngle4 = 0.0;

int weaponTypeL = 0, weaponTypeR = 0, drillRotate = 0.0, gunBarrelRotate = 0.0;
int backType = 0;
float wingLeftTranslateDegree = 0.0, wingRightTranslateDegree = 0.0;
float wingLeftRocketTranslateDegree = 0.0, wingRightRocketTranslateDegree = 0.0;
bool robotExhaustJet = false;
bool drawThorHammer = false;
bool handShield = false;
bool reactorFireball = false;
float reactorFireballTranslate = 0.0;
int fireballCount = 0;

bool LeftLegUpFront = true, LeftLegDownFront = false;
bool RightLegUpFront = false, RightLegDownFront = false;
bool LeftLegUpBack = true, LeftLegDownBack = false;
bool RightLegUpBack = false, RightLegDownBack = false;
float robotUpperBodyTranslate = 0.0;
float robotLeftLegMoveWithBody = 0.0, robotRightLegMoveWithBody;

//Texture variable declaration
std::string armorTextureArray[] = { "textureImage/blue_armor.bmp", "textureImage/black_armor.bmp", "textureImage/camo_armor.bmp", "textureImage/ironMan_armor.bmp" };
std::string innerTextureArray[] = { "textureImage/darkMetal_inner.bmp", "textureImage/whiteMetal_inner.bmp", "textureImage/greenMetal_inner.bmp", "textureImage/ironMan_inner.bmp" };
std::string wingTextureArray[] = { "textureImage/wing1.bmp", "textureImage/wing2.bmp", "textureImage/wing3.bmp", "textureImage/ironMan_wing.bmp" };
std::string wingSecTextureArray[] = { "textureImage/wingSec1.bmp", "textureImage/wingSec2.bmp", "textureImage/wingSec3.bmp", "textureImage/ironMan_wingSec2.bmp" };
std::string reactorTextureArray[] = { "textureImage/reactor.bmp", "textureImage/ironMan_reactor.bmp" };

std::string shield1Array[] = { "textureImage/shield1.bmp", "textureImage/avengerShield1.bmp" };
std::string shield2Array[] = { "textureImage/shield2.bmp", "textureImage/avengerShield2.bmp" };
std::string shield3Array[] = { "textureImage/shield3.bmp", "textureImage/avengerShield3.bmp" };
std::string shield4Array[] = { "textureImage/shield4.bmp", "textureImage/avengerShield4.bmp" };

std::string environmentTopTextureArray[] = { "environmentTexture/majestic_up.bmp", "environmentTexture/criminal-impact_up.bmp", "environmentTexture/midnight-silence_up.bmp", "environmentTexture/darkskies_up.bmp" };
std::string environmentBottomTextureArray[] = { "environmentTexture/majestic_dn.bmp" , "environmentTexture/criminal-impact_dn.bmp", "environmentTexture/midnight-silence_dn.bmp", "environmentTexture/darkskies_dn.bmp" };
std::string environmentLeftTextureArray[] = { "environmentTexture/majestic_rt.bmp" , "environmentTexture/criminal-impact_rt.bmp", "environmentTexture/midnight-silence_rt.bmp", "environmentTexture/darkskies_rt.bmp" };
std::string environmentRightTextureArray[] = { "environmentTexture/majestic_lf.bmp" , "environmentTexture/criminal-impact_lf.bmp", "environmentTexture/midnight-silence_lf.bmp", "environmentTexture/darkskies_lf.bmp" };
std::string environmentFrontTextureArray[] = { "environmentTexture/majestic_bk.bmp" , "environmentTexture/criminal-impact_bk.bmp", "environmentTexture/midnight-silence_bk.bmp", "environmentTexture/darkskies_bk.bmp" };
std::string environmentBackTextureArray[] = { "environmentTexture/majestic_ft.bmp" , "environmentTexture/criminal-impact_ft.bmp", "environmentTexture/midnight-silence_ft.bmp", "environmentTexture/darkskies_ft.bmp" };
int textureSetIndex = 0;
int textureReactorIndex = 0;
int textureEnvironmentIndex = 0;

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

//Lighting declaration - ambient, diffuse, specular
GLfloat light_ambient[] = { 1.0, 1.0 ,1.0, 1.0 };
GLfloat light_close[] = { 0.0, 0.0 ,0.0, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat light_position[] = { 0.2, 0.3, 0.2, 1.0 };

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
		else if (wParam == 'P')               //LEFT WRIST MOVE UP
		{
			if (LeftHandPalmAngle <= 0 && LeftHandPalmAngle > -20)
				LeftHandPalmAngle -= 10;
		}
		else if (wParam == 'O')               //LEFT FOREARM MOVE UP
		{
			if (LeftHandForearmAngle <= 0 && LeftHandForearmAngle > -100)
				LeftHandForearmAngle -= 5;
		}
		else if (wParam == 'I')               //LEFT WHOLE ARM MOVE UP
		{
			if (LeftHandWholeArmAngle <= 10 && LeftHandWholeArmAngle > -100)
				LeftHandWholeArmAngle -= 5;
		}
		else if (wParam == 'U')              //LEFT WRIST MOVE DOWN
		{
			if (LeftHandPalmAngle < 0 && LeftHandPalmAngle >= -20)
				LeftHandPalmAngle += 10;
		}
		else if (wParam == 'Y')               //LEFT FOREARM MOVE DOWN
		{
			if (LeftHandForearmAngle < 0 && LeftHandForearmAngle >= -100)
				LeftHandForearmAngle += 5;
		}
		else if (wParam == 'T')               //LEFT WHOLE ARM MOVE DOWN
		{
			if (LeftHandWholeArmAngle < 10 && LeftHandWholeArmAngle >= -100)
				LeftHandWholeArmAngle += 5;
		}
		else if (wParam == 'L')                //RIGHT WRIST MOVE UP
		{
			if (RightHandPalmAngle <= 0 && RightHandPalmAngle > -20)
				RightHandPalmAngle -= 10;
		}
		else if (wParam == 'K')               //RIGHT FOREARM MOVE UP
		{
			if (RightHandForearmAngle <= 0 && RightHandForearmAngle > -100)
				RightHandForearmAngle -= 5;
		}
		else if (wParam == 'J')               //RIGHT WHOLE ARM MOVE UP
		{
			if (RightHandWholeArmAngle <= 10 && RightHandWholeArmAngle > -100)
				RightHandWholeArmAngle -= 5;
		}
		else if (wParam == 'H')               //RIGHT WRIST MOVE DOWN
		{
			if (RightHandPalmAngle < 0 && RightHandPalmAngle >= -20)
				RightHandPalmAngle += 10;
		}
		else if (wParam == 'G')                 //RIGHT FOREARM MOVE DOWN
		{
			if (RightHandForearmAngle < 0 && RightHandForearmAngle >= -100)
				RightHandForearmAngle += 5;
		}
		else if (wParam == 'F')                  //RIGHT WHOLE ARM MOVE DOWN
		{
			if (RightHandWholeArmAngle < 10 && RightHandWholeArmAngle >= -100)
				RightHandWholeArmAngle += 5;
		}
		else if (wParam == 'R')
		{
			handShield = true;
			//Left side
			if (LeftHandWholeArmAngle >= -75 && LeftHandWholeArmAngle <= 0)
				LeftHandWholeArmAngle -= 5;

			if(LeftHandForearmAngle >= -105 && LeftHandForearmAngle <= 0)
				LeftHandForearmAngle -= 5;

			if(LeftHandWholeArmAnglez >= -30 && LeftHandWholeArmAnglez <=0)
				LeftHandWholeArmAnglez -= 5;

			if(LeftHandForearmAnglez <= 65 && LeftHandForearmAnglez >=0)
				LeftHandForearmAnglez += 5;


			//Right side
			if (RightHandWholeArmAngle >= -75 && RightHandWholeArmAngle <= 0)
				RightHandWholeArmAngle -= 5;

			if (RightHandForearmAngle >= -105 && RightHandForearmAngle <= 0)
				RightHandForearmAngle -= 5;

			if (RightHandWholeArmAnglez <= 30 && RightHandWholeArmAnglez >= 0)
				RightHandWholeArmAnglez += 5;

			if (RightHandForearmAnglez >= -65 && RightHandForearmAnglez <= 0)
				RightHandForearmAnglez -= 5;
		}
		else if (wParam == 'D')
		{
			//Left side
			if (LeftHandWholeArmAngle >= -95 && LeftHandWholeArmAngle < 0)
				LeftHandWholeArmAngle += 5;

			if (LeftHandForearmAngle >= -115 && LeftHandForearmAngle < 0)
				LeftHandForearmAngle += 5;

			if (LeftHandWholeArmAnglez >= -40 && LeftHandWholeArmAnglez < 0)
				LeftHandWholeArmAnglez += 5;

			if (LeftHandForearmAnglez <= 75 && LeftHandForearmAnglez > 0)
				LeftHandForearmAnglez -= 5;

			//Right side
			if (RightHandWholeArmAngle >= -85 && RightHandWholeArmAngle < 0)
				RightHandWholeArmAngle += 5;

			if (RightHandForearmAngle >= -115 && RightHandForearmAngle < 0)
				RightHandForearmAngle += 5;

			if (RightHandWholeArmAnglez <= 40 && RightHandWholeArmAnglez > 0)
				RightHandWholeArmAnglez -= 5;

			if (RightHandForearmAnglez >= -75 && RightHandForearmAnglez < 0)
				RightHandForearmAnglez += 5;

			if (RightHandForearmAnglez >= 0)
				handShield = false;
		}
		else if (wParam == VK_UP)               //MOVE FORWARD
		{
			if (LeftLegUpFront == true)
			{
				if (LeftLegThighAngle <= 0 && LeftLegThighAngle > -20)
				{
					LeftLegThighAngle -= 0.5;
					LeftLegKneeShinAngle += 0.5;

					if (LeftLegThighAngle > -10) {
						robotUpperBodyTranslate += 0.02;
						robotLeftLegMoveWithBody += 0.04;
					}

					RightLegThighAngle += 0.5;

					if (LeftLegThighAngle == -20)
					{
						LeftLegDownFront = true;
						LeftLegUpFront = false;
					}
				}
			}
			else if (LeftLegDownFront == true)
			{
				if (LeftLegThighAngle < 0)
				{
					LeftLegThighAngle += 0.5;
					LeftLegKneeShinAngle -= 0.5;

					if (LeftLegThighAngle < -20)
					{
						robotUpperBodyTranslate += 0.02;
						robotLeftLegMoveWithBody += 0.05;
						robotRightLegMoveWithBody += 0.05;
					}

					RightLegThighAngle -= 0.5;
				}
				if (LeftLegThighAngle == 0)
				{
					RightLegUpFront = true;
					LeftLegDownFront = false;
				}
			}
			else if (RightLegUpFront == true)
			{
				if (RightLegThighAngle <= 0 && RightLegThighAngle > -20)
				{
					RightLegThighAngle -= 0.5;
					RightLegKneeShinAngle += 0.5;

					if (RightLegThighAngle > -10) {
						robotUpperBodyTranslate += 0.02;
						robotRightLegMoveWithBody += 0.04;
					}

					LeftLegThighAngle += 0.5;

					if (RightLegThighAngle == -20)
					{
						RightLegDownFront = true;
						RightLegUpFront = false;
					}
				}
			}
			else if (RightLegDownFront == true)
			{
				if (RightLegThighAngle < 0)
				{
					RightLegThighAngle += 0.5;
					RightLegKneeShinAngle -= 0.5;

					if (RightLegThighAngle < -20)
					{
						robotUpperBodyTranslate += 0.02;
						robotLeftLegMoveWithBody += 0.05;
						robotRightLegMoveWithBody += 0.05;
					}

					LeftLegThighAngle -= 0.5;
				}
				if (LeftLegThighAngle == 0)
				{
					LeftLegUpFront = true;
					RightLegDownFront = false;
				}
			}
		}
		else if (wParam == VK_DOWN)               //MOVE FORWARD
		{
			if (LeftLegUpBack == true)
			{
				if (LeftLegThighAngle >= 0 && LeftLegThighAngle < 20)
				{
					LeftLegThighAngle += 0.5;

					if (LeftLegThighAngle < 10)
					{
						robotUpperBodyTranslate -= 0.02;
						robotLeftLegMoveWithBody -= 0.04;
					}

					RightLegThighAngle -= 0.5;
					RightLegKneeShinAngle += 0.5;

					if (LeftLegThighAngle == 20)
					{
						LeftLegDownBack = true;
						LeftLegUpBack = false;
					}
				}
			}
			else if (LeftLegDownBack == true)
			{
				if (LeftLegThighAngle > 0)
				{
					LeftLegThighAngle -= 0.5;


					if (LeftLegThighAngle > 10)
					{
						robotUpperBodyTranslate -= 0.03;
						robotLeftLegMoveWithBody -= 0.01;
						robotRightLegMoveWithBody -= 0.053;
					}

					RightLegThighAngle += 0.5;
					RightLegKneeShinAngle -= 0.5;
				}
				if (LeftLegThighAngle == 0)
				{
					RightLegUpBack = true;
					LeftLegDownBack = false;
				}
			}
			else if (RightLegUpBack == true)
			{
				if (RightLegThighAngle >= 0 && RightLegThighAngle < 20)
				{
					RightLegThighAngle += 0.5;


					if (RightLegThighAngle > 10) {
						robotUpperBodyTranslate -= 0.04;
						robotRightLegMoveWithBody -= 0.02;
						robotLeftLegMoveWithBody -= 0.01;
					}

					LeftLegThighAngle -= 0.5;
					LeftLegKneeShinAngle += 0.5;

					if (RightLegThighAngle == 20)
					{
						RightLegDownBack = true;
						RightLegUpBack = false;
					}
				}
			}
			else if (RightLegDownBack == true)
			{
				if (RightLegThighAngle > 0)
				{
					RightLegThighAngle -= 0.5;


					if (RightLegThighAngle > 10)
					{
						robotUpperBodyTranslate -= 0.02;
						robotLeftLegMoveWithBody -= 0.05;
						robotRightLegMoveWithBody -= 0.04;
					}

					LeftLegThighAngle += 0.5;
					LeftLegKneeShinAngle -= 0.5;
				}
				if (LeftLegThighAngle == 0)
				{
					LeftLegUpBack = true;
					RightLegDownBack = false;
				}
			}
		}
		else if (wParam == VK_LEFT)               //Robot move left
		{
			robotMoveLeft += 5.0;
		}
		else if (wParam == VK_RIGHT)               //Robot move right
		{
			robotMoveRight -= 5.0;
		}
		else if (wParam == 'W')               //ANKLE MOVE UP
		{
			if (LeftLegAnkleAngle <= 0 && LeftLegAnkleAngle > -10)
				LeftLegAnkleAngle -= 5;
			if (RightLegAnkleAngle <= 0 && RightLegAnkleAngle > -10)
				RightLegAnkleAngle -= 5;
		}
		else if (wParam == 'E')               //ANKLE MOVE DOWN
		{
			if (LeftLegAnkleAngle < 0 && LeftLegAnkleAngle >= -10)
				LeftLegAnkleAngle += 5;
			if (RightLegAnkleAngle < 0 && RightLegAnkleAngle >= -10)
				RightLegAnkleAngle += 5;
		}
		else if (wParam == 'M')               //Finger left close
		{
			if (fingerAngle1 <= 0 && fingerAngle1 > -50)
			{
				fingerAngle1 -= 2.0;
				fingerAngle2 -= 3.0;
			}
		}
		else if (wParam == 'N')               //Finger right close
		{
			if (fingerAngle3 >= 0 && fingerAngle3 < 50)
			{
				fingerAngle3 += 2.0;
				fingerAngle4 += 3.0;

				if (fingerAngle3 >= 50)
				{
					drawThorHammer = true;
				}
			}
		}
		else if (wParam == 'B')               //Finger left open
		{
			if (fingerAngle1 < 0 && fingerAngle1 >= -55)
			{
				fingerAngle1 += 2.0;
				fingerAngle2 += 3.0;
			}

		}
		else if (wParam == 'V')               //Finger right open
		{
			if (fingerAngle3 > 0 && fingerAngle3 <= 55)
			{
				fingerAngle3 -= 2.0;
				fingerAngle4 -= 3.0;

				if (fingerAngle3 < 50)
					drawThorHammer = false;
			}
		}
		else if (wParam == 'Z')                //LEFT HAND SWITCH WEAPONS
		{
			if (weaponTypeL >= 0 & weaponTypeL < 4)
				weaponTypeL += 1;
			else
				weaponTypeL = 0;
		}
		else if (wParam == 'X')                //RIGHT HAND SWITCH WEAPONS
		{
			if (weaponTypeR >= 0 & weaponTypeR < 4)
				weaponTypeR += 1;
			else
				weaponTypeR = 0;
		}
		else if (wParam == VK_CAPITAL)
		{
			if (backType < 2)
				backType += 1;
			else
				backType = 0;
		}
		else if (wParam == 'A')
		{
			if (robotExhaustJet == true)
			{
				if (wingLeftTranslateDegree >= -0.5 && wingLeftTranslateDegree < 2.7)
					wingLeftTranslateDegree += 0.3;
				if (wingLeftRocketTranslateDegree >= -0.5 && wingLeftRocketTranslateDegree < 0.5)
					wingLeftRocketTranslateDegree += 0.05;

				if (wingRightTranslateDegree <= 0.5 && wingRightTranslateDegree > -2.7)
					wingRightTranslateDegree -= 0.3;
				if (wingRightRocketTranslateDegree <= 0.5 && wingRightRocketTranslateDegree > -0.5)
					wingRightRocketTranslateDegree -= 0.05;
			}
		}
		else if (wParam == 'S')
		{
			if (robotExhaustJet == true)
			{
				if (wingLeftTranslateDegree > 0 && wingLeftTranslateDegree <= 3.5)
					wingLeftTranslateDegree -= 0.3;
				if (wingLeftRocketTranslateDegree > 0 && wingLeftRocketTranslateDegree <= 0.6)
					wingLeftRocketTranslateDegree -= 0.05;

				if (wingRightTranslateDegree < 0 && wingRightTranslateDegree >= -3.5)
					wingRightTranslateDegree += 0.3;
				if (wingRightRocketTranslateDegree < 0 && wingRightRocketTranslateDegree > -0.6)
					wingRightRocketTranslateDegree += 0.05;
			}
		}
		else if (wParam == VK_TAB)
		{
			if (fireballCount == 0)
			{
				reactorFireball = true;
				fireballCount += 1;
			}
			else if(fireballCount == 1)
			{
				reactorFireball = false;
				fireballCount -= 1;
			}
		}
		else if (wParam == VK_OEM_4)               //draw doctor strange circle
		{
			if (LeftHandWholeArmAngle <= 0 && LeftHandWholeArmAngle > -95)
			{
				LeftHandWholeArmAngle -= 5;
				
				if (LeftHandWholeArmAngle == -90)
					leftArmRotateDS = true;
			}			
			else if (leftArmRotateDS == true && leftWristRotatey > -90)
			{
				leftWristRotatey -= 15;

				if (leftWristRotatey == -90)
					leftWristRotateyDS = true;
			}
			else if (leftWristRotateyDS == true && LeftHandPalmAngle < 30)
			{
				LeftHandPalmAngle += 5;

				if (LeftHandPalmAngle == 30)
					leftHandPalmRotatez = true;
			}
			else if (leftHandPalmRotatez == true)
			{
				drawDSCircle = true;
			}
		}
		else if (wParam == VK_OEM_6)               //draw doctor strange circle
		{
			drawDSCircle = false;

			if (drawDSCircle == false && LeftHandPalmAngle >0)
			{
				LeftHandPalmAngle -= 5;

				if (LeftHandPalmAngle == 0)
					leftHandPalmRotatez = true;
			}
			else if (leftHandPalmRotatez == true && leftWristRotatey < 0)
			{
				leftWristRotatey += 15;

				if (leftWristRotatey == 0)
					leftWristRotateyDS = true;
			}
			else if (leftWristRotateyDS == true && LeftHandWholeArmAngle < 0)
			{
				LeftHandWholeArmAngle += 5;
			}		
		}
		else if (wParam == VK_SPACE)
		{
			LeftHandPalmAngle = 0.0;
			LeftHandForearmAngle = 0.0;
			LeftHandWholeArmAngle = 0.0;
			LeftHandWholeArmAnglez = 0.0;
			LeftHandForearmAnglez = 0.0;
			RightHandPalmAngle = 0.0;
			RightHandForearmAngle = 0.0;
			RightHandWholeArmAngle = 0.0;
			RightHandWholeArmAnglez = 0.0;
			RightHandForearmAnglez = 0.0;

			LeftLegThighAngle = 0.0;
			LeftLegKneeShinAngle = 0.0;
			LeftLegAnkleAngle = 0.0;
			RightLegThighAngle = 0.0;
			RightLegKneeShinAngle = 0.0;
			RightLegAnkleAngle = 0.0;

			fingerAngle1 = 0.0;
			fingerAngle2 = 0.0;
			fingerAngle3 = 0.0;
			fingerAngle4 = 0.0;

			weaponTypeL = 0;
			weaponTypeR = 0;
			drillRotate = 0.5;
			drawThorHammer = false;
			handShield = false;

			textureSetIndex = 0;
			textureReactorIndex = 0;
			textureEnvironmentIndex = 0;
		}
		else if (wParam == 0x31)		// 1 key on/off ambient light
			ambientOn = !ambientOn;
		else if (wParam == 0x32)		// 2 key on/off diffuse light
			diffuseOn = !diffuseOn;
		else if (wParam == 0x33)		// 3 key on/off specular light
			specularOn = !specularOn;
		else if (wParam == 0x30)		// 9 key toggle day/night
			dayTime = !dayTime;
		else if (wParam == VK_NUMPAD1) // numpad 1 for texture set 1
			textureSetIndex = 0;
		else if (wParam == VK_NUMPAD2) // numpad 2 for texture set 2
			textureSetIndex = 1;
		else if (wParam == VK_NUMPAD3) // numpad 3 for texture set 3
			textureSetIndex = 2;
		else if (wParam == VK_NUMPAD4) // numpad 4 for environemnt texture set 1
			textureEnvironmentIndex = 0;
		else if (wParam == VK_NUMPAD5) // numpad 5 for environemnt texture set 2
			textureEnvironmentIndex = 1;
		else if (wParam == VK_NUMPAD6) // numpad 6 for environemnt texture set 3
			textureEnvironmentIndex = 2;
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

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
	gluQuadricTexture(sphere, GLU_TRUE);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluSphere(sphere, radius, slices, stacks);
	gluDeleteQuadric(sphere);
}

void drawSphere1(float inradius, float outradius, float slices, float loops, float startAngle, float sweepAngle)
{
	GLUquadricObj *sphere = NULL;
	sphere = gluNewQuadric();
	gluQuadricTexture(sphere, GLU_TRUE);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluPartialDisk(sphere, inradius, outradius, slices, loops, startAngle, sweepAngle);
	gluDeleteQuadric(sphere);

}

void drawCylinder(float base, float top, float height, float slices, float stacks)
{
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluQuadricTexture(cylinder, GLU_TRUE);
	gluCylinder(cylinder, base, top, height, slices, stacks);    //gluCylinder(GLUquadric obj *, baseRadius,topRadius, height,slices, stacks);
	gluDisk(cylinder, 0.0f, base, slices, stacks);
	gluDeleteQuadric(cylinder);
}

void drawFilledCubeT()
{
	glBegin(GL_QUADS);
	{
		// Top Face
		glColor3f(1.0, 0.0, 0.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);

		// Left Face
		glColor3f(0.0, 1.0, 0.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Back Face
		glColor3f(0.0, 0.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);

		// Right Face
		glColor3f(1.0, 0.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, -1.0f, 1.0f);

		// Bottom Face
		glColor3f(1.0, 1.0, 0.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);

		// Front Face
		glColor3f(0.0, 1.0, 1.0);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, 1.0f);
	}
	glEnd();
}

void drawFilledCube()
{

	glBegin(GL_QUADS);
	{
		glNormal3f(0.0, 0.0, 1.0);
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

	glBegin(GL_TRIANGLES);
		glTexCoord2f(0, 1);  glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(0, 0);  glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1, 0);  glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_TRIANGLES);
		glTexCoord2f(0, 1);  glVertex3f(0.0, 1.0, -0.1);
		glTexCoord2f(0, 0);  glVertex3f(0.0, 0.0, -0.1);
		glTexCoord2f(1, 0);  glVertex3f(1.0, 0.0, -0.1);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);  glVertex3f(0.0, 1.0, -0.1);
		glTexCoord2f(0, 0);  glVertex3f(0.0, 0.0, -0.1);
		glTexCoord2f(1, 0);  glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1, 1);  glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);  glVertex3f(1.0, 0.0, -0.1);
		glTexCoord2f(0, 0);  glVertex3f(0.0, 0.0, -0.1);
		glTexCoord2f(1, 0);  glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1, 0);  glVertex3f(1.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_QUADS);
		glTexCoord2f(0, 1);  glVertex3f(0.0, 1.0, 0.0);
		glTexCoord2f(0, 0);  glVertex3f(1.0, 0.0, 0.0);
		glTexCoord2f(1, 0);  glVertex3f(1.0, 0.0, -0.1);
		glTexCoord2f(1, 1);  glVertex3f(0.0, 1.0, -0.1);
	glEnd();
}

void drawCircle(float radius)
{
	float xc = 0.0, yc = 0.0, xc2 = 0.0, yc2 = 0.0;
	float angleC = 0.0;

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(xc, yc);

	for (angleC - 0.1; angleC <= 360; angleC += 0.2)
	{
		xc2 = xc + sin(angleC) * radius;
		yc2 = yc + cos(angleC) * radius;
		glVertex2f(xc2, yc2);
	}
	glEnd();
}

void drawGluDisk(float innerRadius, float outerRadius, float slices, float loops)
{
	GLUquadricObj *disk = NULL;
	disk = gluNewQuadric();
	gluQuadricTexture(disk, GLU_TRUE);
	gluQuadricDrawStyle(disk, GLU_FILL);
	gluDisk(disk, innerRadius, outerRadius, slices, loops);
	gluDeleteQuadric(disk);
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

}

void drawHeadArmour()
{
//Forehead
	glPushMatrix();           //First front cube
		glTranslatef(0.0, 3.45, 0.29);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.04, 0.08, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Two side piece (Left)
		glTranslatef(0.06, 3.46, 0.278);
		glRotatef(20, 0.0, 1.0, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(15, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.08, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.148, 3.51, 0.218);
		glRotatef(40, 0.0, 1.0, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(23, 0.0, 0.0, 1.0);
		glScalef(0.12, 0.06, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Two side piece (Right)
		glTranslatef(-0.06, 3.46, 0.278);
		glRotatef(-20, 0.0, 1.0, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(-15, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.08, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-0.148, 3.51, 0.218);
		glRotatef(-40, 0.0, 1.0, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glRotatef(-23, 0.0, 0.0, 1.0);
		glScalef(0.12, 0.06, 0.01);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();           //First front cube (Front abit part)
		glTranslatef(0.0, 3.46, 0.35);
		glRotatef(13, 1.0, 0.0, 0.0);
		glScalef(0.04, 0.06, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Left piece
		glTranslatef(0.09, 3.48, 0.297);
		glRotatef(-40, 0.0, 1.0, 0.0);
		glRotatef(13, 1.0, 0.0, 0.0);
		glScalef(0.01, 0.05, 0.08);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Right piece
		glTranslatef(-0.09, 3.48, 0.297);
		glRotatef(40, 0.0, 1.0, 0.0);
		glRotatef(13, 1.0, 0.0, 0.0);
		glScalef(0.01, 0.05, 0.08);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();           //Front top steep cube
		glTranslatef(0.0, 3.58, 0.255);
		glRotatef(-40, 1.0, 0.0, 0.0);
		glScalef(0.04, 0.07, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Front top steep left piece
		glTranslatef(0.065, 3.57, 0.218);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.05, 0.01, 0.07);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Front top steep right piece
		glTranslatef(-0.065, 3.57, 0.218);
		glRotatef(20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.05, 0.01, 0.07);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();           //Top steep cube
		glTranslatef(0.0, 3.68, 0.1);
		glRotatef(-60, 1.0, 0.0, 0.0);
		glScalef(0.04, 0.15, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep left piece
		glTranslatef(0.065, 3.6, 0.16);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.1, 0.01, 0.07);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep left piece (up abit)
		glTranslatef(0.085, 3.63, 0.08);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.14, 0.01, 0.09);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep right piece
		glTranslatef(-0.065, 3.6, 0.16);
		glRotatef(20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.1, 0.01, 0.07);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep left piece (up abit)
		glTranslatef(-0.085, 3.63, 0.08);
		glRotatef(20, 0.0, 0.0, 1.0);
		glRotatef(40, 1.0, 0.0, 0.0);
		glScalef(0.14, 0.01, 0.09);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();           //Back top steep cube
		glTranslatef(0.0, 3.71, -0.1);
		glRotatef(60, 1.0, 0.0, 0.0);
		glScalef(0.04, 0.08, 0.01);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep left piece (up abit)
		glTranslatef(0.09, 3.64, -0.06);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.14, 0.01, 0.09);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();           //Top steep right piece (up abit)
		glTranslatef(-0.09, 3.64, -0.06);
		glRotatef(20, 0.0, 0.0, 1.0);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.14, 0.01, 0.09);
		drawFilledCube();
	glPopMatrix();

//Face
	glPushMatrix();               //Front under piece (small cube)
		glTranslatef(0.0, 3.1, 0.4);
		glScalef(0.05, 0.07, 0.02);
		drawFilledCube();
	glPopMatrix();

// Left
	glPushMatrix();                //First Left front piece (Left side of small cube)
		glTranslatef(0.118, 3.15, 0.335);
		glRotatef(40, 0.0, 1.0, 0.0);
		glScalef(0.1, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on first left front piece
		glTranslatef(0.18, 3.27, 0.258);
		glRotatef(40, 0.0, 1.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.08, 0.38);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              //Second Left front piece
		glTranslatef(0.23, 3.2, 0.21);
		glRotatef(-30, 0.0, 1.0, 0.0);
		glScalef(0.02, 0.15, 0.08);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on second left front piece
		glTranslatef(0.253, 3.352, 0.132);
		glRotatef(60, 0.0, 1.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		glScalef(0.16, 0.22, 0.38);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              // Left piece
		glTranslatef(0.27, 3.315, 0.005);
		glScalef(0.02, 0.26, 0.13);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on left piece
		glTranslatef(0.291, 3.577, 0.132);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(0.26, 0.16, 0.39);
		drawFilledTriangle();
	glPopMatrix();

//Right
	glPushMatrix();                //First right front piece (right side of small cube)
		glTranslatef(-0.118, 3.15, 0.335);
		glRotatef(-40, 0.0, 1.0, 0.0);
		glScalef(0.1, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on first right front piece
		glTranslatef(-0.208, 3.27, 0.285);
		glRotatef(-40, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.08, 0.38);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              //Second Left front piece
		glTranslatef(-0.23, 3.2, 0.21);
		glRotatef(30, 0.0, 1.0, 0.0);
		glScalef(0.02, 0.15, 0.08);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on second left front piece
		glTranslatef(-0.287, 3.352, 0.149);
		glRotatef(-60, 0.0, 1.0, 0.0);
		glScalef(0.16, 0.22, 0.38);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();              //Right piece
		glTranslatef(-0.27, 3.315, 0.005);
		glScalef(0.02, 0.26, 0.13);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                // Triangle on right piece
		glTranslatef(-0.251, 3.577, 0.132);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(0.26, 0.16, 0.39);
		drawFilledTriangle();
	glPopMatrix();
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
	// Left Upper Shoulder (with missle)
	glPushMatrix();                 //Largest piece (Beside neck)
		glTranslatef(0.52, 3.0, 0.05);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.4, 0.02, 0.55);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                 //Piece behind head (left)
		glTranslatef(0.15, 3.3, -0.5);
		glRotatef(20, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.38, 0.02);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                 //Piece behind head (right)
		glTranslatef(-0.15, 3.3, -0.5);
		glRotatef(-20, 0.0, 0.0, 1.0);
		glScalef(0.3, 0.38, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //Missle  (front cube for easy texture)
		glTranslatef(0.68, 3.1, 0.18);
		glScalef(0.2, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();
	glPushMatrix();                //back cube for easier texture
		glTranslatef(0.68, 3.1, -0.02);
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
		glColor3f(1.0, 1.0, 1.0);
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
		glColor3f(1.0, 1.0, 1.0);
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
		glTranslatef(-0.68, 3.1, 0.18);
		glScalef(0.2, 0.12, 0.02);
		drawFilledCube();
	glPopMatrix();

	glPushMatrix();                //back cube for easier texture
		glTranslatef(-0.68, 3.1, -0.02);
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
		glColor3f(1.0, 1.0, 1.0);
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
		glColor3f(1.0, 1.0, 1.0);
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

void drawReactor() {       //Ironman Reactor       (how to rotate automatically in glPushMatrix)
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 2.1, 0.85);
		glRotatef(reactorRotateAngle, 1.0, 1.0, 1.0);
		drawSphere(0.4, 30, 30);
	glPopMatrix();
}

void drawReactorFireball()
{
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(0.0, 2.1, 0.85);
		drawSphere(0.2, 30, 30);
	glPopMatrix();
}

void drawReactorShell() {
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

void drawThorHammerHead() {
	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceFront.bmp");
	glBegin(GL_QUADS);
	{
		// Top Face
		glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
	}
	glEnd();
	endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceRight.bmp");
	glBegin(GL_QUADS);
	{
		// Left Face
		glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
	}
	glEnd();
	endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceTop.bmp");
	glBegin(GL_QUADS);
	{
		// Back Face
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);

	}
	glEnd();
	endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceLeft.bmp");
	glBegin(GL_QUADS);
	{
		// Right Face
		glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, -1.0f);
		glTexCoord2f(0, 0); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
	}
	glEnd();
	endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceFront.bmp");
	glBegin(GL_QUADS);
	{
		// Bottom Face
		glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
		glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);
	}
	glEnd();
	endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage("textureImage/thorFaceTop.bmp");
	glBegin(GL_QUADS);
	{
		// Front Face
		glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
	}
	glEnd();
	endTexture();
	glPopMatrix();
}

void drawWeapon(int weaponType)
{
	switch (weaponType)
	{
	case 1:
		//Drill hand
		glPushMatrix();
			loadBitmapImage("textureImage/drill.bmp");
			glTranslatef(0.0, -0.6, 0.0);
			glRotatef(drillRotate, 0.0, 1.0, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.4, 0.0, 2.5, 20, 10);
			endTexture();
		glPopMatrix();
		break;

	case 2:
		//Bullet hand

		glPushMatrix();      //Box
			loadBitmapImage("textureImage/gunBody.bmp");
			glScalef(0.3, 0.43, 0.3);
			drawFilledCubeT();
			endTexture();
		glPopMatrix();

		glPushMatrix();
			glRotatef(gunBarrelRotate, 0.0, 1.0, 0.0);
			loadBitmapImage("textureImage/gunBarrel.bmp");
			glPushMatrix();        //Bullet
				glTranslatef(0.15, -1.9, 0.15);
				glRotatef(-90, 1.0, 0.0, 0.0);
				drawCylinder(0.08, 0.08, 1.5, 30, 30);
			glPopMatrix();
			glPushMatrix();        //Bullet
				glTranslatef(-0.15, -1.9, 0.15);
				glRotatef(-90, 1.0, 0.0, 0.0);
				drawCylinder(0.08, 0.08, 1.5, 30, 30);
			glPopMatrix();
			glPushMatrix();        //Bullet
				glTranslatef(0.15, -1.9, -0.15);
				glRotatef(-90, 1.0, 0.0, 0.0);
				drawCylinder(0.08, 0.08, 1.5, 30, 30);
			glPopMatrix();
			glPushMatrix();        //Bullet
				glTranslatef(-0.15, -1.9, -0.15);
				glRotatef(-90, 1.0, 0.0, 0.0);
				drawCylinder(0.08, 0.08, 1.5, 30, 30);
			glPopMatrix();
			endTexture();
		glPopMatrix();
		break;

	case 3:
		//Laser gun
		glPushMatrix();
			loadBitmapImage("textureImage/canonBody.bmp");
			glScalef(0.6, 1.2, 0.6);
			drawSphere(0.6, 20, 20);
			endTexture();
		glPopMatrix();
		glPushMatrix();
			loadBitmapImage("textureImage/canonBarrel.bmp");
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.30, 0.30, 0.85, 50, 50);
			endTexture();
		glPopMatrix();
		glPushMatrix();
			loadBitmapImage("textureImage/canonHole.bmp");
			glTranslatef(0.0, -0.8, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCircle(0.3);
			endTexture();
		glPopMatrix();
		break;

	case 4:
		//Sword

		glPushMatrix();      //Box
			loadBitmapImage("textureImage/swordHandle.bmp");
			glScalef(0.4, 0.3, 0.3);
			drawFilledCubeT();
			endTexture();
		glPopMatrix();
		glPushMatrix();      //Long piece
			loadBitmapImage("textureImage/swordSteel.bmp");
			glTranslatef(1.5, 0.0, 0.0);
			glScalef(1.2, 0.26, 0.06);
			drawFilledCube();
			endTexture();
		glPopMatrix();

		loadBitmapImage("textureImage/swordEdge.bmp");
		glPushMatrix();        //Sharp side triangle
			glTranslatef(2.7, 0.26, 0.055);
			glRotatef(-90, 0.0, 0.0, 1.0);
			glScalef(0.8, 0.8, 1.1);
			drawFilledTriangle();

		glPopMatrix();
		glPushMatrix();      //Under cube piece
			glTranslatef(2.4, -0.4, 0.0);
			glScalef(0.3, 0.14, 0.06);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();        //Sharp side triangle (small)
			glTranslatef(2.099, -0.26, -0.06);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(-90, 0.0, 0.0, 1.0);
			glScalef(0.27, 0.4, 1.15);
			drawFilledTriangle();
		glPopMatrix();

		//Top of sword
		glPushMatrix();           //thin long cube
			glTranslatef(1.5, 0.21, 0.1);
			glScalef(0.5, 0.08, 0.04);
			drawFilledCubeT();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.5, 0.21, -0.1);
			glScalef(0.5, 0.08, 0.04);
			drawFilledCubeT();
		glPopMatrix();

		glPushMatrix();        //front Sharp side triangle (small)
			glTranslatef(2.6, 0.26, 0.14);
			glRotatef(-90, 0.0, 0.0, 1.0);
			glScalef(0.2, 0.5, 0.8);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();        //front Sharp side triangle (small)
			glTranslatef(2.6, 0.26, -0.06);
			glRotatef(-90, 0.0, 0.0, 1.0);
			glScalef(0.2, 0.5, 0.8);
			drawFilledTriangle();
		glPopMatrix();
		endTexture();

		loadBitmapImage("textureImage/swordTopThin.bmp");
		glPushMatrix();           //steep piece
			glTranslatef(1.82, 0.35, 0.0);
			glRotatef(30, 0.0, 0.0, 1.0);
			glScalef(0.2, 0.02, 0.1);
			drawFilledCubeT();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(2.75, 0.35, 0.0);
			glRotatef(-30, 0.0, 0.0, 1.0);
			glScalef(0.2, 0.02, 0.1);
			drawFilledCubeT();
		glPopMatrix();
		endTexture();

		loadBitmapImage("textureImage/swordTopFat.bmp");
		glPushMatrix();           //second thick long cube
			glTranslatef(2.3, 0.26, 0.1);
			glScalef(0.3, 0.2, 0.04);
			drawFilledCubeT();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(2.3, 0.26, -0.1);
			glScalef(0.3, 0.2, 0.04);
			drawFilledCubeT();
		glPopMatrix();

		glPushMatrix();           //horizontal piece
			glTranslatef(2.3, 0.44, 0.0);
			glScalef(0.30, 0.02, 0.1);
			drawFilledCubeT();
		glPopMatrix();
		endTexture();

		break;
	case 5:
		glPushMatrix();
			loadBitmapImage("textureImage/thorHandle.bmp");
			glTranslatef(-1.5, -0.9, -0.9);
			drawCylinder(0.1, 0.1, 1.5, 50, 50);
			endTexture();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.5, -0.9, 1.0);
			glScalef(0.5, 0.8, 0.5);

			drawThorHammerHead();

		glPopMatrix();
	default:
		break;
	}
}

void drawLeftShield()
{
	glPushMatrix();          //Front long piece
		loadBitmapImage(shield3Array[textureReactorIndex].data());
		glScalef(0.4, 1.5, 0.05);
		drawFilledCubeT();
		endTexture();
	glPopMatrix();

	glPushMatrix();         // steep cube inside
		glTranslatef(-0.1, 0.3, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.4, 0.1);
		//drawFilledCubeT();
	glPopMatrix();

	glPushMatrix();        //Top cube piece
		glTranslatef(-0.1, 1.83, -0.195);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.4, 0.05);
		drawFilledCubeT();
	glPopMatrix();
	glPushMatrix();        //Top triangle piece
		glTranslatef(0.2, 1.5, 0.05);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.2, 0.8, 1.0);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();        //Bottom cube piece
	glTranslatef(-0.0, -1.85, -0.06);
	glRotatef(10, 1.0, 0.0, 0.0);
	glScalef(0.4, 0.4, 0.05);
	drawFilledCubeT();
	glPopMatrix();

	glPushMatrix();        //Side piece
		loadBitmapImage(shield4Array[textureReactorIndex].data());
		glTranslatef(0.85, 0.0, -0.17);
		glRotatef(20, 0.0, 1.0 ,0.0);
		glScalef(0.5, 1.5, 0.05);
		drawFilledCubeT();
		endTexture();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.68, 1.805, -0.35);
	glRotatef(20, 0.0, 1.0, 0.0);
	glRotatef(-36, 1.0, 0.0, 0.0);
	glScalef(0.4, 0.4, 0.05);
	drawFilledCubeT();

	glPopMatrix();
}

void drawRightShield()
{
	glPushMatrix();          //Front long piece
		loadBitmapImage(shield2Array[textureReactorIndex].data());
		glScalef(-0.4, 1.5, 0.05);
		drawFilledCubeT();
		endTexture();
	glPopMatrix();

	glPushMatrix();         // steep cube inside
		glTranslatef(0.1, 0.3, 0.0);
		glRotatef(10, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.4, 0.1);
		//drawFilledCubeT();
	glPopMatrix();

	glPushMatrix();        //Top cube piece
		glTranslatef(0.1, 1.83, -0.195);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.4, 0.05);
		drawFilledCubeT();
	glPopMatrix();
	glPushMatrix();        //Top triangle piece
		glTranslatef(-0.2, 1.45, -0.03
);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glRotatef(180, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.8, 1.0);
		drawFilledTriangle();
	glPopMatrix();

	glPushMatrix();        //Bottom cube piece
	glTranslatef(0.0, -1.85, -0.06);
	glRotatef(10, 1.0, 0.0, 0.0);
	glScalef(0.4, 0.4, 0.05);
	drawFilledCubeT();
	glPopMatrix();

	glPushMatrix();        //Side piece
		loadBitmapImage(shield1Array[textureReactorIndex].data());
		glTranslatef(-0.85, 0.0, -0.17);
		glRotatef(-20, 0.0, 1.0, 0.0);
		glScalef(0.5, 1.5, 0.05);
		drawFilledCubeT();
		endTexture();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-0.68, 1.805, -0.35);
		glRotatef(-20, 0.0, 1.0, 0.0);
		glRotatef(-36, 1.0, 0.0, 0.0);
		glScalef(0.4, 0.4, 0.05);
		drawFilledCubeT();
	glPopMatrix();
}


void drawLeftShoulderArm()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();            //Horizontal cylinder connect body and left hand
		glTranslatef(0.8, 2.5, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		drawCylinder(0.2, 0.2, 0.8, 50, 50);
		glPopMatrix();

		glPushMatrix();            //First top vertical cylinder
		glTranslatef(1.55, 2.8, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawCylinder(0.2, 0.2, 0.7, 50, 50);
		glPopMatrix();

		glPushMatrix();            //Cube between elbow and shoulder (arm)
		glTranslatef(1.55, 1.8, 0.0);
		glScalef(0.2, 0.4, 0.2);
		drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
	loadBitmapImage(armorTextureArray[textureSetIndex].data());
//Shoulder outside armor
		glPushMatrix();         //Top horizontal
			glTranslatef(1.55, 3.0, 0.0);
			glScalef(0.3, 0.02, 0.25);
			drawFilledCube();
		glPopMatrix();

		//Inside steep part (right of top horizontal)
		glPushMatrix();
		glTranslatef(1.195, 2.87, 0.0);
		glRotatef(65, 0.0, 0.0, 1.0);
		glScalef(0.15, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // inside small horizontal
		glTranslatef(1.03, 2.75, 0.0);
		glScalef(0.12, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();

		glPushMatrix();          //Front piece infront horizontal and back piece
		glTranslatef(1.09, 2.5, 0.31);
		glRotatef(-26, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.25, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(1.09, 2.5, -0.31);
		glRotatef(26, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.25, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Front steep infront horizontal
		glPushMatrix();
		glTranslatef(1.55, 2.957, 0.319);
		glRotatef(30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.02, 0.1);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Front vertical after steep
		glTranslatef(1.55, 2.58, 0.395);
		glScalef(0.3, 0.35, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between front and outside
		glPushMatrix();
		glTranslatef(1.92, 2.864, 0.3);
		glRotatef(50, 0.0, 1.0, 0.0);
		glRotatef(-50, 1.0, 0.0, 0.0);
		glScalef(0.12, 0.12, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside steep part (Left of top horizontal)
		glPushMatrix();
		glTranslatef(1.96, 2.905, 0.0);
		glRotatef(-40, 0.0, 0.0, 1.0);
		glScalef(0.16, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(2.12, 2.7, 0.0);
		glRotatef(-70, 0.0, 0.0, 1.0);
		glScalef(0.12, 0.02, 0.14);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();           //Front triangle
		glTranslatef(2.07, 2.81, 0.145);
		glRotatef(-70, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(0.23, 0.12, 0.3);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();           //Back triangle
		glTranslatef(2.11, 2.81, -0.13);
		glRotatef(-70, 0.0, 0.0, 1.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glScalef(0.23, 0.12, 0.3);
		drawFilledTriangle();
		glPopMatrix();

		//Between front and outside
		glPushMatrix();
		glTranslatef(1.92, 2.864, -0.3);
		glRotatef(-50, 0.0, 1.0, 0.0);
		glRotatef(50, 1.0, 0.0, 0.0);
		glScalef(0.12, 0.12, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back steep behind horizontal
		glPushMatrix();
		glTranslatef(1.55, 2.957, -0.319);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.02, 0.1);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Front vertical after steep
		glTranslatef(1.55, 2.58, -0.395);
		glScalef(-0.3, 0.35, 0.02);
		drawFilledCube();
		glPopMatrix();



		//Inside Shoulder to arm
		glPushMatrix();          //Top horizontal part (Shoulder)
		glTranslatef(1.55, 2.9, 0.0);
		glScalef(0.2, 0.03, 0.2);
		drawFilledCube();
		glPopMatrix();

		//Front
		glPushMatrix();          //Front steep (Shoulder)
		glTranslatef(1.55, 2.86, 0.25);
		glRotatef(-50, 1.0, 0.0, 0.0);
		glScalef(0.20, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Arm - biceps)
		glTranslatef(1.55, 2.12, 0.31);
		glScalef(0.20, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Front and outside y steep piece
		glPushMatrix();
		glTranslatef(1.8, 2.12, 0.25);
		glRotatef(45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside
		glPushMatrix();          //Outside steep (Shoulder)
		glTranslatef(1.8, 2.86, 0.0);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.20);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Outside vertical cube (Arm - biceps)
		glTranslatef(1.85, 2.12, 0.0);
		glScalef(0.02, 0.7, 0.20);
		drawFilledCube();
		glPopMatrix();

		//Between Front and outside y steep piece
		glPushMatrix();
		glTranslatef(1.8, 2.12, -0.25);
		glRotatef(-45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back
		glPushMatrix();          //Back steep (Shoulder)
		glTranslatef(1.55, 2.86, -0.25);
		glRotatef(50, 1.0, 0.0, 0.0);
		glScalef(0.20, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Back vertical cube (Arm - biceps)
		glTranslatef(1.55, 2.12, -0.31);
		glScalef(0.20, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Back and inside y steep piece
		glPushMatrix();
		glTranslatef(1.3, 2.12, -0.25);
		glRotatef(45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Inside
		glPushMatrix();          //Inside steep (Shoulder)
		glTranslatef(1.3, 2.86, 0.0);
		glRotatef(-50, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.20);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Inside vertical cube (Arm - biceps)
		glTranslatef(1.25, 1.82, 0.0);
		glScalef(0.02, 0.4, 0.20);
		drawFilledCube();
		glPopMatrix();

		//Between inside and front y steep piece
		glPushMatrix();
		glTranslatef(1.3, 2.12, 0.25);
		glRotatef(-45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();
}

void drawLeftElbowForearm()
{
	glPushMatrix();
	loadBitmapImage(innerTextureArray[textureSetIndex].data());
//Inner part
		glPushMatrix();            //Left elbow
		glTranslatef(1.3, 1.2, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		drawCylinder(0.2, 0.2, 0.5, 50, 50);
		glPopMatrix();

		glPushMatrix();            //Cube between elbow and palm (forearm)
		glTranslatef(1.55, 0.36, 0.0);
		glScalef(0.17, 0.65, 0.17);
		drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
	loadBitmapImage(armorTextureArray[textureSetIndex].data());
		//Forearm
		//Front
		glPushMatrix();         //Front steep (Forearm)
		glTranslatef(1.55, 0.98, 0.22);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.18, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Forearm)
		glTranslatef(1.55, 0.31, 0.26);
		glScalef(0.18, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Front and outside y steep piece (forearm)
		glPushMatrix();
		glTranslatef(1.785, 0.31, 0.219);
		glRotatef(35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside
		glPushMatrix();           //Outside steep (back triangle)
		glTranslatef(1.87, 0.92, -0.05);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(0.14, 0.4, 0.5);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();         // Outside steep (middle cube)
		glTranslatef(1.85, 1.12, 0.0);
		glScalef(0.02, 0.2, 0.05);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();           //Outside steep (front triangle)
		glTranslatef(1.82, 0.92, 0.05);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glScalef(0.14, 0.4, 0.5);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();         // Outside vertical cube (Forearm)
		glTranslatef(1.85, 0.31, 0.0);
		glScalef(0.02, 0.61, 0.18);
		drawFilledCube();
		glPopMatrix();


		//Between Outside and back y steep piece (forearm)
		glPushMatrix();
		glTranslatef(1.785, 0.31, -0.219);
		glRotatef(-35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back
		glPushMatrix();         //Back steep (Forearm)
		glTranslatef(1.55, 0.98, -0.22);
		glRotatef(30, 1.0, 0.0, 0.0);
		glScalef(0.18, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Back vertical cube (Forearm)
		glTranslatef(1.55, 0.31, -0.26);
		glScalef(0.18, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Back and inside y steep piece (forearm)
		glPushMatrix();
		glTranslatef(1.307, 0.31, -0.219);
		glRotatef(35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Inside
		glPushMatrix();          //Inside steep (Forearm)
		glTranslatef(1.29, 0.98, 0.0);
		glRotatef(-30, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.18);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Inside vertical cube (Forearm)
		glTranslatef(1.25, 0.31, 0.0);
		glScalef(0.02, 0.61, 0.18);
		drawFilledCube();
		glPopMatrix();

		//Between Inside and Front y steep piece (forearm)
		glPushMatrix();
		glTranslatef(1.307, 0.31, 0.219);
		glRotatef(-35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();

	//Left Shield
	if (handShield == true)
	{
		glPushMatrix();        //Left hand Shield
		glTranslatef(2.02, 0.6, 0.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glRotatef(190, 0.0, 0.0, 1.0);
		drawLeftShield();
		glPopMatrix();
	}
	if (handShield == false)
	{
		if (drawThorHammer) {
			glPushMatrix();
			loadBitmapImage("textureImage/capShield.bmp");
			glTranslatef(1.90, 0.0, 0.0);
			glRotatef(90, 0.0, 1.0, 0.0);
			drawGluDisk(0.0, 1.3, 30, 30);
			endTexture();
			glPopMatrix();

			glPushMatrix();
			loadBitmapImage("textureImage/capShieldInner.bmp");
			glTranslatef(1.89, 0.0, 0.0);
			glRotatef(90, 0.0, 1.0, 0.0);
			drawGluDisk(0.0, 1.3, 30, 30);
			endTexture();
			glPopMatrix();
		}
	}
	if (drawDSCircle == true)
	{
		glPushMatrix();
		glTranslatef(1.7, -1.7, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawGluDisk(0.0, 1.2, 30, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.7, -1.75, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawGluDisk(0.0, 1.6, 30, 30);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1.7, -1.80, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawGluDisk(0.0, 1.2, 30, 30);
		glPopMatrix();
	}
}

void left4FingerPart1()
{
	//Index finger
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.53, -1.0, 0.2);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.53, -1.0, 0.06);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.53, -1.0, -0.08);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.53, -1.0, -0.22);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();
}

void left4FingerPart2()
{
	//Index finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.52, -1.12, 0.2);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.48, -1.25, 0.2);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.52, -1.12, 0.06);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.48, -1.25, 0.06);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.52, -1.12, -0.08);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.48, -1.25, -0.08);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.52, -1.12, -0.22);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.48, -1.25, -0.22);
	glRotatef(-20, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();
}

void left4FingerPart3()
{
	//Index Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.44, -1.36, 0.2);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.35, -1.45, 0.2);
	glRotatef(-50, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.44, -1.36, 0.06);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.35, -1.45, 0.06);
	glRotatef(-50, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.44, -1.36, -0.08);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.35, -1.45, -0.08);
	glRotatef(-50, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();            //small sphere (circle bone)
	glTranslatef(1.44, -1.36, -0.22);
	drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(1.35, -1.45, -0.22);
	glRotatef(-50, 0.0, 0.0, 1.0);
	glScalef(0.04, 0.09, 0.05);
	drawFilledCube1();
	glPopMatrix();
}

void drawLeftHandPalm()
{
	//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();            //cylinder between forearm and palm (wrist)
		glColor3f(0.0, 0.0, 0.0);
		glTranslatef(1.55, -0.20, -0.0);
		glScalef(1.0, 1.0, 1.5);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawCylinder(0.14, 0.08, 0.3, 50, 50);
		glPopMatrix();

		glPushMatrix();           //Palm
		glTranslatef(1.55, -0.7, 0.0);
		glScalef(0.05, 0.2, 0.2);
		drawFilledCube1();
		glPopMatrix();
		endTexture();
	glPopMatrix();


	switch (weaponTypeL)
	{
		case 0:
			//Wrist
			glPushMatrix();
				loadBitmapImage(innerTextureArray[textureSetIndex].data());
				glPushMatrix();       //Front piece
					glTranslatef(1.55, -0.4, 0.25);
					glScalef(0.15, 0.09, 0.015);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Outside piece
					glTranslatef(1.7, -0.4, 0.0);
					glScalef(0.015, 0.09, 0.24);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Back piece
					glTranslatef(1.55, -0.4, -0.25);
					glScalef(0.15, 0.09, 0.015);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Inside piece
					glTranslatef(1.4, -0.4, 0.0);
					glScalef(0.015, 0.09, 0.24);
					drawFilledCube();
				glPopMatrix();
			endTexture();
			glPopMatrix();

			glPushMatrix();
				loadBitmapImage(innerTextureArray[textureSetIndex].data());
				//Palm
				glPushMatrix();        //Outside
					glTranslatef(1.6, -0.7, 0.0);
					glScalef(0.03, 0.22, 0.25);
					drawFilledCube();
				glPopMatrix();
				glPushMatrix();        //Inside
					glTranslatef(1.47, -0.7, 0.0);
					glScalef(0.03, 0.22, 0.25);
					drawFilledCube();
				glPopMatrix();

				//Finger
					//Thumb
				glPushMatrix();
					glColor3f(1.0, 1.0, 1.0);
					glTranslatef(1.53, -0.7, 0.28);
					glRotatef(-30, 1.0, 0.0, 0.0);
					glScalef(0.04, 0.09, 0.05);
					drawFilledCube1();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(1.52, -0.816, 0.345);
					glRotatef(fingerAngle2, 0.0, 1.0, 0.0);
					glTranslatef(-1.52, 0.816, -0.345);
					{
						glPushMatrix();            //small sphere (circle bone)
							glTranslatef(1.52, -0.816, 0.345);
							drawSphere(0.04, 60, 60);
						glPopMatrix();
						glPushMatrix();
							glTranslatef(1.45, -0.92, 0.39);
							glRotatef(-30, 0.0, 0.0, 1.0);
							glRotatef(-20, 1.0, 0.0, 0.0);
							glScalef(0.04, 0.09, 0.05);
							drawFilledCube1();
						glPopMatrix();
					}
				glPopMatrix();

				//All 4 fingers
				glPushMatrix();
					glTranslatef(1.53, -1.0, 0.0);
					glRotatef(fingerAngle1, 0.0, 0.0, 1.0);
					glTranslatef(-1.53, 1.0, 0.0);
					left4FingerPart1();
					{
						glPushMatrix();
							glTranslatef(1.52, -1.12, 0.0);
							glRotatef(fingerAngle2, 0.0, 0.0, 1.0);
							glTranslatef(-1.52, 1.12, 0.0);
							left4FingerPart2();
							{
								glPushMatrix();
									glTranslatef(1.44, -1.36, 0.0);
									glRotatef(fingerAngle2, 0.0, 0.0, 1.0);
									glTranslatef(-1.44, 1.36, 0.0);
									left4FingerPart3();
								glPopMatrix();
							}
						glPopMatrix();
					}
				glPopMatrix();
				endTexture();
			glPopMatrix();


			break;
		case 1:
			glPushMatrix();
				glTranslatef(1.55, 0.1, 0.0);
				drawWeapon(1);
			glPopMatrix();
			break;
		case 2:
			glPushMatrix();
				glTranslatef(1.55, -0.9, 0.0);
				drawWeapon(2);
			glPopMatrix();
			break;
		case 3:
			glPushMatrix();
				glTranslatef(1.55, -1.0, 0.0);
				drawWeapon(3);
			glPopMatrix();
			break;
		case 4:
			glPushMatrix();
				glTranslatef(1.55, -0.6, 0.0);
				glRotatef(-90, 0.0, 1.0, 0.0);
				glRotatef(-90, 0.0, 0.0, 1.0);
				drawWeapon(4);
			glPopMatrix();
			break;
		default:
			break;
	}
}


void drawRightShoulderArm()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();            //Horizontal cylinder connect body and right hand
		glTranslatef(-0.8, 2.5, 0.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		drawCylinder(0.2, 0.2, 0.8, 50, 50);
		glPopMatrix();

		glPushMatrix();            //First top vertical cylinder
		glTranslatef(-1.55, 2.8, 0.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		drawCylinder(0.2, 0.2, 0.7, 50, 50);
		glPopMatrix();

		glPushMatrix();             //Cube between elbow and shoulder
		glTranslatef(-1.55, 1.8, 0.0);
		glScalef(0.2, 0.4, 0.2);
		drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		//Shoulder outside armor
		glPushMatrix();         //Top horizontal
		glTranslatef(-1.55, 3.0, 0.0);
		glScalef(0.3, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();

		//Inside steep part (right of top horizontal)
		glPushMatrix();
		glTranslatef(-1.195, 2.87, 0.0);
		glRotatef(-65, 0.0, 0.0, 1.0);
		glScalef(0.15, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-1.03, 2.75, 0.0);
		glScalef(0.12, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();

		glPushMatrix();          //Front piece infront horizontal and back also
		glTranslatef(-1.09, 2.5, 0.31);
		glRotatef(26, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.25, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-1.09, 2.5, -0.31);
		glRotatef(-26, 0.0, 1.0, 0.0);
		glScalef(0.2, 0.25, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Front steep infront horizontal
		glPushMatrix();
		glTranslatef(-1.55, 2.957, 0.319);
		glRotatef(30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.02, 0.1);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Front vertical after steep
		glTranslatef(-1.55, 2.58, 0.395);
		glScalef(0.3, 0.35, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between front and outside piece
		glPushMatrix();
		glTranslatef(-1.92, 2.864, 0.3);
		glRotatef(-50, 0.0, 1.0, 0.0);
		glRotatef(-50, 1.0, 0.0, 0.0);
		glScalef(0.12, 0.12, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside steep part (Left of top horizontal)
		glPushMatrix();
		glTranslatef(-1.96, 2.905, 0.0);
		glRotatef(40, 0.0, 0.0, 1.0);
		glScalef(0.16, 0.02, 0.25);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-2.12, 2.7, 0.0);
		glRotatef(70, 0.0, 0.0, 1.0);
		glScalef(0.12, 0.02, 0.14);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();           //Front triangle
		glTranslatef(-2.1, 2.81, 0.145);
		glRotatef(-110, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glScalef(0.23, 0.12, 0.3);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();           //Back triangle
		glTranslatef(-2.0175, 2.79, -0.13);
		glRotatef(-110, 0.0, 0.0, 1.0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glScalef(0.23, 0.12, 0.3);
		drawFilledTriangle();
		glPopMatrix();

		//Between front and outside
		glPushMatrix();
		glTranslatef(-1.92, 2.864, -0.3);
		glRotatef(50, 0.0, 1.0, 0.0);
		glRotatef(50, 1.0, 0.0, 0.0);
		glScalef(0.12, 0.12, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back steep behind horizontal
		glPushMatrix();
		glTranslatef(-1.55, 2.957, -0.319);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.3, 0.02, 0.1);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Front vertical after steep
		glTranslatef(-1.55, 2.58, -0.395);
		glScalef(-0.3, 0.35, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Shoulder to arm
		glPushMatrix();          //Top horizontal part (Shoulder)
		glTranslatef(-1.55, 2.9, 0.0);
		glScalef(0.2, 0.03, 0.2);
		drawFilledCube();
		glPopMatrix();

		//Front
		glPushMatrix();          //Front steep (Shoulder)
		glTranslatef(-1.55, 2.86, 0.25);
		glRotatef(-50, 1.0, 0.0, 0.0);
		glScalef(0.20, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Arm - biceps)
		glTranslatef(-1.55, 2.12, 0.31);
		glScalef(0.20, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		// Between Front and outside y steep piece
		glPushMatrix();
		glTranslatef(-1.8, 2.12, 0.25);
		glRotatef(-45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside
		glPushMatrix();          //Front steep (Shoulder)
		glTranslatef(-1.8, 2.86, 0.0);
		glRotatef(-50, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.20);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Arm - biceps)
		glTranslatef(-1.85, 2.12, 0.0);
		glScalef(0.02, 0.7, 0.20);
		drawFilledCube();
		glPopMatrix();

		//Between Front and outside y steep piece
		glPushMatrix();
		glTranslatef(-1.8, 2.12, -0.25);
		glRotatef(45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back
		glPushMatrix();          //Back steep (Shoulder)
		glTranslatef(-1.55, 2.86, -0.25);
		glRotatef(50, 1.0, 0.0, 0.0);
		glScalef(0.20, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Back vertical cube (Arm - biceps)
		glTranslatef(-1.55, 2.12, -0.31);
		glScalef(0.20, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Back and inside y steep piece
		glPushMatrix();
		glTranslatef(-1.3, 2.12, -0.25);
		glRotatef(-45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Inside
		glPushMatrix();          //Front steep (Shoulder)
		glTranslatef(-1.3, 2.86, 0.0);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.20);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Arm - biceps)
		glTranslatef(-1.25, 1.82, 0.0);
		glScalef(0.02, 0.4, 0.20);
		drawFilledCube();
		glPopMatrix();

		//Between inside and front y steep piece
		glPushMatrix();
		glTranslatef(-1.3, 2.12, 0.25);
		glRotatef(45, 0.0, 1.0, 0.0);
		glScalef(0.09, 0.7, 0.02);
		drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();
}

void drawRightElboxForearm()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();            //Right elbow
		glTranslatef(-1.3, 1.2, 0.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		drawCylinder(0.2, 0.2, 0.5, 50, 50);
		glPopMatrix();

		glPushMatrix();            //Cube between elbow and palm (forearm)
		glTranslatef(-1.55, 0.36, 0.0);
		glScalef(0.17, 0.65, 0.17);
		drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		//Forearm
		//Front
		glPushMatrix();         //Front steep (Forearm)
		glTranslatef(-1.55, 0.98, 0.22);
		glRotatef(-30, 1.0, 0.0, 0.0);
		glScalef(0.18, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Front vertical cube (Forearm)
		glTranslatef(-1.55, 0.31, 0.26);
		glScalef(0.18, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Front and outside y steep piece (forearm)
		glPushMatrix();
		glTranslatef(-1.785, 0.31, 0.219);
		glRotatef(-35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Outside
		glPushMatrix();           //Outside steep (back triangle)
		glTranslatef(-1.87, 0.92, 0.05);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glScalef(0.14, 0.4, 0.5);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();         // Outside steep (middle cube)
		glTranslatef(-1.85, 1.12, 0.0);
		glScalef(0.02, 0.2, 0.05);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();           //Outside steep (front triangle)
		glTranslatef(-1.82, 0.92, -0.05);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(0.14, 0.4, 0.5);
		drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();         // Outside vertical cube (Forearm)
		glTranslatef(-1.85, 0.31, 0.0);
		glScalef(0.02, 0.61, 0.18);
		drawFilledCube();
		glPopMatrix();


		//Between Outside and back y steep piece (forearm)
		glPushMatrix();
		glTranslatef(-1.785, 0.31, -0.219);
		glRotatef(35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Back
		glPushMatrix();         //Back steep (Forearm)
		glTranslatef(-1.55, 0.98, -0.22);
		glRotatef(30, 1.0, 0.0, 0.0);
		glScalef(0.18, 0.08, 0.02);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Back vertical cube (Forearm)
		glTranslatef(-1.55, 0.31, -0.26);
		glScalef(0.18, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Between Back and inside y steep piece (forearm)
		glPushMatrix();
		glTranslatef(-1.307, 0.31, -0.219);
		glRotatef(-35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

		//Inside
		glPushMatrix();          //Inside steep (Forearm)
		glTranslatef(-1.29, 0.98, 0.0);
		glRotatef(30, 0.0, 0.0, 1.0);
		glScalef(0.02, 0.08, 0.18);
		drawFilledCube();
		glPopMatrix();
		glPushMatrix();         // Inside vertical cube (Forearm)
		glTranslatef(-1.25, 0.31, 0.0);
		glScalef(0.02, 0.61, 0.18);
		drawFilledCube();
		glPopMatrix();

		//Between Inside and front y steep piece (forearm)
		glPushMatrix();
		glTranslatef(-1.307, 0.31, 0.218);
		glRotatef(35, 0.0, 1.0, 0.0);
		glScalef(0.08, 0.61, 0.02);
		drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();

	//Right Shield
	if (handShield == true)
	{
		glPushMatrix();        //Right hand Shield
		glTranslatef(-2.02, 0.6, 0.0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glRotatef(170, 0.0, 0.0, 1.0);
		drawRightShield();
		glPopMatrix();
	}
}

void right4FingerPart1()
{
	//Index Finger
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.53, -1.0, 0.2);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.53, -1.0, 0.06);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.53, -1.0, -0.08);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.53, -1.0, -0.22);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();
}

void right4FingerPart2()
{
	//Index Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.52, -1.12, 0.2);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.48, -1.25, 0.2);
		glRotatef(20, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.52, -1.12, 0.06);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.48, -1.25, 0.06);
		glRotatef(20, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.52, -1.12, -0.08);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.48, -1.25, -0.08);
		glRotatef(20, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.52, -1.12, -0.22);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.48, -1.25, -0.22);
		glRotatef(20, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();
}

void right4FingerPart3()
{
	//Index Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.44, -1.36, 0.2);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
		glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.35, -1.45, 0.2);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Middle Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.44, -1.36, 0.06);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.35, -1.45, 0.06);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Ring Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.44, -1.36, -0.08);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.35, -1.45, -0.08);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();

	//Pinky Finger
	glPushMatrix();            //small sphere (circle bone)
		glTranslatef(-1.44, -1.36, -0.22);
		drawSphere(0.04, 60, 60);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		glTranslatef(-1.35, -1.45, -0.22);
		glRotatef(50, 0.0, 0.0, 1.0);
		glScalef(0.04, 0.09, 0.05);
		drawFilledCube1();
	glPopMatrix();
}

void drawRightHandPalm()
{
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
	//Inner part
		glPushMatrix();            //cylinder between forearm and palm (wrist)
		glColor3f(0.0, 0.0, 0.0);
			glTranslatef(-1.55, -0.20, -0.0);
			glScalef(1.0, 1.0, 1.5);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.14, 0.08, 0.3, 50, 50);
		glPopMatrix();
		glPushMatrix();           //Palm
			glTranslatef(-1.55, -0.7, 0.0);
			glScalef(0.05, 0.2, 0.2);
			drawFilledCube1();
		glPopMatrix();
		endTexture();
	glPopMatrix();

	switch (weaponTypeR)
	{
		case 0:
			if (drawThorHammer == true) {
				drawWeapon(5);
			}

			glPushMatrix();
			loadBitmapImage(innerTextureArray[textureSetIndex].data());
				//Wrist
				glPushMatrix();       //Front piece
					glTranslatef(-1.55, -0.4, 0.25);
					glScalef(0.15, 0.09, 0.015);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Outside piece
					glTranslatef(-1.7, -0.4, 0.0);
					glScalef(0.015, 0.09, 0.24);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Back piece
					glTranslatef(-1.55, -0.4, -0.25);
					glScalef(0.15, 0.09, 0.015);
					drawFilledCube();
				glPopMatrix();

				glPushMatrix();       //Inside piece
					glTranslatef(-1.4, -0.4, 0.0);
					glScalef(0.015, 0.09, 0.24);
					drawFilledCube();
				glPopMatrix();
				endTexture();
			glPopMatrix();

			glPushMatrix();
				loadBitmapImage(innerTextureArray[textureSetIndex].data());
				//Palm
				glPushMatrix();        //Outside
					glTranslatef(-1.6, -0.7, 0.0);
					glScalef(0.03, 0.22, 0.25);
					drawFilledCube();
				glPopMatrix();
				glPushMatrix();        //Inside
					glTranslatef(-1.47, -0.7, 0.0);
					glScalef(0.03, 0.22, 0.25);
					drawFilledCube();
				glPopMatrix();

				//Finger
					//Thumb
				glPushMatrix();
					glColor3f(1.0, 1.0, 1.0);
					glTranslatef(-1.53, -0.7, 0.28);
					glRotatef(-30, 1.0, 0.0, 0.0);
					glScalef(0.04, 0.09, 0.05);
					drawFilledCube1();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(-1.52, -0.816, 0.345);
					glRotatef(fingerAngle3, 0.0, 1.0, 0.0);
					glTranslatef(1.52, 0.816, -0.345);
					{
						glPushMatrix();            //small sphere (circle bone)
							glTranslatef(-1.52, -0.816, 0.345);
							drawSphere(0.04, 60, 60);
						glPopMatrix();
						glPushMatrix();
							glTranslatef(-1.45, -0.92, 0.39);
							glRotatef(30, 0.0, 0.0, 1.0);
							glRotatef(-20, 1.0, 0.0, 0.0);
							glScalef(0.04, 0.09, 0.05);
							drawFilledCube1();
						glPopMatrix();
					}
				glPopMatrix();

				//All 4 fingers
				glPushMatrix();
					glTranslatef(-1.53, -1.0, 0.2);
					glRotatef(fingerAngle3, 0.0, 0.0, 1.0);
					glTranslatef(1.53, 1.0, -0.2);
					right4FingerPart1();
					{
						glPushMatrix();
							glTranslatef(-1.52, -1.12, 0.2);
							glRotatef(fingerAngle4, 0.0, 0.0, 1.0);
							glTranslatef(1.52, 1.12, -0.2);
							right4FingerPart2();
							{
								glPushMatrix();
									glTranslatef(-1.44, -1.36, 0.2);
									glRotatef(fingerAngle4, 0.0, 0.0, 1.0);
									glTranslatef(1.44, 1.36, -0.2);
									right4FingerPart3();
								glPopMatrix();
							}
						glPopMatrix();
					}
				glPopMatrix();
				endTexture();
			glPopMatrix();

			break;
		case 1:
			glPushMatrix();
				glTranslatef(-1.55, 0.1, 0.0);
				drawWeapon(1);
			glPopMatrix();
			break;
		case 2:
			glPushMatrix();
				glTranslatef(-1.55, -0.9, 0.0);
				drawWeapon(2);
			glPopMatrix();
			break;
		case 3:
			glPushMatrix();
				glTranslatef(-1.55, -1.0, 0.0);
				drawWeapon(3);
			glPopMatrix();
			break;
		case 4:
			glPushMatrix();
				glTranslatef(-1.55, -0.6, 0.0);
				glRotatef(-90, 0.0, 1.0, 0.0);
				glRotatef(-90, 0.0, 0.0, 1.0);
				drawWeapon(4);
			glPopMatrix();
			break;
		default:
			break;
	}


}


void drawLeftLegThigh()
{
//Inner part
	glPushMatrix();
	loadBitmapImage(innerTextureArray[textureSetIndex].data());
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
			glTranslatef(0.689, -1.62, 0.106);
			glScalef(0.24, 0.7, 0.36);
			drawFilledCube1();
		glPopMatrix();

		endTexture();
	glPopMatrix();

//Outside part
	//1 (thigh)
	glPushMatrix();
	loadBitmapImage(armorTextureArray[textureSetIndex].data());
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
			glTranslatef(0.7, -1.36, 0.8);
			glScalef(0.2, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Thigh front left rectangle piece
			glTranslatef(1.0, -1.36, 0.68);
			glRotatef(53, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh left rectangle piece
			glTranslatef(1.11, -1.36, 0.15);
			glScalef(0.02, 0.75, 0.4);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh back left rectangle piece
			glTranslatef(0.91, -1.36, -0.3);
			glRotatef(75, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.7, 0.2);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Thigh front right rectangle piece
		glTranslatef(0.4, -1.36, 0.68);
			glRotatef(-53, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh left rectangle piece
			glTranslatef(0.31, -1.36, 0.15);
			glScalef(0.02, 0.75, 0.4);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh back left rectangle piece
			glTranslatef(0.51, -1.36, -0.3);
			glRotatef(-75, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.7, 0.2);
			drawFilledCube();
		glPopMatrix();

		//3  (thigh)
		glPushMatrix();     //Top steep cube
			glTranslatef(0.7, -2.27, 0.68);
			glRotatef(40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.2, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();    //Left small Triangle piece
			glTranslatef(0.9, -2.17, 0.72);
			glRotatef(-140, 1.0, 0.0, 0.0);
			glRotatef(-40, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.3, 0.4);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();    //Right small Triangle piece
			glTranslatef(0.465, -2.205, 0.78);
			glRotatef(-140, 1.0, 0.0, 0.0);
			glRotatef(50, 0.0, 1.0, 0.0);
			glRotatef(180, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.3, 0.4);
			drawFilledTriangle();
		glPopMatrix();

		endTexture();
		glPopMatrix();
}

void drawLeftLegKneeShin()
{
//Inner part
	glPushMatrix();
	loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();       //Left Joint
			glTranslatef(0.42, -2.57, 0.1);
			glRotatef(90, 0.0, 1.0, 0.0);
			drawCylinder(0.25, 0.25, 0.55, 30, 30);
		glPopMatrix();

		glPushMatrix();          //Left thick cube after joint
			glTranslatef(0.689, -3.65, 0.106);
			glScalef(0.24, 0.85, 0.36);
			drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	//4   (knee)
	glPushMatrix();
	loadBitmapImage(armorTextureArray[textureSetIndex].data());
		glPushMatrix();      //Front piece
			glTranslatef(0.7, -2.6, 0.50);
			glRotatef(15, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.15, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();      //Left front piece
			glTranslatef(0.93, -2.58, 0.42);
			glRotatef(15, 1.0, 0.0, 0.0);
			glRotatef(-28, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.09);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();      //Left piece
			glTranslatef(1.02, -2.53, 0.12);
			glRotatef(8, 1.0, 0.0, 0.0);
			glRotatef(-10, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.3);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();      //Right front piece
			glTranslatef(0.463, -2.58, 0.42);
			glRotatef(15, 1.0, 0.0, 0.0);
			glRotatef(28, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.09);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();      //Right piece
			glTranslatef(0.37, -2.53, 0.12);
			glRotatef(8, 1.0, 0.0, 0.0);
			glRotatef(10, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.3);
			drawFilledCube();
		glPopMatrix();


		//5 (shin)
		glPushMatrix();     //Top steep cube
			glTranslatef(0.7, -2.9, 0.56);
			glRotatef(-40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.15, 0.02);
			drawFilledCube();
		glPopMatrix();

		//6  (shin)
		glPushMatrix();         //Shin front rectangle piece
			glTranslatef(0.7, -3.65, 0.66);
			glScalef(0.2, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Shin front left rectangle piece
			glTranslatef(0.93, -3.65, 0.53);
			glRotatef(58, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Shin left rectangle piece
			glTranslatef(1.02, -3.75, 0.1);
			glScalef(0.02, 0.75, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();          //Left behind rocket
			glTranslatef(0.9, -3.6, -0.42);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.15, 0.15, 0.7, 30, 30);
		glPopMatrix();

		glPushMatrix();         //Shin front right rectangle piece
			glTranslatef(0.468, -3.65, 0.53);
			glRotatef(-58, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Shin right rectangle piece
			glTranslatef(0.38, -3.75, 0.1);
			glScalef(0.02, 0.75, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();          //Right behind rocket
			glTranslatef(0.5, -3.6, -0.42);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.15, 0.15, 0.7, 30, 30);
		glPopMatrix();

	endTexture();
	glPopMatrix();
}

void drawLeftLegAnkle()
{
//Inner part
	glPushMatrix();
	loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();          //Left ankle
			glColor3f(0.0, 0.0, 0.0);
			glTranslatef(0.69, -4.512, 0.02);
			glRotatef(45, 0.0, 1.0, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.347, 0.1, 0.3, 4, 4);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.878, -4.796, 0.3);
			glRotatef(90, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.28, 4.0);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.68, -4.66, 0.48);
			glScalef(0.2, 0.15, 0.18);
			drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
	loadBitmapImage(armorTextureArray[textureSetIndex].data());
//7  (ankle)
		glPushMatrix();     //Top steep cube
			glTranslatef(0.7, -4.4, 0.742);
			glRotatef(-40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.12, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();     //Second steep cube
			glTranslatef(0.7, -4.55, 0.94);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.14, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Second steep left triangle
			glTranslatef(0.799, -4.52, 0.81);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glRotatef(180, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.27, 0.4);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();     //Second steep right triangle
			glTranslatef(0.6, -4.479, 0.83);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(180, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.27, 0.4);
			drawFilledTriangle();
		glPopMatrix();

		glPushMatrix();     //Third front cube
			glTranslatef(0.7, -4.71, 1.06);
			glScalef(0.1, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third front bottom steep cube
			glTranslatef(0.7, -4.84, 0.86);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.02, 0.21);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Bottom cube
			glTranslatef(0.69, -4.865, 0.35);
			glScalef(0.215, 0.02, 0.3);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();     //Third front left cube
			glTranslatef(0.852, -4.71, 0.885);
			glRotatef(70, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third left side cube
			glTranslatef(0.92, -4.74, 0.4);
			glScalef(0.03, 0.13, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third left back cube
			glTranslatef(0.955, -4.69, 0.06);
			glScalef(0.04, 0.172, 0.28);
			drawFilledCube();
		glPopMatrix();


		glPushMatrix();     //Third front right cube
			glTranslatef(0.54, -4.71, 0.885);
			glRotatef(-67, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third right side cube
			glTranslatef(0.46, -4.74, 0.4);
			glScalef(0.03, 0.13, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third right back cube
			glTranslatef(0.43, -4.69, 0.06);
			glScalef(0.04, 0.172, 0.28);
			drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();

}


void drawRightLegThigh()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		//Right leg
		glPushMatrix();             //Two Horizontal cube
			glTranslatef(-0.55, -0.28, 0.4);
			glScalef(0.38, 0.08, 0.08);
			drawFilledCube1();
		glPopMatrix();

		glPushMatrix();
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
			glTranslatef(-0.689, -1.62, 0.106);
			glScalef(0.24, 0.7, 0.36);
			drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
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
			glTranslatef(-0.7, -1.36, 0.8);
			glScalef(0.2, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Thigh front left rectangle piece
			glTranslatef(-0.4, -1.36, 0.68);
			glRotatef(53, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh left rectangle piece
			glTranslatef(-0.31, -1.36, 0.15);
			glScalef(0.02, 0.75, 0.4);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh back left rectangle piece
			glTranslatef(-0.51, -1.36, -0.3);
			glRotatef(75, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.7, 0.2);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Thigh front right rectangle piece
			glTranslatef(-1.0, -1.36, 0.68);
			glRotatef(-53, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.75, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh left rectangle piece
			glTranslatef(-1.11, -1.36, 0.15);
			glScalef(0.02, 0.75, 0.4);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Thigh back left rectangle piece
			glTranslatef(-0.91, -1.36, -0.3);
			glRotatef(-75, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.7, 0.2);
			drawFilledCube();
		glPopMatrix();

		//3  (thigh)
		glPushMatrix();     //Top steep cube
			glTranslatef(-0.7, -2.27, 0.68);
			glRotatef(40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.2, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();    //Left small Triangle piece
			glTranslatef(-0.5, -2.17, 0.72);
			glRotatef(-140, 1.0, 0.0, 0.0);
			glRotatef(-40, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.3, 0.4);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();    //Right small Triangle piece
			glTranslatef(-0.935, -2.205, 0.78);
			glRotatef(-140, 1.0, 0.0, 0.0);
			glRotatef(50, 0.0, 1.0, 0.0);
			glRotatef(180, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.3, 0.4);
			drawFilledTriangle();
		glPopMatrix();

	endTexture();
	glPopMatrix();
}

void drawRightLegKneeShin()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();       //Right Joint
			glTranslatef(-0.42, -2.57, 0.1);
			glRotatef(-90, 0.0, 1.0, 0.0);
			drawCylinder(0.25, 0.25, 0.55, 30, 30);
		glPopMatrix();

		glPushMatrix();          //Right thick cube after joint
			glTranslatef(-0.689, -3.65, 0.106);
			glScalef(0.24, 0.85, 0.36);
			drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		//4  (knee)
		glPushMatrix();      //Front piece
			glTranslatef(-0.7, -2.6, 0.50);
			glRotatef(15, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.15, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();      //Left front piece
			glTranslatef(-0.463, -2.58, 0.42);
			glRotatef(15, 1.0, 0.0, 0.0);
			glRotatef(-28, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.09);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();      //Left piece
			glTranslatef(-0.37, -2.53, 0.12);
			glRotatef(8, 1.0, 0.0, 0.0);
			glRotatef(-10, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.3);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();      //Right front piece
			glTranslatef(-0.93, -2.58, 0.42);
			glRotatef(15, 1.0, 0.0, 0.0);
			glRotatef(28, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.09);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();      //Right piece
			glTranslatef(-1.02, -2.53, 0.12);
			glRotatef(8, 1.0, 0.0, 0.0);
			glRotatef(10, 0.0, 1.0, 0.0);
			glScalef(0.02, 0.15, 0.3);
			drawFilledCube();
		glPopMatrix();

		//5   (shin)
		glPushMatrix();     //Top steep cube
			glTranslatef(-0.7, -2.9, 0.56);
			glRotatef(-40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.15, 0.02);
			drawFilledCube();
		glPopMatrix();

		//6  (shin)
		glPushMatrix();         //Shin front rectangle piece
			glTranslatef(-0.7, -3.65, 0.66);
			glScalef(0.2, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();         //Shin front left rectangle piece
			glTranslatef(-0.47, -3.65, 0.53);
			glRotatef(58, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Shin left rectangle piece
			glTranslatef(-0.37, -3.75, 0.1);
			glScalef(0.02, 0.75, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();          //Left behind rocket
			glTranslatef(-0.5, -3.6, -0.42);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.15, 0.15, 0.7, 30, 30);
		glPopMatrix();

		glPushMatrix();         //Shin front right rectangle piece
			glTranslatef(-0.928, -3.65, 0.53);
			glRotatef(-58, 0.0, 1.0, 0.0);
			glScalef(0.16, 0.65, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();         //Shin right rectangle piece
			glTranslatef(-1.01, -3.75, 0.1);
			glScalef(0.02, 0.75, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();          //Right behind rocket
			glTranslatef(-0.9, -3.6, -0.42);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.15, 0.15, 0.7, 30, 30);
		glPopMatrix();

	endTexture();
	glPopMatrix();

}

void drawRightLegAnkle()
{
//Inner part
	glPushMatrix();
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		glPushMatrix();          //Right ankle
			glTranslatef(-0.69, -4.512, 0.02);
			glRotatef(45, 0.0, 1.0, 0.0);
			glRotatef(90, 1.0, 0.0, 0.0);
			drawCylinder(0.347, 0.1, 0.3, 4, 4);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.478, -4.796, 0.3);
			glRotatef(90, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.28, 4.0);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.68, -4.66, 0.48);
			glScalef(0.2, 0.15, 0.18);
			drawFilledCube1();
		glPopMatrix();

	endTexture();
	glPopMatrix();

//Outside part
	glPushMatrix();
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		//7  (ankle)
		glPushMatrix();     //Top steep cube
			glTranslatef(-0.7, -4.4, 0.742);
			glRotatef(-40, 1.0, 0.0, 0.0);
			glScalef(0.2, 0.12, 0.02);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();     //Second steep cube
			glTranslatef(-0.7, -4.55, 0.94);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.14, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Second steep left triangle
			glTranslatef(-0.6, -4.52, 0.81);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glRotatef(180, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.27, 0.4);
			drawFilledTriangle();
		glPopMatrix();
		glPushMatrix();     //Second steep right triangle
			glTranslatef(-0.8, -4.479, 0.83);
			glRotatef(-65, 1.0, 0.0, 0.0);
			glRotatef(180, 0.0, 1.0, 0.0);
			glRotatef(180, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.27, 0.4);
			drawFilledTriangle();
		glPopMatrix();

		glPushMatrix();     //Third front cube
			glTranslatef(-0.7, -4.71, 1.06);
			glScalef(0.1, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third front bottom steep cube
			glTranslatef(-0.7, -4.84, 0.86);
			glRotatef(-10, 1.0, 0.0, 0.0);
			glScalef(0.1, 0.02, 0.21);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Bottom cube
		glTranslatef(-0.69, -4.865, 0.35);
		glScalef(0.215, 0.02, 0.3);
		drawFilledCube();
		glPopMatrix();

		glPushMatrix();     //Third front left cube
			glTranslatef(-0.54, -4.71, 0.885);
			glRotatef(67, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third left side cube
			glTranslatef(-0.46, -4.74, 0.4);
			glScalef(0.03, 0.13, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third left back cube
			glTranslatef(-0.43, -4.69, 0.06);
			glScalef(0.04, 0.172, 0.28);
			drawFilledCube();
		glPopMatrix();

		glPushMatrix();     //Third front right cube
			glTranslatef(-0.851, -4.71, 0.885);
			glRotatef(-70, 0.0, 1.0, 0.0);
			glScalef(0.2, 0.1, 0.02);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third right side cube
			glTranslatef(-0.92, -4.74, 0.4);
			glScalef(0.03, 0.13, 0.3);
			drawFilledCube();
		glPopMatrix();
		glPushMatrix();     //Third right back cube
			glTranslatef(-0.955, -4.69, 0.06);
			glScalef(0.04, 0.172, 0.28);
			drawFilledCube();
		glPopMatrix();

	endTexture();
	glPopMatrix();
}


void drawBackWing(int backType)
{
	switch (backType)
	{
		case 0:
			robotExhaustJet = false;
			break;
		case 1:
		//Robot wings
		//Left side
			loadBitmapImage(wingSecTextureArray[textureSetIndex].data());
			glPushMatrix();          // Piece stick to body
				glTranslatef(0.5, 1.9, -0.65);
				glScalef(0.3, 0.7, 0.03);
				drawFilledCube();
			glPopMatrix();
			//1
			glPushMatrix();           //Cube piece
				glTranslatef(0.8, 1.5, -0.7);
				glScalef(0.5, 0.6, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();           //Top Triangle piece
				glTranslatef(1.3, 2.1, -0.72);
				glRotatef(180, 0.0, 1.0, 0.0);
				glScalef(1.0, 1.1, 0.4);
				drawFilledTriangle();
			glPopMatrix();
			//2
			glPushMatrix();           //Cube piece
				glTranslatef(1.8, 2.6, -0.7);
				glScalef(0.5, 0.6, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();           //Bottom Triangle piece
				glTranslatef(1.3, 2.0, -0.72);
				glRotatef(180, 1.0, 0.0, 0.0);
				glScalef(1.0, 2.1, 0.4);
				drawFilledTriangle();
			glPopMatrix();
			endTexture();

			loadBitmapImage(wingTextureArray[textureSetIndex].data());
			//3  (wings) (face down)
			glPushMatrix();          //first
				glTranslatef(3.18, 2.1, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.5, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //second
				glTranslatef(2.95, 1.75, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.40, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //third
				glTranslatef(2.72, 1.4, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.3, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fourth
				glTranslatef(2.49, 1.05, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.2, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fifth
				glTranslatef(2.2, 0.75, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.1, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();

			//3  (wings) (face up)
			glPushMatrix();          //first
				glTranslatef(2.0, 3.9, -0.7);
				glRotatef(60, 0.0, 0.0, 1.0);
				glScalef(1.5, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //second
				glTranslatef(2.2, 3.5, -0.7);
				glRotatef(54, 0.0, 0.0, 1.0);
				glScalef(1.4, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //third
				glTranslatef(2.4, 3.1, -0.7);
				glRotatef(50, 0.0, 0.0, 1.0);
				glScalef(1.3, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fourth
				glTranslatef(2.6, 2.7, -0.7);
				glRotatef(46, 0.0, 0.0, 1.0);
				glScalef(1.2, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fifth
				glTranslatef(2.8, 2.3, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.1, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			endTexture();


		//Right side
			loadBitmapImage(wingSecTextureArray[textureSetIndex].data());
			glPushMatrix();          // Piece stick to body
				glTranslatef(-0.5, 1.9, -0.65);
				glScalef(0.3, 0.7, 0.03);
				drawFilledCube();
			glPopMatrix();
			//1
			glPushMatrix();           //Cube piece
				glTranslatef(-0.8, 1.5, -0.7);
				glScalef(0.5, 0.6, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();           //Triangle piece
				glTranslatef(-1.3, 2.1, -0.68);
				glScalef(1.0, 1.1, 0.4);
				drawFilledTriangle();
			glPopMatrix();
			//2
			glPushMatrix();           //Cube piece
				glTranslatef(-1.8, 2.6, -0.7);
				glScalef(0.5, 0.6, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();           //Bottom Triangle piece
				glTranslatef(-1.3, 2.0, -0.68);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(180, 1.0, 0.0, 0.0);
				glScalef(1.0, 2.1, 0.4);
				drawFilledTriangle();
			glPopMatrix();
			endTexture();

			//3  (wings) (face down)
			loadBitmapImage(wingTextureArray[textureSetIndex].data());
			glPushMatrix();          //first
				glTranslatef(-3.18, 2.1, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.5, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //second
				glTranslatef(-2.95, 1.75, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.40, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //third
				glTranslatef(-2.72, 1.4, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.3, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fourth
				glTranslatef(-2.49, 1.05, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.2, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fifth
				glTranslatef(-2.2, 0.75, -0.7);
				glRotatef(40, 0.0, 0.0, 1.0);
				glScalef(1.1, 0.2, 0.02);
				drawFilledCube();
			glPopMatrix();

			//3  (wings) (face up)
			glPushMatrix();          //first
				glTranslatef(-2.0, 3.9, -0.7);
				glRotatef(-60, 0.0, 0.0, 1.0);
				glScalef(1.5, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //second
				glTranslatef(-2.2, 3.5, -0.7);
				glRotatef(-54, 0.0, 0.0, 1.0);
				glScalef(1.4, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //third
				glTranslatef(-2.4, 3.1, -0.7);
				glRotatef(-50, 0.0, 0.0, 1.0);
				glScalef(1.3, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fourth
				glTranslatef(-2.6, 2.7, -0.7);
				glRotatef(-46, 0.0, 0.0, 1.0);
				glScalef(1.2, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			glPushMatrix();          //fifth
				glTranslatef(-2.8, 2.3, -0.7);
				glRotatef(-40, 0.0, 0.0, 1.0);
				glScalef(1.1, 0.25, 0.02);
				drawFilledCube();
			glPopMatrix();
			endTexture();
			break;
		case 2:
		//Robot exhaust jet
		//Middle
			//Inside two stick to body cube
			loadBitmapImage(wingSecTextureArray[textureSetIndex].data());
			glPushMatrix();
				glTranslatef(0.45, 1.5, -0.6);
				glScalef(0.2, 0.9, 0.2);
				drawFilledCube();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(-0.45, 1.5, -0.6);
				glScalef(0.2, 0.9, 0.2);
				drawFilledCube();
			glPopMatrix();

			glPushMatrix();        //Middle box
				glTranslatef(0.0, 1.8, -1.0);
				glScalef(1.0, 1.1, 0.3);
				drawFilledCube();
			glPopMatrix();
			endTexture();

			//Left wings
			robotExhaustJet = true;

			glPushMatrix();
			loadBitmapImage(wingTextureArray[textureSetIndex].data());
			glTranslatef(wingLeftTranslateDegree, 0.0, 0.0);
			{
				//Left wings
				glPushMatrix();
				glTranslatef(0.0, 2.5, -1.0);
				glScalef(2.0, 0.4, 0.1);
				drawFilledCube();
				glPopMatrix();
				glPushMatrix();
				glTranslatef(-2.0, 2.1, -1.1);
				glRotatef(180, 1.0, 0.0, 0.0);
				glScalef(3.6, 1.2, 2.0);
				drawFilledTriangle();
				glPopMatrix();
			}
			endTexture();
			glPopMatrix();

			glPushMatrix();              //Rocket
			loadBitmapImage("textureImage/rocket.bmp");
			glTranslatef(wingLeftRocketTranslateDegree, 0.0, 0.0);
			{

				glPushMatrix();
				glTranslatef(0.0, 0.8, -1.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(1.5, 1.0, 1.0);
				drawCylinder(0.15, 0.30, 1.0, 50, 50);
				glPopMatrix();
			}
			endTexture();
			glPopMatrix();

				//Right wings
			glPushMatrix();
			loadBitmapImage(wingTextureArray[textureSetIndex].data());
			glTranslatef(wingRightTranslateDegree, 0.0, 0.0);
			{
				glPushMatrix();
				glTranslatef(0.0, 2.5, -1.0);
				glScalef(2.0, 0.4, 0.1);
				drawFilledCube();
				glPopMatrix();
				glPushMatrix();
				glTranslatef(2.0, 2.1, -0.9);
				glRotatef(180, 0.0, 1.0, 0.0);
				glRotatef(180, 1.0, 0.0, 0.0);
				glScalef(3.6, 1.2, 2.0);
				drawFilledTriangle();
				glPopMatrix();
			}
			endTexture();
			glPopMatrix();

			glPushMatrix();              //Rocket
			loadBitmapImage("textureImage/rocket.bmp");
			glTranslatef(wingRightRocketTranslateDegree, 0.0, 0.0);
			{
				glPushMatrix();
				glTranslatef(0.0, 0.8, -1.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				glScalef(1.5, 1.0, 1.0);
				drawCylinder(0.15, 0.30, 1.0, 50, 50);
				glPopMatrix();
			}
			endTexture();
			glPopMatrix();
			break;
		default:
			break;
	}
}

void electricEffect() {
	glPushMatrix();
	loadBitmapImage("textureImage/lightning.bmp");
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef(90, 1.0, 0.0, 0.0);
	GLUquadricObj *disk = NULL;
	disk = gluNewQuadric();
	gluQuadricTexture(disk, GLU_TRUE);    //gluCylinder(GLUquadric obj *, baseRadius,topRadius, height,slices, stacks);

	glPushMatrix();
	glRotatef(electricEffectMovement, 0, 0, 1);
	gluDisk(disk, 0.0f, 0.2, 30, 30);
	glPopMatrix();

	gluDeleteQuadric(disk);
	endTexture();
	glPopMatrix();
}

void environment() {
	glTranslatef(0.0, 5.0, 0.0);
	glScalef(30.0, 10.0, 30.0);
	glPushMatrix();
		loadBitmapImage(environmentTopTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Top Face
			glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
		}
		glEnd();
		endTexture();
	glPopMatrix();

	glPushMatrix();
		loadBitmapImage(environmentLeftTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Left Face
			glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.0f, -1.0f);
			glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
		}
		glEnd();
		endTexture();
	glPopMatrix();

	glPushMatrix();
		loadBitmapImage(environmentBackTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Back Face
			glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, -1.0f);
			glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, -1.0f);

		}
		glEnd();
		endTexture();
	glPopMatrix();

	glPushMatrix();
	loadBitmapImage(environmentRightTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Right Face
			glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, -1.0f);
			glTexCoord2f(0, 0); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
		}
		glEnd();
		endTexture();
	glPopMatrix();

	glPushMatrix();
		loadBitmapImage(environmentBottomTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Bottom Face
			glTexCoord2f(0, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
			glTexCoord2f(1, 0); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1, 1); glVertex3f(1.0f, -1.0f, -1.0f);
			glTexCoord2f(0, 1); glVertex3f(-1.0f, -1.0f, -1.0f);
		}
		glEnd();
		endTexture();
	glPopMatrix();

	glPushMatrix();
		loadBitmapImage(environmentFrontTextureArray[textureEnvironmentIndex].data());
		glBegin(GL_QUADS);
		{
			// Front Face
			glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 1); glVertex3f(1.0f, 1.0f, 1.0f);
			glTexCoord2f(0, 0); glVertex3f(1.0f, -1.0f, 1.0f);
			glTexCoord2f(1, 0); glVertex3f(-1.0f, -1.0f, 1.0f);
		}
		glEnd();
		endTexture();
	glPopMatrix();

}

void drawSunOrMoon() {
	glPushMatrix();
	glTranslatef(light_position[0], light_position[1], light_position[2]);
		if (dayTime) {
			loadBitmapImage("textureImage/sun.bmp");
		}
		else {
			loadBitmapImage("textureImage/moon.bmp");
		}
		drawSphere(0.02, 30, 30);
		endTexture();
	glPopMatrix();
}

void jaegerRobot()
{
//Body

	glRotatef(90, 1.0, 0.0, 0.0);

	glPushMatrix();  //First push pop
	glRotatef(robotMoveRight, 0.0, 1.0, 0.0);
	glRotatef(robotMoveLeft, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, robotMoveFront);

	glPushMatrix();
	glTranslatef(0.0, 0.0, robotUpperBodyTranslate);
//Inner part of robot
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(innerTextureArray[textureSetIndex].data());
		drawInnerRobotPart();
		endTexture();
	glPopMatrix();

	//Head part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		drawHeadArmour();
		endTexture();
	glPopMatrix();

//Backbone part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/chrome_backbone.bmp");
		drawBackBone();
		endTexture();
	glPopMatrix();

//Back wing or Exhaust
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		drawBackWing(backType);
	glPopMatrix();

// Armor part
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		drawFrontArmour();
		endTexture();
	glPopMatrix();

// Reactor
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(reactorTextureArray[textureReactorIndex].data()	);
		drawReactor();
		endTexture();
	glPopMatrix();

	//Fireball
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(reactorTextureArray[textureReactorIndex].data());
		glTranslatef(0.0, 0.0, reactorFireballTranslate);
		drawReactorFireball();
		endTexture();
	glPopMatrix();



// Reactor shell
	/*glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage("textureImage/.bmp");
		drawReactorShell();
		endTexture();
	glPopMatrix();*/

//Front Belly
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		loadBitmapImage(armorTextureArray[textureSetIndex].data());
		drawFrontBelly();
		endTexture();
	glPopMatrix();

//Hand part
	//Left
	glPushMatrix();
	glTranslatef(1.55, 2.8, 0.0);
	glRotatef(LeftHandWholeArmAngle, 1.0, 0.0, 0.0);
	glRotatef(LeftHandWholeArmAnglez, 0.0, 0.0, 1.0);
	glTranslatef(-1.55, -2.8, 0.0);
	drawLeftShoulderArm();
	{
		glPushMatrix();
		glTranslatef(1.3, 1.2, 0.0);
		glRotatef(LeftHandForearmAngle, 1.0, 0.0, 0.0);
		glRotatef(LeftHandForearmAnglez, 0.0, 1.0, 0.0);
		glTranslatef(-1.3, -1.2, 0.0);
		drawLeftElbowForearm();
		{
			glPushMatrix();
			glTranslatef(1.55, -0.20, -0.0);
			glRotatef(leftWristRotatey, 0.0, 1.0, 0.0);
			glRotatef(LeftHandPalmAngle, 0.0, 0.0, 1.0);
			glTranslatef(-1.55, 0.20, 0.0);
			drawLeftHandPalm();
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

	//Right
	glPushMatrix();
	glTranslatef(-1.55, 2.8, 0.0);
	glRotatef(RightHandWholeArmAngle, 1.0, 0.0, 0.0);
	glRotatef(RightHandWholeArmAnglez, 0.0, 0.0, 1.0);
	glTranslatef(1.55, -2.8, 0.0);
	drawRightShoulderArm();
	{
		glPushMatrix();
		glTranslatef(-1.3, 1.2, 0.0);
		glRotatef(RightHandForearmAngle, 1.0, 0.0, 0.0);
		glRotatef(RightHandForearmAnglez, 0.0, 1.0, 0.0);
		glTranslatef(1.3, -1.2, 0.0);
		drawRightElboxForearm();

		{
			glPushMatrix();
			glRotatef(RightHandPalmAngle, 1.0, 0.0, 0.0);
			drawRightHandPalm();
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();
//Leg part
//Left
	glPushMatrix();
	glTranslatef(0.0, 0.0, robotLeftLegMoveWithBody);
	glTranslatef(0.5, -1.0, 0.0);
	glRotatef(LeftLegThighAngle, 1.0, 0.0, 0.0);
	glTranslatef(-0.5, 1.0, 0.0);
	drawLeftLegThigh();
	{


		glPushMatrix();
		glTranslatef(0.42, -2.57, 0.1);
		glRotatef(LeftLegKneeShinAngle, 1.0, 0.0, 0.0);
		glTranslatef(-0.42, 2.57, -0.1);
		//glTranslatef(0.0, 0.0, LeftLegKneeShinAngle1);
		//glTranslatef(0.0, LeftLegKneeShinAngle, 0.0);
		drawLeftLegKneeShin();
		{
			glPushMatrix();
			glTranslatef(0.5, -4.5, 0.0);
			glRotatef(LeftLegAnkleAngle, 1.0, 0.0, 0.0);
			glTranslatef(-0.5, 4.5, 0.0);
			drawLeftLegAnkle();
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();

//Right
	glPushMatrix();
	glTranslatef(0.0, 0.0, robotRightLegMoveWithBody);
	glTranslatef(0.5, -1.0, 0.0);
	glRotatef(RightLegThighAngle, 1.0, 0.0, 0.0);
	glTranslatef(-0.5, 1.0, 0.0);
	drawRightLegThigh();
	{
		glPushMatrix();
		glTranslatef(-0.42, -2.57, 0.1);
		glRotatef(RightLegKneeShinAngle, 1.0, 0.0, 0.0);
		glTranslatef(0.42, 2.57, -0.1);
		drawRightLegKneeShin();
		{
			glPushMatrix();
			glTranslatef(0.5, -4.5, 0.0);
			glRotatef(RightLegAnkleAngle, 1.0, 0.0, 0.0);
			glTranslatef(-0.5, 4.5, 0.0);
			drawRightLegAnkle();
			glPopMatrix();

		}
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();   //First push pop
//	//Testing Area

}

void initLighting() {
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);

	if (dayTime) {
		glEnable(GL_LIGHT0);
	}
	else {
		glDisable(GL_LIGHT0);
	}

	if (ambientOn) {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	}
	else {
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_close);
	}

	if (diffuseOn) {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	}
	else {
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_close);
	}

	if (specularOn) {
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	}
	else {
		glLightfv(GL_LIGHT0, GL_SPECULAR, light_close);
	}


	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	initLighting();

	glTranslatef(0.0, 0.0, zoomLevel);

	glRotatef(-90, 1.0, 0.0, 0.0);                    //Must turn it 90 degree so can move using mouse

	glRotatef(xRotated, 1.0, 0.0, 0.0);

	glRotatef(yRotated, 0.0, 1.0, 0.0);

	glRotatef(zRotated, 0.0, 0.0, 1.0);


	GLfloat no_shininess[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat mat_emission[] = { 0.5f, 0.5f, 0.5f, 0.0f };

	glColor3f(0.0f, 0.0f, 0.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 25.0);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

	jaegerRobot();

	environment();

	drawSunOrMoon();

	if (drawThorHammer == true) {
		electricEffect();
		textureSetIndex = 3;
		textureReactorIndex = 1;
		textureEnvironmentIndex = 3;
	
	}

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
	gluPerspective(70.0, 1, 0.1, 100.0);


	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (reactorRotateAngle < 360) {
			reactorRotateAngle += 30.0f;
		}
		else {
			reactorRotateAngle = 0.0f;
		}

		if (electricEffectMovement < 360) {
			electricEffectMovement += 0.000005f;
		}
		else {
			electricEffectMovement = 0.0f;
		}

		if (reactorFireball == true)
		{
			reactorFireballTranslate += 0.3;
		}
		else if (reactorFireball == false)
		{
			reactorFireballTranslate = 0.0;
		}

		if (drillRotate < 360) {
			drillRotate += 50.0f;
		}
		else {
			drillRotate = 0.0f;
		}

		if (gunBarrelRotate < 360) {
			gunBarrelRotate += 100.0f;
		}
		else {
			gunBarrelRotate = 0.0f;
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
//3. slow down robot moving speed
