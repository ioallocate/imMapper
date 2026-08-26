#include "imMapper.hpp"
#include "Helpers/Logger.hpp"

int main()
{
    imMapper mapper;
    if (!mapper.Initialize()) {
        Console->Log("[imMapper] failed to initialize");
        return 1;
    }

    Console->Log("[imMapper] initialized");
    return 0;
}
