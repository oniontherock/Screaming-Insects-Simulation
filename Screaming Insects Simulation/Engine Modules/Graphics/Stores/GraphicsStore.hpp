#ifndef __GRAPHICS_STORE_H__
#define __GRAPHICS_STORE_H__

#include "ImageStore.hpp"
#include "TextureStore.hpp"

// holds an ImageStore and a TextureStore
namespace GraphicsStore {
	extern ImageStore imageStore;
	extern TextureStore textureStore;
}

#endif