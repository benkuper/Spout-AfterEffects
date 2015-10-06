#include <Spout.h>

HWND hWnd;

bool InitGLtexture(GLuint &texID, unsigned int width, unsigned int height)
{
	// printf("testApp::InitGLtexture %dx%d\n", width, height);

	if(texID != 0) glDeleteTextures(1, &texID);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}


void bitmapDataToGL(GLuint &texID, int width, int height, GLvoid * pixels)
{
	glBindTexture(GL_TEXTURE_2D, texID);
	

	glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA, width, height, 0,GL_BGRA_EXT, GL_UNSIGNED_BYTE,pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //  GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //  GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


void GLToBitmapData(GLuint &texID, int width, int height, GLvoid * pixels)
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glGetTexImage(GL_TEXTURE_2D, 0,  GL_BGRA_EXT,  GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D,1);
}


//GL INIT / CONTEXT CREATION
BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
	DWORD windowID;
	GetWindowThreadProcessId(hwnd, &windowID);

	if (windowID == lParam)
	{
		printf("Found HWND !\n");
		hWnd = hwnd;

		return false;
	}

	return true;
}


void EnableOpenGL(HWND hWnd, HDC * hDC, HGLRC * hRC)
{
    PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    // get the device context (DC)
    *hDC = GetDC( hWnd );

    // set the pixel format for the DC
    ZeroMemory( &pfd, sizeof( pfd ) );
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                  PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;
    iFormat = ChoosePixelFormat( *hDC, &pfd );
    SetPixelFormat( *hDC, iFormat, &pfd );

    // create and enable the render context (RC)
    *hRC = wglCreateContext( *hDC );
    wglMakeCurrent( *hDC, *hRC );
}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent( NULL, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );
}


bool InitGL()						// All Setup For OpenGL Goes Here
{

	printf("InitGL with EnabledOpenGL\n");
			
	//set hWnd with process ID
	DWORD processID = GetCurrentProcessId();
	EnumWindows(EnumProc, processID);


	HDC hDC;
	HGLRC glContext;
	EnableOpenGL(hWnd,&hDC,&glContext);

	// Determine hardware capabilities now, not later when all is initialized
	// =====================================================
	//glContext = wglGetCurrentContext(); // should check if opengl context creation succeed
	if(glContext) {
		printf("GL Context creation OK\n");
	}
	else {

		printf("No GL context, GetLastError : %X\n",GetLastError());

		return false;
	}

	printf("And we are there !\n");

	return true;										// Initialization Went OK

}
