#include "Simulation.h"
#include <iostream>
Simulation::Simulation()
{

	TabArray = NULL;
	settings = NULL;
	arraySize = 0;
	cycle = 1;

}
void Simulation::SimulationMain(SimulationSettings & settings)
{

	int draw = 1;
	window.create(sf::VideoMode(1280, 850), "Symulacja Liszaja", sf::Style::Titlebar);
	window.setFramerateLimit(30);

	sf::Clock zegar;
	zegar.restart();
	LoadMedia();
	this->settings = &settings;
	this->arraySize = this->settings->GetSetting(1);
	MakeArray();
	StateFirst();
	int dlugoscCyklu = this->settings->GetSetting(7);
	while (window.isOpen())
	{
		
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		sf::Event event;
		for (int i = 0; i < 4; i++)
		{
			button[i].MouseHover(mousePos, button[i].getGlobalBounds());
		}
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				CleanUp();
				window.close();
			}
			
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			{
				CleanUp();
				window.close();
			}
			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					if (InfectWhenClicked(mousePos)) {}
					else if (button[0].contains(mousePos))
					{

					}
					else if (button[2].contains(mousePos))
						Reset();
					else if(button[3].contains(mousePos))
					{
						CleanUp();
						window.close();
					}
				}
			}
		}
		if (!window.isOpen())
			break;
		if (dlugoscCyklu >= 100 && dlugoscCyklu <= 2000)
		{
			sf::Time t = (zegar.getElapsedTime());
			std::cout << t.asSeconds() << std::endl;
			if (t.asMilliseconds() >= (float)dlugoscCyklu)
			{
				zegar.restart();
				cycle += 1;
				updateState();
			}
				
		}


		window.clear();
		window.draw(background);
		st = "Cykl " + std::to_string(this->cycle);
		cycleText.setString(st);
		window.draw(cycleText);
		for (int i = 0; i < 4; i++)
			window.draw(button[i]);
		drawing();
		window.display();
	}

}
void Simulation::StateFirst()
{
	TabArray[(int)(this->arraySize / 2) - 1][(int)(this->arraySize / 2) - 1].ChangeState(2);
}

int Simulation::randomNumber()
{
	return (rand() % (100) + 1);
}

void Simulation::Reset()
{
	for (int i = 0; i < arraySize; i++)
		for (int j = 0; j < arraySize; j++)
			TabArray[i][j].ChangeState(1);
	StateFirst();
	cycle = 1;

}

void Simulation::DynamicModification(int before, int after)
{
}

bool Simulation::InfectWhenClicked(sf::Vector2i mPos)
{
	sf::FloatRect rect;
	rect.top = rect.left = 10;
	rect.width = rect.height = arraySize * 10;

	if (rect.contains((float)mPos.x, (float)mPos.y))
	{
		TabArray[(int)(mPos.y / 10)][(int)(mPos.x / 10)].ChangeState(2);
		return true;
	}
	return false;
		
}

void Simulation::LoadMedia()
{

	bgTexture.loadFromFile("imgs/mainmenu_bg.png");
	font.loadFromFile("font/Sansita-Italic.ttf");
	background.setTexture(bgTexture);
	sf::String str[4] = { L"Start", L"Pause",L"Restart", L"Powrot" };
	cycleText.setFont(font);
	cycleText.setFillColor(sf::Color::White);
	cycleText.setPosition(1000.f, 50.f);
	cycleText.setCharacterSize(60);
	float y = 250.f;
	for (int i = 0; i < 4; i++)
	{
		button[i].LoadButtonNormal();
		button[i].setPosition(900.f, y);
		button[i].LoadMenu(str[i]);

		y += 130.f;
	}
}



void Simulation::drawing()
{
	if (TabArray == NULL)
		return;
	int n = this->arraySize;
	int dlugoscStanuZarazenia = settings->GetSetting(5);
	int dlugoscStanuOdpornosci = settings->GetSetting(6);
	int stan, dlStanu;
	float x = 0.f, y = 10.f;
	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < n; j++)
		{
			stan = TabArray[i][j].retur(1);
			x += 10.f;
			TabArray[i][j].setPosition(x, y);
			if (stan == 1)
			{
				window.draw(TabArray[i][j]);
			}
			if (stan == 2)
			{
				window.draw(TabArray[i][j]);
			}
			if (stan == 3)
			{
				window.draw(TabArray[i][j]);
			}
		}
		y += 10.f;
		x = 0.f;
	}
}

void Simulation::updateState()
{
}

void Simulation::MakeArray()
{
	int n = this->arraySize;
	this->TabArray = new Point*[n];
	for (int i = 0; i < n; i++)
		TabArray[i] = new Point[n];
}

void Simulation::CleanUp()
{
	if (TabArray)
	{
		int n = this->arraySize;
		for (int i = 0; i < n; i++)
			delete[] TabArray[i];
		delete[] TabArray;

	}
}
