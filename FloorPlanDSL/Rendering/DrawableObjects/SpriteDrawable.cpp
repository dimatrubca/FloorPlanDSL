#include "SpriteDrawable.h"
#include "../MyResourceManager.h"

void SpriteDrawable::draw(Renderer* renderer, TextRenderer* textRenderer) {
	Texture2D texture = MyResourceManager::GetTexture(getSpriteName());
	renderer->drawSprite(texture, position, size, rotate, color);
};