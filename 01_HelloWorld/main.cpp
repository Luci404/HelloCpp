#include <iostream>
#include <string>

#define HELLO_WORLD_MESSAGE "Hello World!"

void Print(std::string message)
{
    std::cout << "[HelloWorldApp] " << message << '\n';
} 

int main(int argc, char* argv[])
{
    Print(HELLO_WORLD_MESSAGE);
    Print("argc: " + std::to_string(argc));

    return 0;
}

// g++
/*main(0, {})
{
    if (argc == 0)
    {
        // error
        return -1 
    }
    else
    {
        // compile
    }

    return 0;
}*/