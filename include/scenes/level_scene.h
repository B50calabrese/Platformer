#ifndef INCLUDE_SCENES_LEVEL_SCENE_H_
#define INCLUDE_SCENES_LEVEL_SCENE_H_

#include <engine/scene/scene.h>

namespace platformer {

/**
 * @brief Scene that contains the actual gameplay logic for a level.
 */
class LevelScene : public engine::Scene {
 public:
  LevelScene();
  ~LevelScene() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_LEVEL_SCENE_H_
