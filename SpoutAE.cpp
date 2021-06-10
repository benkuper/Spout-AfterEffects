/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "SpoutAE.h"
#include <cmath>

#ifdef AE_OS_WIN 
BOOL APIENTRY LibMain(HANDLE hInstance, DWORD fdwReason, LPVOID lpReserved)
{
	printf("Spout.aex Initialization entry point.");
	return TRUE;
}
#endif

static PF_Err
MyBlit(
	void						*hook_refconPV,
	const AE_PixBuffer			*pix_buf,
	const AE_ViewCoordinates	*viewP,
	AE_BlitReceipt				receipt,
	AE_BlitCompleteFunc			complete_func0,
	AE_BlitInFlags				in_flags,
	AE_BlitOutFlags				*out_flags)
{

	bool shouldUpdate = false;

	if (pix_buf->widthL != (tWidth) || pix_buf->heightL != (tHeight)) shouldUpdate = true;
	tWidth = pix_buf->widthL;
	tHeight = pix_buf->heightL;


	if (!bSenderInitialized)
	{
		printf("Create sender\n");
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_hRC;
		HGLRC m_hSharedRC;
		bool bOpenGL = false;

		m_hwnd = CreateWindow(L"BUTTON", L"VDJ Sender", WS_OVERLAPPEDWINDOW | CS_OWNDC, 0, 0, 32, 32, NULL, NULL, NULL, NULL);

		if (!m_hwnd)
		{
			printf("InitOpenGL error 1\n");
			MessageBoxA(NULL, "Error 1\n", "InitOpenGL", MB_OK);
		}

		m_hdc = GetDC(m_hwnd);
		if (!m_hdc)
		{
			printf("InitOpenGL error 2\n");
			MessageBoxA(NULL, "Error 2\n", "InitOpenGL", MB_OK);
		}

		PIXELFORMATDESCRIPTOR pfd;
		ZeroMemory(&pfd, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;
		pfd.iLayerType = PFD_MAIN_PLANE;
		int iFormat = ChoosePixelFormat(m_hdc, &pfd);
		if (!iFormat)
		{
			printf("InitOpenGL error 3\n");
			MessageBoxA(NULL, "Error 3\n", "InitOpenGL", MB_OK);
		}

		if (!SetPixelFormat(m_hdc, iFormat, &pfd))
		{
			printf("InitOpenGL error 4\n");
			MessageBoxA(NULL, "Error 4\n", "InitOpenGL", MB_OK);
		}

		m_hRC = wglCreateContext(m_hdc);
		if (!m_hRC)
		{
			printf("InitOpenGL error 5\n");
			MessageBoxA(NULL, "Error 5\n", "InitOpenGL", MB_OK);
		}

		wglMakeCurrent(m_hdc, m_hRC);
		if (wglGetCurrentContext() == NULL)
		{
			printf("InitOpenGL error 6\n");
			MessageBoxA(NULL, "Error 6\n", "InitOpenGL", MB_OK);
		}

		m_hSharedRC = wglCreateContext(m_hdc);
		if (!m_hSharedRC) printf("InitOpenGL shared context not created\n");
		if (!wglShareLists(m_hSharedRC, m_hRC)) printf("wglShare Lists failed\n");

		//spoutSender = std::make_shared<SpoutSender>();
		std::cout << std::endl << std::endl << "Num Adapters:\t" << spoutsender.GetNumAdapters() << std::endl;
		std::cout << "Using Adapter:\t" << spoutsender.GetAdapter() << std::endl;
		std::cout << "using DX9?\t" << spoutsender.GetDX9() << std::endl << "using CPU?\t" << spoutsender.GetCPUmode() << std::endl << "GetShareMode():\t" << spoutsender.GetShareMode() << std::endl;
		bSenderInitialized = spoutsender.CreateSender(senderName, tWidth, tHeight);

		if (pixels2 != nullptr)
		{
			delete pixels2;
			pixels2 = (PF_Pixel8*)malloc(tWidth * tHeight * sizeof(PF_Pixel8));
		}

		/*
		bSenderInitialized = spoutsender.CreateSender(senderName, tWidth, tHeight);
		if (pixels2 != nullptr)
		{
			delete pixels2;
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}*/
	}
	else if (shouldUpdate)
	{
		printf("Update sender\n");
		spoutsender.ReleaseSender();
		bSenderInitialized = spoutsender.CreateSender(senderName, tWidth, tHeight);
		if (pixels2 != nullptr)
		{
			delete pixels2;
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}

	}
	

	if(bSenderInitialized)
	{
		PF_Pixel8 * pixels = (PF_Pixel8 *)pix_buf->pixelsPV;

		if (pixels2 == nullptr)
		{
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}
		 
		for (int y = 0; y<tHeight; y++)
		{
			for (int x = 0; x<tWidth; x++)
			{
				const unsigned char* src_ptr = (unsigned char*) pix_buf->pixelsPV;

				unsigned char r,g,b,a;
				// plane bytes aka bytes per channel tells us what channel format AE passes
				switch (pix_buf->plane_bytesL)
				{
					case 1:
					{
						// 8 bit unsigned char
						const unsigned char* src = (unsigned char*) (src_ptr + y*pix_buf->row_bytesL);
						b = src[x*4 + 0];
						g = src[x*4 + 1];
						r = src[x*4 + 2];
						a = src[x*4 + 3];
						break;
					}

					case 2:
					{
						// 16 bit unsigned char. AE uses a maximum channel value of 32768 instead of the full 2^16 - 1 = 65535
						const unsigned short* src = (unsigned short*) (src_ptr + y*pix_buf->row_bytesL);
						b = (unsigned char) round(src[x*4 + 0] * 255.f/32768.f);
						g = (unsigned char) round(src[x*4 + 1] * 255.f/32768.f);
						r = (unsigned char) round(src[x*4 + 2] * 255.f/32768.f);
						a = (unsigned char) round(src[x*4 + 3] * 255.f/32768.f);
						break;
					}

					case 4:
					{
						// 32 bit float
						const float* src = (float*) (src_ptr + y*pix_buf->row_bytesL);
						b = (unsigned char) round(src[x*4 + 0] * 255.f);
						g = (unsigned char) round(src[x*4 + 1] * 255.f);
						r = (unsigned char) round(src[x*4 + 2] * 255.f);
						a = (unsigned char) round(src[x*4 + 3] * 255.f);
						break;
					}

					default:
						break;
				}

				pixels2[y*tWidth + x].alpha	= a;
				pixels2[y*tWidth + x].red	= r;
				pixels2[y*tWidth + x].green	= g;
				pixels2[y*tWidth + x].blue	= b;

			}
		}

		spoutsender.SendImage((unsigned char *)pixels2, tWidth, tHeight,GL_BGRA_EXT, false);
	}

	return PF_Err_NONE;
}

static void
MyDeath(
	void 		*hook_refconPV)
{
	// free anything you allocated.
	spoutsender.SpoutCleanUp(true);
}

static void
MyVersion(
	void 			*hook_refconPV,
	A_u_long		*versionPV)
{
	
	*versionPV = 1;
}

DllExport PF_Err
EntryPointFunc(
	A_long			major_version,
	A_long			minor_version,
	AE_FileSpecH	file_specH,
	AE_FileSpecH	res_specH,
	AE_Hooks		*hooksP)
{
	PF_Err	err = PF_Err_NONE;
	hooksP->blit_hook_func = MyBlit;
	hooksP->death_hook_func = MyDeath;
	hooksP->version_hook_func = MyVersion;


	
	//AllocConsole();
	//freopen("CONIN$",  "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	//freopen("CONOUT$", "w", stderr);
	//
	//printf("SpoutAE EntryPoint\n");
	

	
	//spoutsender = GetSpout(); // Create an instance of the Spout library
	bSenderInitialized = false;						// Spout sender initialization
	bMemoryShare = false;						// Memoryshare mode not detected yet
	strcpy(senderName, "After Effects");		// Set the sender name
	tWidth = 0;
	tHeight = 0;
	pixels2 = nullptr;

	return err;
}

