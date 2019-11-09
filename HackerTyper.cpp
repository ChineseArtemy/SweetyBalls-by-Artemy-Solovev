#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>



class Text
{
	public:
		sf::Text text;	
    	
		void draw(sf::RenderWindow* window)
		{
			window->draw(text);
		}

		void  build(sf::Font* font, int fontSize, sf::Color color, float x, float y)
		{
			text.setFont(*font);
			text.setCharacterSize(fontSize);
			text.setFillColor(color);
			text.setPosition(x,y);
			sf::Text result;
		}

		void setString(std::string visibleText)
		{
			text.setString(visibleText);
		}
		

		
};

bool wasLineBreakIn (std::string text)
{
    for (int i = 0; i <= text.length(); i++)
    {
        if (text[i] == '\n')
        {
            return true;
        }
    }
    return false;
}

bool textIsTooBig(sf::Text text, float windowHeight, int stringPtr)
{
	const sf::Font* font = text.getFont();
	if (text.findCharacterPos(stringPtr).y + font->getLineSpacing(text.getCharacterSize()) + text.getCharacterSize() >= windowHeight)
	{
		return true;
	}
	return false;
}

char* fileCopy(const char* filename)
{
	std::ifstream file;
    	file.open(filename);
	file.seekg(0, std::ios::end);
    	int fileSize = file.tellg();
    	file.seekg(0, std::ios::beg);
    	if (fileSize != 0)
	{
		char* res = new char[fileSize+1];
    		file.read(res, fileSize);
    		file.close();
		return res;
	}
	return NULL;
	
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080),"HT", sf::Style::Fullscreen);
    sf::Texture background;
    background.loadFromFile("paper.png", sf::IntRect(0, 0, 1920,1080));
    
    sf::Sprite wallpaper;
    wallpaper.setTexture(background);

    char* lines= fileCopy("source.txt");
    Text text;
    sf::Font bestFont;
    bestFont.loadFromFile("13496.otf");
    text.build(&bestFont, 14, sf::Color(150, 150, 150), 100, 50); 

    std::string visibleText = ("Hacker Typer, Version 1.0. ChineseArtemy, SweetyBalls Entertainment. Press any key to start hacking...\n\n");
    text.setString(visibleText);

    int stringPtr = visibleText.length();
    float posY = text.text.getPosition().y;
    float windowHeight = window.getSize().y;
    std::string appendedPiece;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
		    sf::Keyboard keyboard;
		    if (keyboard.isKeyPressed(sf::Keyboard::Escape))
    		    {
			window.close();
			break;			
		    }
                    visibleText.append(lines, stringPtr, 2);
		    appendedPiece = visibleText.substr(stringPtr, 2);
		    stringPtr += 2;
            }
        }

        if (textIsTooBig(text.text, windowHeight, stringPtr))
	{
                posY -= text.text.getCharacterSize();
		text.text.setPosition(100, posY);
	}

       	text.setString(visibleText);

        window.clear();
	window.draw(wallpaper);
	window.draw(text.text);
        window.display();
    }

    delete [] lines;

    return 0;
}

