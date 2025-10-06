#pragma once
#include <string>
#include <cstdio>
#include <switch.h>
#include <cstring>

class App
{
private:
    u64 titleID;
    std::string name;
    std::string display_version;
    std::string author;
    u8 icon[0x20000]; // JPEG icon

public:
    // Constructor
    App(u64 id, const std::string &n, const std::string &dv, const std::string &a, const u8 *ic);

    // Getters
    u64 getTitleID() const;
    std::string getName() const;
    std::string getDisplayVersion() const;
    std::string getAuthor() const;
    const u8 *getIcon() const;

    // Methods
    void printInfo() const;
    void launch() const;
};
