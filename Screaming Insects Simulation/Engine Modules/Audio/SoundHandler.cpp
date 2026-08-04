#include "SoundHandler.hpp"
#include "../Auxiliary/NumberGenerator.hpp"

SoundUMap SoundHandler::soundUMap{};


bool SoundHandler::soundTypeExists(SoundTypeName name) {
	return soundUMap.find(name) != soundUMap.end();
}
const SoundTypeVector& SoundHandler::soundTypeVectorGet(SoundTypeName name) {
	return soundUMap[name];
}
void SoundHandler::soundTypeAdd(SoundTypeName name) {
	soundUMap.insert({ name, SoundTypeVector() });
}
void SoundHandler::soundAdd(SoundTypeName typeName, sf::Sound sound) {
	if (!soundTypeExists(typeName)) {
		soundTypeAdd(typeName);
	}

	soundUMap.at(typeName).push_back(sound);
}

sf::Sound SoundHandler::soundGetOfType(SoundTypeName typeName) {
	uint16_t soundVectorSize = soundTypeCountGet(typeName);

	uint16_t soundIndRand = RNGu16::getUnder(soundVectorSize);

	return soundUMap.at(typeName)[soundIndRand];
}
uint16_t SoundHandler::soundTypeCountGet(SoundTypeName typeName) {
	return static_cast<uint16_t>(soundTypeVectorGet(typeName).size());
}

void SoundHandler::soundPitchOffset(sf::Sound& sound, float min, float max) {
	sound.setPitch(sound.getPitch() + RNGf::getRange(min, max));
}
void SoundHandler::soundVolumeOffset(sf::Sound& sound, float min, float max) {
	sound.setVolume(sound.getVolume() + RNGf::getRange(min, max));
}

sf::Sound SoundHandler::soundGetOfTypeOffsetPitch(SoundTypeName typeName, float min, float max) {
	sf::Sound sound = soundGetOfType(typeName);
	soundPitchOffset(sound, min, max);
	return sound;
}
sf::Sound SoundHandler::soundGetOfTypeOffsetVolume(SoundTypeName typeName, float min, float max) {
	sf::Sound sound = soundGetOfType(typeName);
	soundVolumeOffset(sound, min, max);
	return sound;
}
sf::Sound SoundHandler::soundGetOfTypeOffsetPitchAndVolume(SoundTypeName typeName, float pitchMin, float pitchMax, float volumeMin, float volumeMax) {
	sf::Sound sound = soundGetOfType(typeName);
	soundPitchOffset(sound, pitchMin, pitchMax);
	soundVolumeOffset(sound, volumeMin, volumeMax);
	return sound;
}
