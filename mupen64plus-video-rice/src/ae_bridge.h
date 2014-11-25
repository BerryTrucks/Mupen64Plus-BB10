#ifndef __AE_IMPORTS__
#define __AE_IMPORTS__

#include <bbutil.h>

#define HARDWARE_TYPE_UNKNOWN 0
#define HARDWARE_TYPE_OMAP 1
#define HARDWARE_TYPE_OMAP_2 2
#define HARDWARE_TYPE_QUALCOMM 3
#define HARDWARE_TYPE_IMAP 4
#define HARDWARE_TYPE_TEGRA 5
#define HARDWARE_TYPE_CUSTOM 999

inline int Android_JNI_GetHardwareType() { return HARDWARE_TYPE_QUALCOMM; }

inline void Android_JNI_GetPolygonOffset(int hardwareType, int bias, float *polygonOffsetFactor, float *polygonOffsetUnits) { *polygonOffsetFactor = -0.2f; *polygonOffsetUnits = -0.2f; }

extern void Android_JNI_SwapWindow() { PB_eglSwapBuffers(); }

#endif //__AE_IMPORTS__