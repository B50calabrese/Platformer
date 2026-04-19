#include "scenes/main_menu_scene.h"

#include <engine/util/logger.h>

namespace platformer {

MainMenuScene::MainMenuScene() : engine::Scene("MainMenuScene") {}

void MainMenuScene::OnAttach() {
  LOG_INFO("MainMenuScene Attached");
}

void MainMenuScene::OnDetach() {
  LOG_INFO("MainMenuScene Detached");
}

void MainMenuScene::OnUpdate(float delta_time_seconds) {
  // Logic for main menu (e.g., checking for button clicks) will go here.
}

void MainMenuScene::OnRender() {
  // Rendering for main menu will go here.
}

}  // namespace platformer
