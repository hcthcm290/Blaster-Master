#include "GarbageBin.h"

GarbageBin* GarbageBin::__instance;

GarbageBin* GarbageBin::GetInstance()
{
    if (__instance == nullptr)
    {
        __instance = new GarbageBin();
    }

    return __instance;
}

void GarbageBin::AddToGarbageBin(CGameObject* object)
{
    if (!IsExistInGarbageBin(object))
    {
        storage.emplace_back(object);
    }
}

/// <summary>
/// Delete memory and remove GameObject* out of the garbage
/// If pointer to that GameObject is not found in GarbageBin, nothing change
/// </summary>
void GarbageBin::RemoveFromGarbageBin(CGameObject* object)
{
    auto ite = std::find(storage.begin(), storage.end(), object);

    if (ite != storage.end())
    {
        storage.erase(ite);
        delete object;
    }
}

/// <summary>
/// Remove GameObject* out of the garbage without Delete it
/// If pointer to that GameObject is not found in GarbageBin, nothing change
/// </summary>
void GarbageBin::RestoreFromGarbageBin(CGameObject* object)
{
    auto ite = std::find(storage.begin(), storage.end(), object);

    if (ite != storage.end())
    {
        storage.erase(ite);
    }
}

/// <summary>
/// Delete and remove ALL GameObject* in the GarbageBin
/// </summary>
void GarbageBin::CleanGarbageBin()
{
    for (auto obj : storage)
    {
        delete obj;
    }

    storage.clear();
}

bool GarbageBin::IsExistInGarbageBin(CGameObject* object)
{
    auto ite = std::find(storage.begin(), storage.end(), object);

    if (ite != storage.end())
    {
        return true;
    }

    return false;
}

/// <summary>
/// Return the number of Object in GarbageBin
/// </summary>
/// <returns></returns>
int GarbageBin::NumberOfTrash()
{
    return storage.size();
}
