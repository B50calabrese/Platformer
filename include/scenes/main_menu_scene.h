#ifndef INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#define INCLUDE_SCENES_MAIN_MENU_SCENE_H_

#include <engine/scene/scene.h>

namespace platformer {

/**
 * @brief Scene for the main menu of the game.
 */
class MainMenuScene : public engine::Scene {
 public:
  MainMenuScene();
  ~MainMenuScene() override = default;

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate(float delta_time_seconds) override;
  void OnRender() override;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_MAIN_MENU_SCENE_H_
