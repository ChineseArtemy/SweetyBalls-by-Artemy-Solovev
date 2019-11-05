#include <SFML/Graphics.hpp>
#include <fstream>


bool wasLineBreakIn (string text)
{
    for (int i = 0; i <= text.length(); i++)
    {
        if (text[i] == "\n")
        {
            return true;
        }
    }
    return false;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    ifstream sourceFile;
    sourceFile.open("source.txt");
    sourceFile.seekg(0, std::ios::end);
    int fileSize = sourceFile.tellg();
    sourceFile.seekg(0, std::ios::beg);
    string lines = new [fileSize];
    sourceFile.getLine(lines, fileSize);
    sourceFile.close();



    sf::Font font;
    font.loadFromFile("arial.ttf")

    sf::Text text;
    text.setFont(font);
    string visibleText = "HackerTyper by ChineseArtemy. 2019, All Rights Reserved.\n";
    text.setPosition(0,0);
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::Green);
    int stringPtr = 0;
    int lineCounter = 1;
    float textSize = 14;
    float posY = text.getPosition().y;

    while (window.isOpen())
    {
        textSize = lineCounter * (text.getLineSpacing() + text.getCharacterSize());
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    visibleText.append(lines, stringPtr, 3);
                    stringPtr += 3;
                         /* апппенднуть строку несколькими символами */
            }
        }



        if (wasLineBreakIn(appendedPiece))
            {
                lineCounter += 1;
            }
        if (lineCounter >= window.getSize().y)
            {
                posY -= text.getCharacterSize() + text.getLineSpacing();
                text.setPosition(0,posY);
            }

        text.setString(visibleText);

        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
