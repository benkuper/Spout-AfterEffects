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
#define DESCRIPTION			"Spout Sender"

#include "AE_Hook.h"

#ifndef DllExport
	#define DllExport   __declspec( dllexport )
#endif

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
	BOOL __stdcall LibMain(HANDLE hInstance, DWORD fdwReason, LPVOID lpReserved);
#endif
		
#endif //EMP_H