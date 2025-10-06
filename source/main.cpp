#include "util.h"
#include "app.h"
#include <switch.h>
#include <vector>
#include <cstdio>

int main(int argc, char *argv[])
{
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    if (R_FAILED(nsInitialize()))
    {
        printf("Failed to initialize ns\n");
        consoleUpdate(NULL);
        consoleExit(NULL);
        return -1;
    }

    // Get installed apps
    std::vector<App> apps = util::getInstalledApps();

    if (apps.empty())
    {
        printf("No installed titles found.\nPress + to exit.\n");
        while (appletMainLoop())
        {
            padUpdate(&pad);
            if (padGetButtonsDown(&pad) & HidNpadButton_Plus)
                break;
            consoleUpdate(NULL);
        }
        nsExit();
        consoleExit(NULL);
        return 0;
    }

    size_t selected = 0;

    while (appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        // Scroll up
        if (kDown & HidNpadButton_Up && selected > 0)
        {
            selected--;
        }

        // Scroll down
        if (kDown & HidNpadButton_Down && selected + 1 < apps.size())
        {
            selected++;
        }

        // Launch app
        if (kDown & HidNpadButton_A)
        {
            printf("Launching %s...\n", apps[selected].getName().c_str());
            consoleUpdate(NULL);
            apps[selected].launch();
            break; // exit after launching
        }

        // Exit app
        if (kDown & HidNpadButton_Plus)
        {
            break;
        }

        // Display menu
        consoleClear();
        printf("Installed Apps (use D-pad, A to launch):\n\n");
        for (size_t i = 0; i < apps.size(); i++)
        {
            if (i == selected)
                printf(" > %s\n", apps[i].getName().c_str());
            else
                printf("   %s\n", apps[i].getName().c_str());
        }

        consoleUpdate(NULL);
    }

    nsExit();
    consoleExit(NULL);
    return 0;
}
