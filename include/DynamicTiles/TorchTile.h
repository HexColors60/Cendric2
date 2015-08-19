#pragma once

#include "global.h"
#include "DynamicTile.h"

class TorchTile : public DynamicTile
{
public:
	TorchTile(Level* level) : DynamicTile(level) {}
	void init() override;
	void load(int skinNr) override;
	void onHit(Spell* spell) override;

private:
};