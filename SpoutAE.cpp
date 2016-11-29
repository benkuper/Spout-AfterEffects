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
	const AE_PixBuffer			*pix_bufP0,
	const AE_ViewCoordinates	*viewP,
	AE_BlitReceipt				receipt,
	AE_BlitCompleteFunc			complete_func0,
	AE_BlitInFlags				in_flags,
	AE_BlitOutFlags				*out_flags)
{

	bool shouldUpdate = false;

	if (pix_bufP0->widthL != (tWidth - 4) || pix_bufP0->heightL != (tHeight + 4)) shouldUpdate = true;
	tWidth = pix_bufP0->widthL + 4;
	tHeight = pix_bufP0->heightL - 4;
	

	if (!bSenderInitialized)
	{
		printf("Create sender\n");
		bSenderInitialized = spoutsender->CreateSender(senderName, tWidth, tHeight);
		if (pixels2 != nullptr)
		{
			delete pixels2;
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}
	}
	else if (shouldUpdate)
	{
		printf("Update sender\n");
		spoutsender->ReleaseSender();
		bSenderInitialized = spoutsender->CreateSender(senderName, tWidth, tHeight);
		if (pixels2 != nullptr)
		{
			delete pixels2;
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}

	}
	

	if(bSenderInitialized)
	{

		PF_Pixel8 * pixels = (PF_Pixel8 *)pix_bufP0->pixelsPV;

		if (pixels2 == nullptr)
		{
			pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight * sizeof(PF_Pixel8));
		}
		 
		memcpy(pixels2, pixels, tWidth*tHeight * sizeof(PF_Pixel8));

		for (int y = 0; y<tHeight; y++)
		{
			for (int x = 0; x<tWidth; x++)
			{
				int index = x + y*tWidth;
				int index2 = x + (tHeight - 1 - y)*tWidth;
				pixels2[index].alpha = pixels[index2].blue;
				pixels2[index].red = pixels[index2].green;
				pixels2[index].green = pixels[index2].red;
				pixels2[index].blue = pixels[index2].alpha;
			}
		}

		spoutsender->SendImage((unsigned char *)pixels2, tWidth, tHeight,GL_BGRA_EXT,true);
	}


	return PF_Err_NONE;
}

static void
MyDeath(
	void 		*hook_refconPV)
{
	// free anything you allocated.
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


	/*
	AllocConsole();
	freopen("CONIN$",  "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	
	printf("SpoutAE EntryPoint\n");
	*/

	spoutsender = GetSpout(); // Create an instance of the Spout library
	bSenderInitialized = false;						// Spout sender initialization
	bMemoryShare = false;						// Memoryshare mode not detected yet
	strcpy(senderName, "After Effects");		// Set the sender name
	tWidth = 0;
	tHeight = 0;
	pixels2 = nullptr;

	return err;
}

