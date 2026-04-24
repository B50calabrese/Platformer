#ifndef INCLUDE_UTIL_LEVEL_CONFIG_H_
#define INCLUDE_UTIL_LEVEL_CONFIG_H_

#include <string>

namespace platformer {

/**
 * @brief Configuration for a single level, loaded from file.
 */
struct LevelConfig {
  std::string name;
  float gravity = 1800.0f;
  float acceleration = 2500.0f;
  float deceleration = 3000.0f;
  float max_speed = 500.0f;
  float jump_force = 750.0f;
  int width = 0;
  int height = 0;
};

}  // namespace platformer

#endif  // INCLUDE_UTIL_LEVEL_CONFIG_H_
