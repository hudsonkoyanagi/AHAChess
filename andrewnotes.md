

- did not touch pawn yet, will edit this note if i do

---
- Move object needs more flags in order to properly reverse history. Maybe it needs to be like formal chess algebraic notation.
- For make_move, history.push_back(m) is only occuring for captures for pawns. It should be after any successful move.

---
- HUDSON NOTE: done this, added check at top of function as no piece can start and end on same square
<s>
- should note that, if moving a piece, sometimes you need to check that you aren't moving it stationarily (no change to position)
 - actually, think that this might be covered by a check to see if the piece is on the right team?
 - for bishop and rook however, this meant that I have to check target->col before I check through the spaces between the current position and the target position (otherwise the while loop is fucked)
 </s>
---
- it might make sense to create a function to check the possible moves of a piece, as that could be re-used for check logic
---
- the blocks
    std::swap(target->loc, p->loc); // update piece location
      std::swap(board[m.start.row][m.start.col], board[m.end.row][m.end.col]); // update board

      history.push_back(m); // is this necessary? I think it should be, check andrewnotes for what I think about history

      for (auto v : views) {
        v->render(board); // TODO FIX THIS LATER
      }
      // notify_views();
      return SUCCESS;
  are repetitive. can probably turn this into a function.
---
- please fix check condition, not sure if right!
- add an if condition for a check if you attempt to move a moven piece
- add an if condition for a check if you attempt to castle
- do we return castle (new enum)?
---
- NEED: add history to each case
---
quick copy paste for a run:

game human human
move e2 e4
move e7 e5
