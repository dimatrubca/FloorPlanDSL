#include "SpriteDrawable.h"
#include "../MyResourceManager.h"

void SpriteDrawable::draw(Renderer* renderer) {
	Texture2D texture = MyResourceManager::GetTexture("bed");
	renderer->drawSprite(texture, position, size, rotate, color);
};