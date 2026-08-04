#ifndef __IMAGE_STORE_H__
#define __IMAGE_STORE_H__

#include "../../Auxiliary/FileStore.hpp"
#include <SFML/Graphics.hpp>

class ImageStore : public FileStore<sf::Image> {
public:
	ImageStore();
};

#endif