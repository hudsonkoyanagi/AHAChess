#ifndef CORD_H
#define CORD_H

class Cord {
public:
    int row;
    int col;
    Cord(int r=-1, int c=-1) : row{r}, col{c} {}
};

#endif