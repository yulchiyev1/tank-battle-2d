#pragma once
#include "Engine.h"
#include <string>
#include <vector>

class MapLoader
{
public:
    static bool Load(const std::string& filename, const EngineContext& context);
    static std::vector<glm::vec2> freeSpaces;
};