#ifndef INCLUDE_UTIL_CONSTANTS_H_
#define INCLUDE_UTIL_CONSTANTS_H_

#include <glm/vec4.hpp>

namespace platformer {

// Visual Constants
inline const glm::vec4 kPlayerColor = {0.0f, 0.5f, 1.0f, 1.0f};      // Blue
inline const glm::vec4 kPlatformColor = {0.2f, 0.8f, 0.2f, 1.0f};    // Green
inline const glm::vec4 kPlatformBorderColor = {0.1f, 0.4f, 0.1f, 1.0f};
inline const glm::vec4 kGoalColor = {1.0f, 0.84f, 0.0f, 1.0f};      // Gold
inline const glm::vec4 kBgColor = {0.1f, 0.1f, 0.15f, 1.0f};        // Dark Gray-Blue

inline constexpr float kPlatformThickness = 0.15f;
inline constexpr float kPlayerRoundness = 0.1f;
inline constexpr float kGoalRoundness = 0.2f;

// Gameplay Constants
inline constexpr float kTileSize = 40.0f;

}  // namespace platformer

#endif  // INCLUDE_UTIL_CONSTANTS_H_
