#include "../include/Entity.h"

Entity::Entity() : currentFrame(0), elapsedTime(0.0f), frameTime(0.1f), totalFrames(0), isAttacking(false) {}

void Entity::setTexture(Texture& tex, int frameWidth, int frameHeight, int _totalFrames, float _frameTime) {
    shape.setTexture(&tex);
    shape.setSize(Vector2f(frameWidth, frameHeight));
    shape.setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
    shape.setOrigin(frameWidth / 2, 0);

    frames.clear();
    for (int i = 0; i < _totalFrames; ++i) {
        frames.emplace_back(i * frameWidth, 0, frameWidth, frameHeight);
    }

    totalFrames = _totalFrames;
    frameTime = _frameTime;
    shape.setTextureRect(frames[currentFrame]);
}

void Entity::moveShape(float offsetX, float offsetY) {
    shape.move(offsetX, offsetY);
}

void Entity::animate(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime && !frames.empty()) {
        elapsedTime = 0.0f;

        if (isAttacking) {
            if (currentFrame < totalFrames - 1) {
                currentFrame++;
            }
            else {
                cout << "Animation terminée, blocage sur la dernière frame." << std::endl;
                return;
            }
        }
        else {
            currentFrame = (currentFrame + 1) % totalFrames;
        }

        shape.setTextureRect(frames[currentFrame]);
    }
}





bool Entity::intersects(const Entity& other) const {
    return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
}