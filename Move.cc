#include "Move.h"

Move::Move(Cord s, Cord e) : start{s}, end{e}, moved{EMPTY}, taken{EMPTY}, move_result{INVALID_MOVE}, check{false} {}