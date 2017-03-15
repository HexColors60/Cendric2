#include "Level/DynamicTiles/DivineTorchTile.h"
#include "GameObjectComponents/LightComponent.h"
#include "Spells/Spell.h"
#include "Registrar.h"

REGISTER_LEVEL_DYNAMIC_TILE(LevelDynamicTileID::DivineTorch, DivineTorchTile)

DivineTorchTile::DivineTorchTile(LevelScreen* levelScreen) : LevelDynamicTile(levelScreen) {
	m_lightComponent = new LightComponent(LightData(sf::Vector2f(TILE_SIZE_F / 2.f, TILE_SIZE_F / 2.f), sf::Vector2f(150.f, 150.f)), this);
	addComponent(m_lightComponent);
	loadParticleSystem();
}

void DivineTorchTile::init() {
	setSpriteOffset(sf::Vector2f(0.f, 0.f));
	setPositionOffset(sf::Vector2f(0.f, 0.f));
	setBoundingBox(sf::FloatRect(0.f, 0.f, TILE_SIZE_F, TILE_SIZE_F));
}

void DivineTorchTile::loadAnimation(int skinNr) {
	bool isOn = skinNr % 2 == 0;
	skinNr = skinNr / 2;
	int height = TILE_SIZE;
	int width = TILE_SIZE;
	const sf::Texture* tex = g_resourceManager->getTexture(getSpritePath());

	Animation* idleAnimation = new Animation(sf::seconds(10.f));
	idleAnimation->setSpriteSheet(tex);
	idleAnimation->addFrame(sf::IntRect(0, skinNr * height, TILE_SIZE, TILE_SIZE));
	idleAnimation->setLooped(false);

	addAnimation(GameObjectState::Idle, idleAnimation);

	Animation* burningAnimation = new Animation(sf::seconds(10.f));
	burningAnimation->setSpriteSheet(tex);
	burningAnimation->addFrame(sf::IntRect(0, skinNr * height, TILE_SIZE, TILE_SIZE));
	burningAnimation->setLooped(false);
	
	addAnimation(GameObjectState::Burning, burningAnimation);

	// initial values
	m_state = isOn ? GameObjectState::Burning : GameObjectState::Idle;
	m_lightComponent->setVisible(isOn);
	setCurrentAnimation(getAnimation(m_state), false);
	playCurrentAnimation(true);
}

void DivineTorchTile::onHit(Spell* spell) {
	switch (spell->getSpellID()) {
	case SpellID::Light:
		if (m_state == GameObjectState::Idle) {
			setState(GameObjectState::Burning);
			m_lightComponent->setVisible(true);
		}
		break;
	default:
		break;
	}
}

void DivineTorchTile::setPosition(const sf::Vector2f& pos) {
	LevelDynamicTile::setPosition(pos);
	updateParticleSystemPosition();
}

void DivineTorchTile::render(sf::RenderTarget& target) {
	LevelDynamicTile::render(target);
	if (m_state == GameObjectState::Burning) {
		m_ps->render(target);
	}
}

void DivineTorchTile::update(const sf::Time& frameTime) {
	LevelDynamicTile::update(frameTime);
	if (m_state == GameObjectState::Burning) {
		m_ps->update(frameTime);
	}
}

void DivineTorchTile::loadParticleSystem() {
	m_ps = new particles::TextureParticleSystem(50, g_resourceManager->getTexture(GlobalResource::TEX_PARTICLE_BLOB));
	m_ps->additiveBlendMode = true;
	m_ps->emitRate = 50.0f / 2.0f;

	// Generators
	auto spawner = m_ps->addSpawner<particles::BoxSpawner>();
	spawner->center = sf::Vector2f(getPosition().x + getBoundingBox()->width / 2.f, getPosition().y + getBoundingBox()->height / 2.f);
	spawner->size = sf::Vector2f(getBoundingBox()->width, 0.f);
	m_particleSpawner = spawner;

	auto sizeGen = m_ps->addGenerator<particles::SizeGenerator>();
	sizeGen->minStartSize = 30.f;
	sizeGen->maxStartSize = 40.f;
	sizeGen->minEndSize = 20.f;
	sizeGen->maxEndSize = 30.f;

	auto colGen = m_ps->addGenerator<particles::ColorGenerator>();
	colGen->minStartCol = sf::Color(255, 255, 204, 100);
	colGen->maxStartCol = sf::Color(255, 255, 255, 150);
	colGen->minEndCol = sf::Color(255, 255, 0, 0);
	colGen->maxEndCol = sf::Color(255, 255, 255, 0);

	auto velGen = m_ps->addGenerator<particles::AngledVelocityGenerator>();
	velGen->minAngle = 90.f;
	velGen->maxAngle = 270.f;
	velGen->minStartSpeed = 10.f;
	velGen->maxStartSpeed = 15.f;

	auto timeGen = m_ps->addGenerator<particles::TimeGenerator>();
	timeGen->minTime = 1.0f;
	timeGen->maxTime = 2.0f;

	// Updaters
	m_ps->addUpdater<particles::TimeUpdater>();
	m_ps->addUpdater<particles::ColorUpdater>();
	m_ps->addUpdater<particles::EulerUpdater>();
	m_ps->addUpdater<particles::SizeUpdater>();
}

void DivineTorchTile::updateParticleSystemPosition() {
	if (m_particleSpawner == nullptr) return;
	m_particleSpawner->center.x = getPosition().x + getBoundingBox()->width / 2;
	m_particleSpawner->center.y = getPosition().y + getBoundingBox()->height / 2;
}


std::string DivineTorchTile::getSpritePath() const {
	return "res/assets/level_dynamic_tiles/spritesheet_tiles_divinetorch.png";
}
