#include <GPNBadge.hpp>
#include <FS.h>
#include <vector>
#include "colors.h"

#include "rboot.h"
#include "rboot-api.h"

#include <gyrostick.h>

//#define DEBUG

#include "enemy.h"

#ifdef DEBUG
  #define ENEMY_COUNT 1
#else
  #define ENEMY_COUNT 6
#endif

Badge badge;
Gyrostick gyrostick;
std::vector<Enemy*> enemies(ENEMY_COUNT);
long start;

void setup() {
  badge.init();
  bno.begin();

  SPIFFS.begin();

  tft.fillScreen(BLACK);
  tft.writeFramebuffer();
  badge.setBacklight(true);

  delay(300);

  rboot_config rboot_config = rboot_get_config();
  File f = SPIFFS.open("/rom" + String(rboot_config.current_rom), "w");
  f.println("Evade!\n");

  reset();
}

void loop() {
  tft.fillScreen(BLACK);

  imu::Vector<3> coords = gyrostick.coords();

  size_t position = constrain(coords.y() - 8, 0.0, 112.0);

  tft.drawFastVLine(
   20,
   position,
   16,
   WHITE);

  #ifdef DEBUG
  tft.setCursor(2, 2);
  tft.println(position);
  #endif

  for(size_t i = 0; i < enemies.size(); i++) {
    enemies[i]->moveLeft();

    if(enemies[i]->collides(20, position)) {
      badge.setGPIO(VIBRATOR, HIGH);

      tft.fillScreen(RED);
      tft.writeFramebuffer();
      delay(100);

      reset();
    }

    if(enemies[i]->isDone()) {
      enemies[i] = enemies.back();
      enemies.pop_back();

      enemies.push_back(new Enemy(random(100, 128), random(0, 120)));
    } else {
      enemies[i]->draw(tft);
    }
  }

  long score = (millis() - start) / 1000;
  tft.setCursor(100, 2);
  tft.println(score);

  tft.writeFramebuffer();

  pixels.show();
}

void reset() {
  badge.setGPIO(VIBRATOR, LOW);
  tft.fillScreen(BLACK);

  for(size_t i = 0; i < ENEMY_COUNT; i++) {
    enemies[i] = new Enemy(random(100, 128), random(0, 120));
    enemies[i]->draw(tft);
  }

  start = millis();
}
