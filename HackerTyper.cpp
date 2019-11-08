#include <SFML/Graphics.hpp>
#include <fstream>
#include <cstdlib>

class textConfig
{
	private:
		float x = 0;
		float y = 0;
    		unsigned int size = 14;
		std::string text = "HackerTyper by ChineseArtemy. 2019, All Rights Reserved.\n";
    		sf::Color color = sf::Color::Green;
	public:
		sf::Text build()
		{
			sf::Text result;
			sf::Font font;
    			font.loadFromFile("2006.ttf");
    			result.setFont(font);
    			result.setPosition(this->x, this->y);
    			result.setCharacterSize(this->size);
    			result.setFillColor(this->color);
			result.setString(this->text);
			return result;
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
    sf::RenderWindow window(sf::VideoMode(1200, 720), "SFML works!");

    char* lines;
	  lines  = fileCopy("source.txt");

    textConfig  example;
    sf::Text text = example.build(); 

    std::string visibleText = text.getString();
    int stringPtr = 0;
    float posY = text.getPosition().y;
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
                    visibleText.append(lines, stringPtr, 3);
		    appendedPiece = visibleText.substr(stringPtr, 3);
		    stringPtr += 3;
            }
        }

//	textSize = lineCounter * (font.getLineSpacing(text.getCharacterSize()) + text.getCharacterSize());

     //   if (wasLineBreakIn(appendedPiece))
       //     {
         //       lineCounter += 1;
          //  }

        if (textIsTooBig(text, windowHeight, stringPtr))
            {
                posY -= text.getCharacterSize();
                text.setPosition(0, posY);
            }

        text.setString(visibleText);

        window.clear();
        window.draw(text);
        window.display();
    }
    delete [] lines;
    return 0;
}

