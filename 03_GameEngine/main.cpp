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

class Sprite
{
public:
    Sprite(uint16_t sizeX, uint16_t sizeY, const char* spriteBuffer)
    {
        if (strlen(spriteBuffer) != sizeX * sizeY)
        {
            std::cout << "ERROR: Failed to create sprite, sizes didn't match input buffer size." << std::endl;
            return;
        }

        this->SizeX = sizeX;
        this->SizeY = sizeY;

        SpriteBuffer = new CHAR_INFO[this->SizeX * this->SizeY];

        // Construct SpriteBuffer from the input string (spriteBuffer).
        for (uint16_t i = 0; i < strlen(spriteBuffer); i++)
        {
            CHAR_INFO charInfo;
            charInfo.Char.AsciiChar = spriteBuffer[i];
            charInfo.Attributes = FOREGROUND_GREEN;

            this->SpriteBuffer[i] = charInfo;
        }
    }    
    
    const CHAR_INFO GetChar(uint8_t x, uint8_t y)
    {
        uint16_t index = y * SizeX + x;

        if (index < SizeX * SizeY)
        {
            return SpriteBuffer[index];
        }

        CHAR_INFO errorCharInfo;
        errorCharInfo.Char.AsciiChar = 'X';    
        errorCharInfo.Attributes = FOREGROUND_RED;
        return errorCharInfo;
    }

public:
    uint16_t SizeX;
    uint16_t SizeY;
    CHAR_INFO* SpriteBuffer;
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
    Sprite* EntitySprite;
};

class PlayerEntity : public Entity
{
public:
    virtual void BeginPlay()
    {
        EntitySprite = new Sprite(3, 3, "I I/+\\***");
    }

    virtual void Tick() override
    {
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_D)) PositionX += 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_A)) PositionX -= 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_S)) PositionY += 1;
        if (InputManager::IsKeyDown(SHITTYENGINE_KEYCODE_W)) PositionY -= 1;
    }
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
                charInfo.Attributes = FOREGROUND_GREEN;

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

    void SubmitSprite(uint8_t posX, uint8_t posY, Sprite* sprite)
    { 
        for(uint8_t x = 0; x < sprite->SizeX; x++)
        {
            for(uint8_t y = 0; y < sprite->SizeY; y++)
            {
                SubmitPixel(posX + x, posY + y, sprite->GetChar(x, y));
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
    CHAR_INFO* m_ScreenBuffer;
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
        // TODO: Manage data on destruction.
        m_SceneEntities.push_back(new PlayerEntity());

        // Call the BeginPlay() function of all entities in the scene.
        for (Entity* entity : m_SceneEntities)
        {
            entity->BeginPlay();
        }
    }

    void Tick()
    {
        // Call the Tick() function of all entities in the scene.
        for (Entity* entity : m_SceneEntities)
        {
            entity->Tick();
        }

        m_Renderer.Clear();
        // Draw all entities in the scene.
        for (Entity* entity : m_SceneEntities)
        {
            m_Renderer.SubmitSprite(entity->PositionX, entity->PositionY, entity->EntitySprite);
        }
        m_Renderer.Draw();
    }

private:
    std::vector<Entity*> m_SceneEntities;
    Renderer m_Renderer;
};

int main(int argc, char *argv[])
{
    Game game;
    game.Run();

    return 0;
}