// SpoutAE.cpp : Defines the exported functions for the DLL application.
//

#include "SpoutAE.h"
#include "Spout2Helper.h"


int tWidth = 0;
int tHeight = 0;
bool texInit = false;
bool firstInit = false;
 
SpoutSender sender;
GLuint texID;

#ifdef AE_OS_WIN 
	BOOL APIENTRY LibMain(HANDLE hInstance, DWORD fdwReason, LPVOID lpReserved)
	{
		printf("SpoutAE.aex Initialization entry point.");
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
	
	if(pix_bufP0->widthL != (tWidth-4) || pix_bufP0->heightL != (tHeight+4))
	{
		texInit = false;
	}
	
	//printf("Blit : %i	%i	%i	%i	%i\n",pix_bufP0->widthL,pix_bufP0->heightL,pix_bufP0->depthL,pix_bufP0->plane_bytesL,pix_bufP0->chan_bytesL);

	if(!texInit)
	{
		//printf("texInit !\n");
		tWidth = pix_bufP0->widthL+4;
		tHeight = pix_bufP0->heightL-4;

		
		InitGLtexture(texID,tWidth,tHeight);
		
		bool bTextureShare = false;
		
		

		if(!firstInit)
		{
			texInit = sender.CreateSender("AfterFX", tWidth,tHeight);
			//texInit = spout.InitSender("AE", tWidth,tHeight,bTextureShare,false);
			firstInit = true;
		}

		//printf("TexInit OK !\n");
		texInit = true;
		
	}

	if(texInit)
	{
		
		PF_Pixel8 * pixels  = (PF_Pixel8 *) pix_bufP0->pixelsPV;
		PF_Pixel8 * pixels2 = (PF_Pixel8 *)malloc(tWidth*tHeight*sizeof(PF_Pixel8));
		
		memcpy(pixels2,pixels,tWidth*tHeight*sizeof(PF_Pixel8));
		
		for(int y=0; y<tHeight; y++)
		{
			for(int x=0; x<tWidth; x++)
			{
				int index = x+y*tWidth;
				int index2 = x+(tHeight-1 - y)*tWidth;
				pixels2[index].alpha = pixels[index2].blue;
				pixels2[index].red = pixels[index2].green;
				pixels2[index].green = pixels[index2].red;
				pixels2[index].blue = pixels[index2].alpha;
			}
		}
			
		//Spout::Spout::sen
		sender.SendImage((unsigned char *)pixels2,tWidth,tHeight);
		//bool shareResult = sender.UpdateSender("AfterFX",*)pixels2,tWidth,tHeight);
		//printf("Share Result ? %s",shareResult?"OK":"Error");
		
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
EntryPointFunc (
	A_long			major_version,
	A_long			minor_version,
	AE_FileSpecH	file_specH,			
	AE_FileSpecH	res_specH,	
	AE_Hooks		*hooksP)
{
	PF_Err	err 				= PF_Err_NONE;
	hooksP->blit_hook_func 		= MyBlit;
	hooksP->death_hook_func 	= MyDeath;
	hooksP->version_hook_func 	= MyVersion;


	
	//Temp

	AllocConsole();
    freopen("CONIN$",  "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
	
	printf("SpoutAE EntryPoint\n");

	//Ben - Spout Helpers 
	InitGL();

	sender.CreateSender("AfterFX", tWidth,tHeight);
	
	/*
	printf("BlitHook init %i\n",processID);

	bool initResult = false;

	printf("Init Spout with hwnd : %i\n",hWnd);
		
	initResult = initSpout(hWnd);

	printf("> Spout init result : %d\n",initResult);
	
	*/

	return err;
}

