#pragma once

#include "global.h"
#include "Level/Enemy.h"

class ParticleComponent;

// Superclass for bosses.
class Boss : public virtual Enemy {
public:
	Boss(const Level* level, Screen* screen);
	virtual ~Boss() {};

	void update(const sf::Time& frameTime) override;
	void setDead() override;

	int getMentalStrength() const override { return 4; };

protected:
	std::string getDeathSoundPath() const override;

	void loadDeathParticles();
	ParticleComponent* m_deathPc;

	sf::Time m_fadingTime = sf::seconds(2.f);
	sf::Time m_particleTime = sf::seconds(2.f);
};