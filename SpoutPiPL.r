#include "AEConfig.h"

#ifndef AE_OS_WIN
	#include "AE_General.r"
#endif

resource 'PiPL' (16000) {
	{	/* array properties: 7 elements */
		/* [1] */
		Kind {
			AEGeneral
		},
		/* [2] */
		Name {
			"Spout"
		},
		/* [3] */
		Category {
			"Spout Sender Plugin"
		},
		/* [4] */
		Version {
			0x00030000
		},
		/* [5] */
#ifdef AE_OS_WIN
	#ifdef AE_PROC_INTELx64
		CodeWin64X86 {"EntryPointFunc"},
	#else
		CodeWin32X86 {"EntryPointFunc"},
	#endif
#else
	#ifdef AE_OS_MAC
			CodeMachOPowerPC {"EntryPointFunc"},
			CodeMacIntel32 {"EntryPointFunc"},
			CodeMacIntel64 {"EntryPointFunc"},
	#endif
#endif
	}
};

