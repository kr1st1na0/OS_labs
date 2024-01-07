#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "dag.hpp"

#include "inipp.h"

DAG Parse(const std::string &path);
