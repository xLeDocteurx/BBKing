#include <string>
#include <vector>

#include <Step.h>

#ifndef PARTSTRUCT_H
#define PARTSTRUCT_H
struct Part
{
    int staves;
    // std::vector<std::vector<int>> steps;
    std::vector<std::vector<Step>> steps;
};
#endif // PARTSTRUCT_H
