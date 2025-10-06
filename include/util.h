#pragma once
#include "app.h"
#include <vector>

#define MAX_TITLES 200 // Raise later it's just slow to cap it

namespace util
{
    // Returns all installed apps with metadata
    std::vector<App> getInstalledApps();
}
