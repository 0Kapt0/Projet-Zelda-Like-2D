#include "include/Entity.h"

bool Entity::intersects(const Entity& other) const {
    return shape.getGlobalBounds().intersects(other.shape.getGlobalBounds());
}
