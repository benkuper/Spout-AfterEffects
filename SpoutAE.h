#pragma once

#ifndef SPOUTAE_H
#define SPOUTAE_H

#include "AEConfig.h"
#include "A.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "AE_Hook.h"
#include "entry.h"

#ifdef AE_OS_WIN
#include <stdio.h>
#include <windows.h>
#endif

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	0

#define	NAME				"Spout"
#define DESCRIPTION			"Spout Sender Plugin"

#include "AE_Hook.h"

#define DllExport   __declspec( dllexport )


#include "SpoutLibrary.h"
SPOUTLIBRARY * spoutsender;
bool bSenderInitialized;						// Sender initialization result
char senderName[256];							// Sender name
bool bMemoryShare;
int tWidth ;
int tHeight ;
PF_Pixel8 * pixels2;

extern "C" {
	DllExport PF_Err
		EntryPointFunc(
			A_long 			major_version,	/* >> */
			A_long 			minor_version,	/* >> */
			AE_FileSpecH 	file_specH,		/* >> */
			AE_FileSpecH 	res_specH,		/* >> */
			AE_Hooks 		*hooksP);		/* <> */
}
#ifdef AE_OS_WIN
BOOL WINAPI LibMain(HANDLE hInstance, DWORD fdwReason, LPVOID lpReserved);
#endif

#endif //SPOUTAE_H