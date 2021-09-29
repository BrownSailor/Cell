#include "include/utils.h"

int typeToInt(std::string name)
{
    int t = 0;
    for (unsigned int i = 0; i < name.size(); i++)
    {
        t += name[i];
    }

    return t;
}