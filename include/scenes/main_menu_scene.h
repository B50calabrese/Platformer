#ifndef INCLUDE_SCENES_MAIN_MENU_SCENE_H_
#define INCLUDE_SCENES_MAIN_MENU_SCENE_H_

#include <memory>

#include <engine/scene/scene.h>
#include <engine/ui/ui_systems.h>

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

 private:
  std::unique_ptr<engine::ui::UIRenderSystem> ui_render_system_;
};

}  // namespace platformer

#endif  // INCLUDE_SCENES_MAIN_MENU_SCENE_H_
