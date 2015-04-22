#pragma once

#include <map>

#include "global.h"
#include "AnimatedSprite.h"

// this enum is used for animations and used by all game objects
enum class GameObjectState
{
	Idle, // used by nearly every game object
	Idle_up, // used by map main character
	Idle_down, // used by map main character
	Idle_left, // used by map main character
	Idle_right, // used by map main character
	Walking, // used by main character and its weapons
	Walking_up, // used by map main character
	Walking_down, // used by map main character
	Walking_right, // used by map main character
	Walking_left, // used by map main character
	Jumping, // used by main character and its weapons
	Fighting, // used by main character and its weapons
	Frozen, // used by water tile
	Crumbling // used by crumbly block tile
};

// A game object with animations, position, bounding box, game state.
class GameObject 
{
public:
	// \brief loads animations using the resource manager
	// also defines bounding box and sprite offset
	virtual void load() = 0;
	virtual void update(sf::Time& frameTime);
	virtual void render(sf::RenderTarget& renderTarget) const;

	void setBoundingBox(const sf::FloatRect& rect);
	void setPosition(const sf::Vector2f& pos);
	void setPositionX(const float posX);
	void setPositionY(const float posY);
	const sf::Vector2f& getPosition();
	sf::FloatRect* getBoundingBox();
	sf::Vector2f getCenter();

	void setCurrentAnimation(Animation* animation, bool isFlipped);
	void addAnimation(GameObjectState state, Animation& animation);
	void setSpriteOffset(const sf::Vector2f& spriteOffset);
	void setFrameTime(sf::Time time);
	void playCurrentAnimation(bool play);
	void loopCurrentAnimation(bool loop);
	Animation* getAnimation(GameObjectState state);

protected:
	GameObjectState m_state;
	
private:
	std::map<GameObjectState, Animation> m_animations;
	AnimatedSprite m_animatedSprite;
	sf::Vector2f m_spriteOffset;
	sf::FloatRect m_boundingBox;
	// absolute position as seen from the upper left corner
	sf::Vector2f m_position;
};