#include <string>
#include <vector>

#ifndef PARTSTRUCT_H
#define PARTSTRUCT_H
struct Part
{
    int staves;
    std::vector<std::vector<int>> steps;
};
#endif // PARTSTRUCT_H
