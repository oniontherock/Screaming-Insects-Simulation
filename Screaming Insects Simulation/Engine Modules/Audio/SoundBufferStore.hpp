#ifndef __SOUND_BUFFER_STORE_H__
#define __SOUND_BUFFER_STORE_H__

#include "../Auxiliary/FileStore.hpp"
#include <SFML/Audio.hpp>

class SoundBufferStore : public FileStore<sf::SoundBuffer> {
public:
	SoundBufferStore();
};

#endif