#include <inttypes.h>
#include <windows.h>
#include <iostream>
#include <string.h>

class Sprite
{
public:
    Sprite(uint8_t width, uint8_t height, const char* spriteImage)
    {
        m_Width = width;
        m_Height = height;

        if (strlen(spriteImage) != m_Width * m_Height)
        {
            std::cout << "ERROR: Failed to create sprite, sizes didn't match" << std::endl;
            return;
        }

        m_SpriteBuffer = new CHAR_INFO[m_Width * m_Height];

        for (uint16_t i = 0; i < strlen(spriteImage); i++)
        {
            CHAR_INFO charInfo;
            charInfo.Char.AsciiChar = spriteImage[i];    
            charInfo.Attributes = FOREGROUND_GREEN;

            m_SpriteBuffer[i] = charInfo;
        }
    }

    const CHAR_INFO GetChar(uint8_t posX, uint8_t posY)
    {
        uint16_t index = posY * m_Width + posX;

        if (index < m_Width * m_Height)
        {
            return m_SpriteBuffer[index];
        }

        CHAR_INFO errorCharInfo;
        errorCharInfo.Char.AsciiChar = 'X';    
        errorCharInfo.Attributes = FOREGROUND_RED;
        return errorCharInfo;
    }

    const uint8_t GetWidth()
    {
        return m_Width;
    }

    const uint8_t GetHeight()
    {
        return m_Height;
    }

private:
    CHAR_INFO* m_SpriteBuffer;
    uint8_t m_Width;
    uint8_t m_Height;
};

class Renderer
{
public:
    void Resize(uint8_t rows, uint8_t columns)
    {
        m_Rows = rows;
        m_Columns = columns;

        // Resize window
        HWND consoleWindow = GetConsoleWindow();
        RECT ConsoleRect;
        GetWindowRect(consoleWindow, &ConsoleRect); 
        MoveWindow(consoleWindow, ConsoleRect.left, ConsoleRect.top, 800, 600, TRUE);
    
        // Set the console screen buffer size 
        m_ConsoleScreenBufferHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleScreenBufferSize(m_ConsoleScreenBufferHandle, { m_Rows, m_Columns });
 
        // Create the new, resized screen buffer
        m_ScreenBuffer = new CHAR_INFO[rows * columns];
    }

    void Clear()
    {
        for(uint8_t row = 0; row < m_Rows; row++)
        {
            for(uint8_t column = 0; column < m_Columns; column++)
            {
                CHAR_INFO charInfo;
                charInfo.Char.AsciiChar = '-';
                charInfo.Attributes = FOREGROUND_GREEN;

                SubmitPixel(row, column, charInfo);
            }
        }
    }

    void Draw()
    {
        SMALL_RECT drawRect = {0, 0, (uint8_t)(m_Rows-1), (uint8_t)(m_Columns-1)};
        COORD screenBufferSize = {m_Rows, m_Columns};
        COORD screenBufferOffset = {0, 0};

        WriteConsoleOutputA(m_ConsoleScreenBufferHandle, m_ScreenBuffer, screenBufferSize, screenBufferOffset, &drawRect);
    }

    void SubmitPixel(uint8_t row, uint8_t column, CHAR_INFO charInfo)
    {
        uint16_t index = column * m_Rows + row;

        if (index < m_Rows * m_Columns)
        {
            m_ScreenBuffer[index] = charInfo;
        }
    }

    void SubmitRect(uint8_t posX, uint8_t posY, uint8_t sizeX, uint8_t sizeY)
    {
        for(uint8_t row = 0; row < sizeX; row++)
        {
            for(uint8_t column = 0; column < sizeY; column++)
            {
                CHAR_INFO charInfo;
                charInfo.Char.AsciiChar = '#';    
                charInfo.Attributes = FOREGROUND_GREEN;

                SubmitPixel(posX + row, posY + column, charInfo);
            }
        }
    }

    void SubmitSprite(uint8_t posX, uint8_t posY, Sprite& sprite)
    { 
        for(uint8_t x = 0; x < sprite.GetWidth(); x++)
        {
            for(uint8_t y = 0; y < sprite.GetHeight(); y++)
            {
                SubmitPixel(posX + x, posY + y, sprite.GetChar(x, y));
            }
        }
    }

private:
    HANDLE m_ConsoleScreenBufferHandle;
    CHAR_INFO* m_ScreenBuffer;
    uint8_t m_Rows;
    uint8_t m_Columns;
};

int main(int argc, char* argv[])
{
    Renderer renderer;
    renderer.Resize(70, 20);

    Sprite spaceShipSprite = Sprite(3, 3, "123456789");

    while (true)
    {
        renderer.Clear();
        
        renderer.SubmitRect(2, 1, 2, 4);
        renderer.SubmitSprite(6, 5, spaceShipSprite);

        renderer.Draw();
    }

    return 0;
}