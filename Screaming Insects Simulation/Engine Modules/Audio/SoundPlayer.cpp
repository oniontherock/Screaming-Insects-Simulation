#include "SoundPlayer.hpp"

std::deque<sf::Sound> SoundPlayer::sounds{};

void SoundPlayer::soundsUpdate() {
	while (sounds.size() > soundCountMax) {
		sounds.pop_front();
	}

	for (int16_t i = int16_t(sounds.size() - 1); i >= 0; i--) {
		if (sounds[i].getStatus() != sf::Sound::Status::Playing) {
			sounds.erase(sounds.begin() + i);
		}
	}
}

void SoundPlayer::soundPlay(sf::Sound sound) {
	sounds.push_back(sound);
	sounds[sounds.size() - 1].play();
}

