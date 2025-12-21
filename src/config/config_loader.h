#pragma once

#include <cstdio>
#include <iostream>
#include <string>

#include "models/config.h"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

class ConfigLoader {
public:
    static Config Load(const std::string& filename);
};