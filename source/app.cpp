#include "app.h"
#include <switch.h>
#include <cstring>

// Constructor
App::App(u64 id, const std::string &n, const std::string &dv, const std::string &a, const u8 *ic)
    : titleID(id), name(n), display_version(dv), author(a)
{
    memset(icon, 0, sizeof(icon));
    if (ic)
        memcpy(icon, ic, sizeof(icon));
}

// Getters
u64 App::getTitleID() const { return titleID; }
std::string App::getName() const { return name; }
std::string App::getDisplayVersion() const { return display_version; }
std::string App::getAuthor() const { return author; }
const u8 *App::getIcon() const { return icon; }

// Print info
void App::printInfo() const
{
    printf("Title ID: %016lX\n", titleID);
    printf("Name: %s\n", name.c_str());
    printf("Version: %s\n", display_version.c_str());
    printf("Author: %s\n", author.c_str());
}

// Launch
void App::launch() const
{
    Result rc = appletRequestLaunchApplication(titleID, NULL);
    if (R_FAILED(rc))
    {
        printf("Failed to launch application %s (Title ID: %016lX): 0x%08X\n",
               name.c_str(), titleID, rc);
    }
}
