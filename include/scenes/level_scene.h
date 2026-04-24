#ifndef INCLUDE_SCENES_LEVEL_SCENE_H_
#define INCLUDE_SCENES_LEVEL_SCENE_H_

#include <engine/ecs/entity_manager.h>
#include <engine/scene/scene.h>

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
  void LoadLevel(int index);

  int level_index_;
  engine::ecs::EntityID player_entity_ = 0;
  bool is_level_loaded_ = false;
  bool level_complete_ = false;

  // Level configuration
  struct LevelConfig {
    std::string name;
    float gravity = 1500.0f;
    float acceleration = 2000.0f;
    float deceleration = 2500.0f;
    float max_speed = 400.0f;
    float jump_force = 600.0f;
    int width = 0;
    int height = 0;
  } config_;

  const float kTileSize = 40.0f;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_LEVEL_SCENE_H_
