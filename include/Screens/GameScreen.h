#include "stdafx.h"

class MainCharacter;

class GameScreen : public Screen
{
public:
	GameScreen();
	~GameScreen();

	Screen* update(sf::Time frameTime) override;
	void render(sf::RenderTarget &renderTarget) override;

private:
	Level m_currentLevel;
	MainCharacter* m_mainChar;
};