#pragma once
#include "Assets/Vertex.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class OBJImporter
{
public:
    static bool LoadOBJStream(std::istream &file, const std::string &path, std::vector<Vertex> &outVertices, std::vector<unsigned int> &outIndices);
};