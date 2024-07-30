#ifndef UTILS_H
#define UTILS_H
#include "Cord.h"
#include "Enums.h"
#include <string>

bool is_valid_cord(const std::string& str);
Cord str_to_cord(const std::string& str);

char piece_col_to_char(PIECES p, COLOURS c);

#endif