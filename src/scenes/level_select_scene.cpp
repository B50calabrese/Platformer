#include "scenes/level_select_scene.h"

#include <engine/util/logger.h>

namespace platformer {

LevelSelectScene::LevelSelectScene() : engine::Scene("LevelSelectScene") {}

void LevelSelectScene::OnAttach() {
  LOG_INFO("LevelSelectScene Attached");
}

void LevelSelectScene::OnDetach() {
  LOG_INFO("LevelSelectScene Detached");
}

void LevelSelectScene::OnUpdate(float delta_time_seconds) {
  // Logic for level selection will go here.
}

void LevelSelectScene::OnRender() {
  // Rendering for level selection will go here.
}

}  // namespace platformer
