#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__

#include <SFML/Audio.hpp>
#include <vector>
#include <queue>

// simple class that holds on to sound instances so they don't get deleted
class SoundPlayer {
	// queue of sf::Sounds, sounds that aren't playing are deleted
	static std::deque<sf::Sound> sounds;
public:
	// maximum amount of sounds that can be playing at once, if there are more sounds than this playing, the oldest sounds will be deleted.
	static constexpr uint16_t soundCountMax = 256;

	static void soundsUpdate();
	static void soundPlay(sf::Sound sound);
};

#endif