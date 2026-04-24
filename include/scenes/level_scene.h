#ifndef INCLUDE_SCENES_LEVEL_SCENE_H_
#define INCLUDE_SCENES_LEVEL_SCENE_H_

#include <engine/ecs/entity_manager.h>
#include <engine/scene/scene.h>

#include "util/level_config.h"

namespace platformer {

/**
 * @brief Scene that contains the actual gameplay logic for a level.
 */
class LevelScene : public engine::Scene {
 public:
  explicit LevelScene(int level_index = 1);
  ~LevelScene() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;

 private:
  int level_index_;
  engine::ecs::EntityID player_entity_ = 0;
  bool is_level_loaded_ = false;
  bool level_complete_ = false;

  LevelConfig config_;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_LEVEL_SCENE_H_
