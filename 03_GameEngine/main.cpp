#include <iostream>
#include <vector>
#include <windows.h>

class Entity
{
public:
    void BeginPlay()
    {
    }

    void Tick()
    {
    }
};

class Scene
{
public: 
    std::vector<Entity> SceneEntities;
};

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
    
private:
    Scene m_Scene;
};

int main(int argc, char* argv[])
{
    Game game;
    game.Run();

    return 0;
}