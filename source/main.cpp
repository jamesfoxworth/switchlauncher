#include <stdlib.h>
#include <algorithm>
#include <switch.h>
#include <vector>
#include <string>
#include <cstring>

#define MAX_TITLES 3000

std::vector<std::string> getInstalledTitles()
{
    std::vector<std::string> titles;
    NsApplicationRecord *records = new NsApplicationRecord[MAX_TITLES];
    NsApplicationControlData *controlData = NULL;

    s32 recordCount = 0;
    u64 controlSize = 0;

    Result rc = nsListApplicationRecord(records, MAX_TITLES, 0, &recordCount);
    if (R_FAILED(rc))
    {
        delete[] records;
        return titles;
    }

    for (s32 i = 0; i < recordCount; i++)
    {
        free(controlData);
        controlData = (NsApplicationControlData *)calloc(1, sizeof(NsApplicationControlData));

        if (!controlData)
        {
            break;
        }

        controlSize = 0;
        rc = nsGetApplicationControlData(
            NsApplicationControlSource_Storage,
            records[i].application_id,
            controlData,
            sizeof(NsApplicationControlData),
            &controlSize);

        if (R_FAILED(rc))
        {
            continue;
        }

        if (controlSize < sizeof(controlData->nacp))
        {
            continue;
        }

        // Try to extract the title name (UTF-8)
        char name[0x201];
        memset(name, 0, sizeof(name));
        strncpy(name, controlData->nacp.lang[0].name, sizeof(name) - 1);

        titles.push_back(name);
    }

    free(controlData);
    delete[] records;

    std::sort(titles.begin(), titles.end());
    return titles;
}

int main(int argc, char *argv[])
{
    consoleInit(NULL);
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    PadState pad;
    padInitializeDefault(&pad);

    Result rc = nsInitialize();
    if (R_FAILED(rc))
    {
        printf("Failed to initialize ns: 0x%x\n", rc);
        consoleUpdate(NULL);
        consoleExit(NULL);
        return -1;
    }

    std::vector<std::string> titles = getInstalledTitles();

    printf("\nInstalled Titles:\n\n");
    if (titles.empty())
        printf("(none found)\n");
    else
        for (const auto &title : titles)
            printf("%s\n", title.c_str());

    printf("\nPress + to exit.\n");

    while (appletMainLoop())
    {
        padUpdate(&pad);
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break;

        consoleUpdate(NULL);
    }

    nsExit();
    consoleExit(NULL);
    return 0;
}
