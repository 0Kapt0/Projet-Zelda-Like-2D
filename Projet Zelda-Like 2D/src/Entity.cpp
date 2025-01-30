#include "../include/Entity.h"

Entity::Entity() : currentFrame(0), elapsedTime(0.0f), frameTime(0.1f), isAttacking(false) {}

void Entity::setTexture(Texture& tex, int frameWidth, int frameHeight, int totalFrames, float _frameTime) {
    shape.setTexture(&tex);
    shape.setSize(Vector2f(frameWidth, frameHeight));
    shape.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    shape.setOrigin(frameWidth/2, frameHeight/2);

    frames.clear();
    for (int i = 0; i < totalFrames; ++i) {
        frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
    }

    frameTime = _frameTime;
    shape.setTextureRect(frames[currentFrame]);
}

void Entity::animate(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime && !frames.empty()) {
        elapsedTime = 0.0f;
        currentFrame++;

        if (isAttacking && currentFrame >= frames.size() - 1) {
            isAttacking = false;
        }

        currentFrame %= frames.size();
        shape.setTextureRect(frames[currentFrame]);
    }
}


bool Entity::intersects(const Entity& other) const {
    return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
}
