#include "combonumber.h"
#include "../utils/defs.h"
#include <string>

combonumber::combonumber() {
  active = false;
  currentValue = 0.0;
  animationTimer = 0.0;
  animation = 0;
  active = false;
}
void combonumber::logic() {
  if (animationTimer > 0 && active) {
    animationTimer -= graphics::deltaTime / 750.0;
    
  }
}
void combonumber::render() {
  
  graphics::fonts->at(5)->render(
      renderString, 320 - (length * (1-animationTimer)*60) / 2, 
      240 - (25 * (1-animationTimer)*60) / 2, false, 255,
      255, 255, 0, false, 0, 0, 0, (1-animationTimer)*60, 640, 480,
      animationTimer <= 0.5 ? (animationTimer * 2)*NUMBERALPHA : NUMBERALPHA);

  graphics::fonts->at(5)->render(
      renderString, 320 - (length * (1 - animationTimer - 0.1) * 60) / 2,
      240 - (25 * (1 - animationTimer - 0.1) * 60) / 2, false, 255, 255, 255, 0,
      false, 0, 0, 0, (1 - animationTimer - 0.1) * 60, 640, 480,
      animationTimer <= 0.5 ? (animationTimer - 0.1 * 2)*NUMBERALPHA : NUMBERALPHA);
  
    graphics::fonts->at(5)->render(
      renderString, 320 - (length * (1-animationTimer-0.25)*60) / 2, 240 - (25 * (1-animationTimer-0.25)*60) / 2, false, 255,
      255, 255, 0, false, 0, 0, 0, (1-animationTimer-0.25)*60, 640, 480,
      animationTimer <= 0.5 ? (animationTimer-0.25 * 2)*NUMBERALPHA : NUMBERALPHA);


}
void combonumber::splash(float value) {
  std::string conversion = std::to_string(value);
  if (conversion.at(2) != '0') {
    renderString = conversion.substr(0, 3);
    length = 15 + 4 + 15;
  } else {
    renderString = conversion.at(0);
    length = 15;
  }
  animationTimer = 1;
  std::cout << renderString << "renderstring\n";
  active = true;

  
}