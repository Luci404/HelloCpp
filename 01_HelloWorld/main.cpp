#include <iostream>
#include <string>

#define HELLO_WORLD_MESSAGE "Hello World!"

void Print(const std::string& message)
{
    std::cout << "[HelloWorldApp] " << message << '\n';
} 

int main(int argc, char* argv[])
{
    Print(HELLO_WORLD_MESSAGE);
    Print("argc: " + std::to_string(argc));

    return 0;
}