#include "TheEye.h"

TheEye* TheEye::__instance = NULL;

TheEye* TheEye::GetInstance()
{
    if (__instance == NULL)
        __instance = new TheEye();

    return __instance;
}
