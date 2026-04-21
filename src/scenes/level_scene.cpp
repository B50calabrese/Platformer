#include "scenes/level_scene.h"

#include <engine/util/logger.h>

namespace platformer {

LevelScene::LevelScene(int level_index)
    : engine::Scene("LevelScene"), level_index_(level_index) {}

void LevelScene::OnAttach() {
  LOG_INFO("LevelScene Attached - Level %d", level_index_);
}

void LevelScene::OnDetach() {
  LOG_INFO("LevelScene Detached");
}

void LevelScene::OnUpdate(float delta_time_seconds) {
  // Gameplay logic will go here.
}

void LevelScene::OnRender() {
  // Level rendering will go here.
}

}  // namespace platformer
