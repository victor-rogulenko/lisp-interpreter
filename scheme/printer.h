#pragma once

#include <sstream>

#include <parser.h>

class Object;

void PrintTo(const std::shared_ptr<Object>& obj, std::ostream* out);

std::string Print(const std::shared_ptr<Object>& obj);
