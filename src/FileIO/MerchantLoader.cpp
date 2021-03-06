#include "FileIO/MerchantLoader.h"
#include "GlobalResource.h"
#include "CharacterCore.h"

using namespace luabridge;

MerchantData MerchantLoader::loadMerchant(const std::string& merchantID, const CharacterCore* core) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	MerchantData merchantData;

	std::string filename = GlobalResource::NPC_FOLDER + merchantID + "/me_" + merchantID + ".lua";

	if (luaL_dofile(L, getResourcePath(filename).c_str()) != 0) {
		g_logger->logError("MerchantLoader", "Cannot read lua script: " + getResourcePath(filename));
		return merchantData;
	}

	lua_pcall(L, 0, 0, 0);

	LuaRef multiplier = getGlobal(L, "multiplier");
	if (multiplier.isNumber()) {
		float mult = multiplier.cast<float>();
		if (mult < 1.f) {
			g_logger->logWarning("MerchantLoader", "Merchant multiplier is smaller than 1, this is not allowed, the default (1.5f) is taken instead.");
		}
		else {
			merchantData.multiplier = mult;
		}
	}

	// handle receiver
	LuaRef receiver_condition = getGlobal(L, "receiver_condition");
	if (receiver_condition.isTable()) {
		LuaRef conditionType = receiver_condition[1];
		LuaRef condition = receiver_condition[2];
		if (!conditionType.isString() || !conditionType.isString()) {
			g_logger->logError("MerchantLoader", "File [" + filename + "]: merchant condition table not resolved, wrong types.");
			return merchantData;
		}

		if (core->isConditionFulfilled(conditionType.cast<std::string>(), condition.cast<std::string>())) {
			LuaRef receiver_multiplier = getGlobal(L, "receiver_multiplier");
			if (receiver_multiplier.isNumber()) {
				float mult = receiver_multiplier.cast<float>();
				merchantData.receiver_multiplier = mult;
			}
		}
	}

	LuaRef fraction = getGlobal(L, "fraction");
	if (fraction.isString()) {
		merchantData.fraction = resolveFractionID(fraction.cast<std::string>());
	}

	LuaRef wares = getGlobal(L, "wares");
	if (wares.isTable()) {
		int i = 1; // in lua, the first element is 1, not 0. Like Eiffel haha.
		LuaRef element = wares[i];
		while (!element.isNil()) {
			LuaRef name = element[1];
			LuaRef amount = element[2];
			if (!name.isString() || !amount.isNumber()) {
				g_logger->logError("MerchantLoader", "File [" + filename + "]: wares table not resolved, no name or amount or of wrong type.");
				return merchantData;
			}
			merchantData.wares.insert({ name.cast<std::string>(), amount.cast<int>() });
			i++;
			element = wares[i];
		}
	}

	LuaRef reputation = getGlobal(L, "reputation");
	if (reputation.isTable()) {
		int i = 1;
		LuaRef element = reputation[i];
		while (!element.isNil()) {
			LuaRef name = element[1];
			LuaRef amount = element[2];
			if (!name.isString() || !amount.isNumber()) {
				g_logger->logError("MerchantLoader", "File [" + filename + "]: reputation table not resolved, no name or reputation or of wrong type.");
				return merchantData;
			}
			merchantData.reputation.insert({ name.cast<std::string>(), amount.cast<int>() });
			i++;
			element = reputation[i];
		}
	}

	return merchantData;
}
