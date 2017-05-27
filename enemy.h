class Enemy {
 public:
  Enemy(size_t x, size_t y);
  void moveLeft();
  void draw(TFT_ILI9163C tft);
  bool isDone();
  bool collides(size_t x, size_t y);

 private:
  size_t x;
  size_t y;
  size_t height;
};

Enemy::Enemy(size_t x, size_t y) {
  this->x = x;
  this->y = y;
  this->height = 8;
}

void Enemy::moveLeft() {
  this->x = constrain(this->x - 1, 0, 128);
}

void Enemy::draw(TFT_ILI9163C tft) {
  tft.drawFastVLine(this->x, this->y - (this->height / 2), this->height, MAGENTA);

  #ifdef DEBUG
    tft.setCursor(2, 20);
    tft.print(this->x);
    tft.print(" - ");
    tft.println(this->y - (this->height / 2));
  #endif
}

bool Enemy::isDone() {
  return this->x == 0;
}

bool Enemy::collides(size_t x, size_t y) {
  size_t minY = this->y - (this->height / 2);
  size_t maxY = this->y + (this->height / 2);

  return this->x >= x - 1 && this->x <= x + 1
    && ((minY > y && minY < y + 16) || (maxY < y + 16 && maxY > y));
}
