#include "scenes/level_scene.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <engine/core/engine.h>
#include <engine/ecs/components/camera_component.h>
#include <engine/ecs/components/collider.h>
#include <engine/ecs/components/gravity.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/components/velocity.h>
#include <engine/ecs/systems/camera_system.h>
#include <engine/ecs/systems/physics_system.h>
#include <engine/graphics/ecs/sprite_render_system.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/collision.h>
#include <engine/util/logger.h>

#include "scenes/main_menu_scene.h"
#include "util/constants.h"
#include "util/level_loader.h"

namespace platformer {

LevelScene::LevelScene(int level_index)
    : engine::Scene("LevelScene"), level_index_(level_index) {}

void LevelScene::OnAttach() {
  LOG_INFO("LevelScene Attached - Level %d", level_index_);

  // Create background entity
  engine::ecs::EntityID bg = registry_.CreateEntity();
  registry_.AddComponent<engine::ecs::components::Transform>(
      bg, engine::ecs::components::Transform{{-5000, -5000}, {10000, 10000}});
  registry_.AddComponent<engine::ecs::components::Quad>(
      bg, engine::ecs::components::Quad{kBgColor, {0, 0}, -100.0f});

  if (LevelLoader::Load(level_index_, &registry_, &config_, &player_entity_)) {
    is_level_loaded_ = true;

    // Set goal callback
    engine::ecs::Registry::View<engine::ecs::components::Collider>
        collider_view = registry_.GetView<engine::ecs::components::Collider>();
    for (engine::ecs::EntityID entity : collider_view) {
      engine::ecs::components::Collider& col =
          registry_.GetComponent<engine::ecs::components::Collider>(entity);
      if (col.is_trigger) {
          col.on_collision = [this](engine::ecs::EntityID self, engine::ecs::EntityID other) {
              if (other == player_entity_) {
                  level_complete_ = true;
              }
          };
      }
    }

  } else {
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
  }
}

void LevelScene::OnDetach() {
  LOG_INFO("LevelScene Detached");
}

void LevelScene::OnUpdate(float delta_time_seconds) {
  if (!is_level_loaded_) {
    return;
  }

  if (level_complete_) {
    int next_level = level_index_ + 1;
    std::string next_filename =
        "levels/level" + std::to_string(next_level) + ".txt";
    std::string full_path =
        engine::graphics::Renderer::Get().ResolveAssetPath(next_filename);
    std::ifstream file(full_path);
    if (file.good()) {
      engine::SceneManager::Get().SetScene(
          std::make_unique<LevelScene>(next_level));
    } else {
      engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    }
    return;
  }

  engine::InputManager& input = engine::InputManager::Get();
  engine::ecs::components::Velocity& vel =
      registry_.GetComponent<engine::ecs::components::Velocity>(player_entity_);

  // Horizontal movement tuning - snapper feel
  float move_dir = 0.0f;
  if (input.IsKeyDown(engine::KeyCode::kLeft)) {
    move_dir -= 1.0f;
  }
  if (input.IsKeyDown(engine::KeyCode::kRight)) {
    move_dir += 1.0f;
  }

  if (move_dir != 0.0f) {
    vel.velocity.x += move_dir * config_.acceleration * delta_time_seconds;
  } else {
    // Snappy deceleration
    float decel_amount = config_.deceleration * delta_time_seconds;
    if (std::abs(vel.velocity.x) <= decel_amount) {
      vel.velocity.x = 0.0f;
    } else {
      vel.velocity.x -= std::copysign(decel_amount, vel.velocity.x);
    }
  }

  // Clamp speed
  if (vel.velocity.x > config_.max_speed) {
    vel.velocity.x = config_.max_speed;
  }
  if (vel.velocity.x < -config_.max_speed) {
    vel.velocity.x = -config_.max_speed;
  }

  // Better grounded check
  bool is_on_ground = false;
  if (vel.velocity.y <= 0.1f) {
    const engine::ecs::components::Transform& trans =
        registry_.GetComponent<engine::ecs::components::Transform>(
            player_entity_);
    const engine::ecs::components::Collider& col =
        registry_.GetComponent<engine::ecs::components::Collider>(
            player_entity_);

    glm::vec2 check_pos =
        trans.position + col.offset - glm::vec2(0, 2.0f);  // 2 pixels below
    glm::vec2 check_size = {col.size.x, 2.0f};

    engine::ecs::Registry::View<engine::ecs::components::Transform,
                                engine::ecs::components::Collider>
        collider_view = registry_.GetView<engine::ecs::components::Transform,
                                          engine::ecs::components::Collider>();
    for (engine::ecs::EntityID entity : collider_view) {
      if (entity == player_entity_) {
        continue;
      }
      const engine::ecs::components::Transform& p_trans =
          registry_.GetComponent<engine::ecs::components::Transform>(entity);
      const engine::ecs::components::Collider& p_col =
          registry_.GetComponent<engine::ecs::components::Collider>(entity);
      if (p_col.is_trigger) {
        continue;
      }

      if (engine::util::CheckAABB(check_pos, check_size,
                                  p_trans.position + p_col.offset,
                                  p_col.size)) {
        is_on_ground = true;
        break;
      }
    }
  }

  if (input.IsKeyPressed(engine::KeyCode::kSpace) && is_on_ground) {
    vel.velocity.y = config_.jump_force;
  }
}

void LevelScene::OnRender() {}

}  // namespace platformer
