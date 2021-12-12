#include <iostream>
#include <windows.h>

class Game
{
public:
    void Run()
    {
        std::cout << "Game::Run()" << std::endl;

        BeginPlay();

        while (true)
        {
            Tick();
            
            Sleep(10);
        }
    }

    void BeginPlay()
    {
        std::cout << "Game::BeginPlay()" << std::endl;
    }
    
    void Tick()
    {
        std::cout << "Game::Tick()" << std::endl;
    }
};

int main(int argc, char* argv[])
{
    Game game;
    game.Run();

    return 0;
}