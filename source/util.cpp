#include "util.h"
#include "app.h"

#include "util.h"
#include "app.h"

namespace util
{
    std::vector<App> getInstalledApps()
    {
        std::vector<App> apps;

        NsApplicationRecord *records = new NsApplicationRecord[MAX_TITLES];
        s32 recordCount = 0;

        // List all installed applications
        Result rc = nsListApplicationRecord(records, MAX_TITLES, 0, &recordCount);
        if (R_FAILED(rc))
        {
            delete[] records;
            return apps;
        }

        for (s32 i = 0; i < recordCount; i++)
        {
            NsApplicationControlData controlData;
            u64 controlSize = 0;

            // Get application control data
            rc = nsGetApplicationControlData(
                NsApplicationControlSource_Storage,
                records[i].application_id,
                &controlData,
                sizeof(NsApplicationControlData),
                &controlSize);

            if (R_FAILED(rc) || controlSize < sizeof(controlData.nacp))
                continue;

            // Extract UTF-8 title name
            std::string name(reinterpret_cast<char *>(controlData.nacp.lang[0].name));

            // Extract display version
            std::string displayVersion(reinterpret_cast<char *>(controlData.nacp.display_version));

            // Extract author
            std::string author(reinterpret_cast<char *>(controlData.nacp.lang[0].author));

            // Extract icon if available
            u8 icon[0x20000] = {0};
            size_t iconSize = sizeof(icon);
            if (controlSize >= sizeof(controlData))
            {
                memcpy(icon, controlData.icon, iconSize);
            }
            else
            {
                memset(icon, 0, iconSize);
            }

            // Create App object
            apps.emplace_back(
                records[i].application_id,
                name,
                displayVersion,
                author,
                icon);
        }

        delete[] records;

        return apps;
    }

} // namespace util
