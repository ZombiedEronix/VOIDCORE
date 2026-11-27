#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Assets/Vertex.h"
#include "Assets/AssetRegistry.hpp"

class OBJImporter
{
public:
    static bool LoadOBJStream(std::istream &bytes, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices);
};