#include "include/Game.h"
#include "include/Animation.h"

int main() {
    Animation* animation = new Animation(1);
    animation->Start();
    delete animation;
    Game* game = new Game();
    game->run();
    delete game;
    return 0;
}