#include <cstdio>
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "models/config.hpp"

class ConfigLoader {
public:
    static Config Load(const std::string& filename);
};