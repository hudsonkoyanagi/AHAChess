#include "Move.h"

Move::Move(Cord s, Cord e) : start{s}, end{e}, moved{EMPTY}, taken{EMPTY}, move_result{INVALID_MOVE}, check{false} {}

Move::Move(int sr, int sc, int er, int ec) : start{ Cord{sr, sc} }, end{ Cord{er, ec} }, moved{ EMPTY }, taken{ EMPTY }, move_result{ INVALID_MOVE }, check{ false } {}