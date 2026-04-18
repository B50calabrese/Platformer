#ifndef INCLUDE_SCENES_LEVEL_SELECT_SCENE_H_
#define INCLUDE_SCENES_LEVEL_SELECT_SCENE_H_

#include <engine/scene/scene.h>

namespace platformer {

/**
 * @brief Scene for selecting which level to play.
 */
class LevelSelectScene : public engine::Scene {
 public:
  LevelSelectScene();
  ~LevelSelectScene() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_LEVEL_SELECT_SCENE_H_
