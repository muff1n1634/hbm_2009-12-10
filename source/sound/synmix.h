#ifndef RVL_SDK_HBM_SOUND_SYN_MIX_H
#define RVL_SDK_HBM_SOUND_SYN_MIX_H

/*******************************************************************************
 * headers
 */

#include <types.h> // s32

#include "syn.h" // HBMSYNVOICE

/*******************************************************************************
 * external variables
 */

#ifdef __cplusplus
	extern "C" {
#endif

// .data
extern const s32 __HBMSYNVolumeAttenuation[128];
extern const s32 __HBMSYNAttackAttnTable[100];

/*******************************************************************************
 * functions
 */

void __HBMSYNSetupVolume(HBMSYNVOICE *voice);
void __HBMSYNSetupPan(HBMSYNVOICE *voice);
s32 __HBMSYNGetVoiceInput(HBMSYNVOICE *voice);
s32 __HBMSYNGetVoiceFader(HBMSYNVOICE *voice);
void __HBMSYNUpdateMix(HBMSYNVOICE *voice);

#ifdef __cplusplus
	}
#endif

#endif // RVL_SDK_HBM_SOUND_SYN_MIX_H
