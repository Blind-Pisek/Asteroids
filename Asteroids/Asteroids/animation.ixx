export module animation;

export import <SFML/Graphics.hpp>;

import <vector>;


export class Animation {
private:

    float frame, speed;
    std::vector<sf::IntRect> frames;

public:  

    sf::Sprite sprite;

    Animation() {}

    Animation(sf::Texture& t, int x, int y, int w, int h, int count, float Speed) {
        frame = 0;
        speed = Speed;

        for (int i = 0; i < count; i++)
            frames.push_back(sf::IntRect(x + i * w, y, w, h));

        sprite.setTexture(t);
        sprite.setOrigin(w / 2, h / 2);
        sprite.setTextureRect(frames[0]);
    }

    void update() {
        frame += speed;
        int n = frames.size();
        if (frame >= n) frame -= n;
        if (n > 0) sprite.setTextureRect(frames[int(frame)]);
    }

    bool isEnd() {
        return frame + speed >= frames.size();
    }

};