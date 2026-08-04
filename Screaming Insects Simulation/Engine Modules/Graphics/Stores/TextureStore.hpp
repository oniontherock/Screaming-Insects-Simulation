#ifndef __TEXTURE_STORE_H__
#define __TEXTURE_STORE_H__

#include "../../Auxiliary/FileStore.hpp"
#include <SFML/Graphics.hpp>

class TextureStore : public ObjectStore<sf::Texture> {
public:
	TextureStore();
};

#endif