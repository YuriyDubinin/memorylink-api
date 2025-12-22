#pragma once

#include <string>

#include "models/config.h"

class ConfigManager {
public:
    static void          Load(const std::string& filename);
    static const Config& Get();

private:
    Config cfg_;

    void LoadConfig_(const std::string& filename);

    static ConfigManager& instance_();
};