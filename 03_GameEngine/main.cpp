#include <iostream>
#include <vector>
#include <windows.h>

#define SHITTYENGINE_KEYCODE_SPACE 0x20
#define SHITTYENGINE_KEYCODE_A 0x41
#define SHITTYENGINE_KEYCODE_D 0x44
#define SHITTYENGINE_KEYCODE_S 0x53
#define SHITTYENGINE_KEYCODE_W 0x57

class InputManager
{
public:
    static bool IsKeyDown(int keyCode)
    {
        return GetAsyncKeyState(keyCode) /*& 0x8000*/;
    }
};

class Entity
{
public:
    Entity()
    {
        PositionX = 0;
        PositionY = 0;
    }

    virtual void BeginPlay()
    {
    }

    virtual void Tick()
    {
    }

public:
    uint16_t PositionX;
    uint16_t PositionY;
};

class PlayerEntity : public Entity
{
public:
    virtual void Tick() override
    {
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_D)) PositionX += 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_A)) PositionX -= 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_S)) PositionY += 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_W)) PositionY -= 1;
    }
};

class Scene
{
public:
    std::vector<Entity *> SceneEntities;
};

class Renderer
{
public:
    Renderer()
    {
        m_Rows = 70;
        m_Columns = 30;

        // Set screen buffer size
        m_ScreenBufferHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleScreenBufferSize(m_ScreenBufferHandle, {m_Rows, m_Columns});

        // Create new screen buffer
        m_ScreenBuffer = new CHAR_INFO[m_Rows * m_Columns];

        system("cls");
    };

    void Clear()
    {
        for (uint8_t row = 0; row < m_Rows; row++)
        {
            for (uint8_t column = 0; column < m_Columns; column++)
            {
                // For every pixel
                CHAR_INFO charInfo;
                charInfo.Char.AsciiChar = '.';
                charInfo.Attributes = BACKGROUND_BLUE | FOREGROUND_BLUE;

                SubmitPixel(row, column, charInfo);
            }
        }
    }

    void SubmitRect(uint16_t positionX, uint16_t positionY, uint16_t sizeX, uint16_t sizeY)
    {
        for (uint16_t row = 0; row < sizeX; row++)
        {
            for (uint8_t column = 0; column < sizeY; column++)
            {
                CHAR_INFO charInfo;
                charInfo.Char.AsciiChar = '#';
                charInfo.Attributes = BACKGROUND_GREEN | FOREGROUND_GREEN;

                SubmitPixel(row + positionX, column + positionY, charInfo);
            }
        }
    }

    void SubmitPixel(uint8_t row, uint8_t column, CHAR_INFO charInfo)
    {
        uint16_t index = column * m_Rows + row;

        if (index < m_Rows * m_Columns)
        {
            m_ScreenBuffer[index] = charInfo;
        }
    }

    void Draw()
    {
        SMALL_RECT drawRect = {0, 0, (uint8_t)(m_Rows - 1), (uint8_t)(m_Columns - 1)};
        COORD screenBufferSize = {m_Rows, m_Columns};
        COORD screenBufferOffset = {0, 0};

        WriteConsoleOutputA(m_ScreenBufferHandle, m_ScreenBuffer, screenBufferSize, screenBufferOffset, &drawRect);
    }

private:
    HANDLE m_ScreenBufferHandle;
    CHAR_INFO *m_ScreenBuffer;
    uint8_t m_Rows;
    uint8_t m_Columns;
};

class Game
{
public:
    void Run()
    {
        BeginPlay();

        while (true)
        {
            Tick();
            Sleep(10);
        }
    }

    void BeginPlay()
    {
    }

    void Tick()
    {
        m_Player.Tick();

        m_Renderer.Clear();
        m_Renderer.SubmitRect(m_Player.PositionX, m_Player.PositionY, 3, 3);
        m_Renderer.Draw();
    }

private:
    Scene m_Scene;
    Renderer m_Renderer;

    // TMP
    PlayerEntity m_Player;
};

int main(int argc, char *argv[])
{
    Game game;
    game.Run();

    return 0;
}