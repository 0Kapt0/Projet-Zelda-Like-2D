#include "../include/Entity.h"

Entity::Entity() : currentFrame(0), elapsedTime(0.0f), frameTime(0.1f) {}

void Entity::setTexture(const string& textureFile, int frameWidth, int frameHeight, int totalFrames, float _frameTime) {
    if (!texture.loadFromFile(textureFile)) {
        cerr << "Failed to load texture: " << textureFile << endl;
        return;
    }

    shape.setSize(Vector2f(frameWidth, frameHeight));
    shape.setTexture(&texture);

    frames.clear();
    for (int i = 0; i < totalFrames; ++i) {
        frames.push_back(IntRect(i * frameWidth, 0, frameWidth, frameHeight));
    }

    shape.setTextureRect(frames[currentFrame]);
    frameTime = _frameTime;
}

void Entity::animate(float deltaTime) {
    elapsedTime += deltaTime;
    if (elapsedTime >= frameTime && !frames.empty()) {
        elapsedTime = 0.0f;
        currentFrame = (currentFrame + 1) % frames.size();
        shape.setTextureRect(frames[currentFrame]);
    }
}

void Entity::draw(RenderWindow& window) {
    window.draw(shape);
}


bool Entity::intersects(const Entity& other) const {
    return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
}
