#include "SpellCreators/WindGustSpellCreator.h"
#include "Screens/LevelScreen.h"

WindGustSpellCreator::WindGustSpellCreator(const SpellBean &spellBean, LevelMovableGameObject *owner) : SpellCreator(spellBean, owner)
{
	m_allowedModifiers.push_back(SpellModifierType::Duration);
	m_allowedModifiers.push_back(SpellModifierType::Range);
	m_allowedModifiers.push_back(SpellModifierType::Strength);
}

void WindGustSpellCreator::executeSpell(const sf::Vector2f &target)
{
	SpellBean spellBean = m_spellBean;
	updateDamage(spellBean);
	WindGustSpell* newSpell = new WindGustSpell();
	newSpell->load(spellBean, m_owner, target, 0);
	m_screen->addObject(newSpell);
	m_owner->setFightAnimationTime();
}

void WindGustSpellCreator::addRangeModifier(int level)
{
	m_spellBean.boundingBox.width += m_spellBean.rangeModifierAddition * level;
}

void WindGustSpellCreator::addStrengthModifier(int level)
{
	// TODO. maybe this pushes faster, and maybe it pushes other "blocks"
}