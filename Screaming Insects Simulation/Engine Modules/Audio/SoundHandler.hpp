#ifndef __SOUND_HANDLER_H__
#define __SOUND_HANDLER_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Audio.hpp>

// name of a type of sound, so for example, "AR-15 Shot", "Human Footstep"
typedef std::string SoundTypeName;
// vector of a bunch of sound instances that have the given name
typedef std::vector<sf::Sound> SoundTypeVector;
// contains vectors of sounds, where each sub vector has a corresponding name.
typedef std::unordered_map<SoundTypeName, SoundTypeVector> SoundUMap;


// class that simplifies the playing of sounds, so, for example, you can load bunch of sounds with the name "Human Footstep", and then retrieve one of the sounds at random.
class SoundHandler {
	static SoundUMap soundUMap;
public:

	// returns whether the given sound type exists
	static bool soundTypeExists(SoundTypeName name);
	
	// gets the number of sounds of the given type
	static uint16_t soundTypeCountGet(SoundTypeName typeName);
	
	static const SoundTypeVector& soundTypeVectorGet(SoundTypeName name);
	
	// returns a random sound of the given type
	static sf::Sound soundGetOfType(SoundTypeName typeName);
	
	// offsets the pitch of a given sound in a range
	static void soundPitchOffset(sf::Sound& sound, float min, float max);
	// offsets the volume of a given sound in a range
	static void soundVolumeOffset(sf::Sound& sound, float min, float max);

	// returns a random sound of the given type with a pitch offset with min and max
	static sf::Sound soundGetOfTypeOffsetPitch(SoundTypeName typeName, float min, float max);
	// returns a random sound of the given type with a volume offset with min and max
	static sf::Sound soundGetOfTypeOffsetVolume(SoundTypeName typeName, float min, float max);
	// returns a random sound of the given type with a pitch offset with pitchMin and pitchMax, and a volume offset with volumeMin, volumeMax
	static sf::Sound soundGetOfTypeOffsetPitchAndVolume(SoundTypeName typeName, float pitchMin, float pitchMax, float volumeMin, float volumeMax);

	// adds a new type of sound to the soundUMap
	static void soundTypeAdd(SoundTypeName name);

	static void soundAdd(SoundTypeName typeName, sf::Sound sound);
};


#endif