#ifndef INCLUDE_UTIL_LEVEL_LOADER_H_
#define INCLUDE_UTIL_LEVEL_LOADER_H_

#include <string>
#include <vector>

#include <engine/ecs/registry.h>

#include "util/level_config.h"

namespace platformer {

/**
 * @brief Utility class for loading level files and populating the ECS registry.
 */
class LevelLoader {
 public:
  /**
   * @brief Loads a level by index.
   * @param level_index The level number to load.
   * @param registry The ECS registry to populate.
   * @param config Output parameter for level configuration.
   * @param player_entity Output parameter for the created player entity.
   * @return true if loading was successful.
   */
  static bool Load(int level_index, engine::ecs::Registry* registry,
                   LevelConfig* config, engine::ecs::EntityID* player_entity);
};

}  // namespace platformer

#endif  // INCLUDE_UTIL_LEVEL_LOADER_H_
