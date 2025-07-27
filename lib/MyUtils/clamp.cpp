#include <MyUtils.h>
#include <Defs.h>
  
// // One function works for all data types. 
// // This would work even for user defined types 
// // if operator '>' is overloaded 
// template <typename T>

uint32_t clampInt32(uint32_t input, uint32_t min, uint32_t max)
{
    if (input > max)
    {
        return max;
    }
    else if (input < min)
    {
        return min;
    }
    return input;
}