#pragma once

#include "models/config.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"

#include <cstdio>
#include <iostream>
#include <string>

class ConfigLoader {
public:
    static Config Load(const std::string& filename);
};