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
#include <engine/util/collision.h>
#include <engine/graphics/ecs/sprite_render_system.h>
#include <engine/graphics/renderer.h>
#include <engine/input/input_manager.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "scenes/main_menu_scene.h"

namespace platformer {

namespace {
const glm::vec4 kPlayerColor = {0.0f, 0.5f, 1.0f, 1.0f};      // Blue
const glm::vec4 kPlatformColor = {0.2f, 0.8f, 0.2f, 1.0f};    // Green
const glm::vec4 kPlatformBorderColor = {0.1f, 0.4f, 0.1f, 1.0f};
const glm::vec4 kGoalColor = {1.0f, 0.84f, 0.0f, 1.0f};      // Gold
const glm::vec4 kBgColor = {0.1f, 0.1f, 0.15f, 1.0f};        // Dark Gray-Blue

const float kPlatformThickness = 0.15f;
const float kPlayerRoundness = 0.1f;
const float kGoalRoundness = 0.2f;
}  // namespace

LevelScene::LevelScene(int level_index)
    : engine::Scene("LevelScene"), level_index_(level_index) {}

void LevelScene::OnAttach() {
  LOG_INFO("LevelScene Attached - Level %d", level_index_);

  // Create background entity
  auto bg = registry_.CreateEntity();
  registry_.AddComponent<engine::ecs::components::Transform>(
      bg, engine::ecs::components::Transform{{-5000, -5000}, {10000, 10000}});
  registry_.AddComponent<engine::ecs::components::Quad>(
      bg, engine::ecs::components::Quad{kBgColor, {0, 0}, -100.0f});

  LoadLevel(level_index_);
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
    std::string next_filename = "levels/level" + std::to_string(next_level) + ".txt";
    std::string full_path = engine::graphics::Renderer::Get().ResolveAssetPath(next_filename);
    std::ifstream file(full_path);
    if (file.good()) {
        engine::SceneManager::Get().SetScene(std::make_unique<LevelScene>(next_level));
    } else {
        engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    }
    return;
  }

  auto& input = engine::InputManager::Get();
  auto& vel =
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
  if (vel.velocity.x > config_.max_speed) vel.velocity.x = config_.max_speed;
  if (vel.velocity.x < -config_.max_speed) vel.velocity.x = -config_.max_speed;

  // Better grounded check
  bool is_on_ground = false;
  if (vel.velocity.y <= 0.1f) {
    auto& trans = registry_.GetComponent<engine::ecs::components::Transform>(
        player_entity_);
    auto& col =
        registry_.GetComponent<engine::ecs::components::Collider>(player_entity_);

    glm::vec2 check_pos =
        trans.position + col.offset - glm::vec2(0, 2.0f);  // 2 pixels below
    glm::vec2 check_size = {col.size.x, 2.0f};

    auto collider_view =
        registry_.GetView<engine::ecs::components::Transform,
                          engine::ecs::components::Collider>();
    for (auto entity : collider_view) {
      if (entity == player_entity_) continue;
      auto& p_trans =
          registry_.GetComponent<engine::ecs::components::Transform>(entity);
      auto& p_col =
          registry_.GetComponent<engine::ecs::components::Collider>(entity);
      if (p_col.is_trigger) continue;

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

void LevelScene::OnRender() {
}

void LevelScene::LoadLevel(int index) {
  std::string filename = "levels/level" + std::to_string(index) + ".txt";
  std::string full_path =
      engine::graphics::Renderer::Get().ResolveAssetPath(filename);
  std::ifstream file(full_path);

  if (!file.is_open()) {
    LOG_ERR("Failed to load level file: %s", full_path.c_str());
    engine::SceneManager::Get().SetScene(std::make_unique<MainMenuScene>());
    return;
  }

  std::string line;
  std::vector<std::string> grid;
  while (std::getline(file, line)) {
    if (line.empty()) continue;

    if (line.find("Name:") == 0) {
      config_.name = line.substr(6);
    } else if (line.find("Gravity:") == 0) {
      config_.gravity = std::stof(line.substr(9));
    } else if (line.find("Acceleration:") == 0) {
      config_.acceleration = std::stof(line.substr(14));
    } else if (line.find("Deceleration:") == 0) {
      config_.deceleration = std::stof(line.substr(14));
    } else if (line.find("MaxSpeed:") == 0) {
      config_.max_speed = std::stof(line.substr(10));
    } else if (line.find("JumpForce:") == 0) {
      config_.jump_force = std::stof(line.substr(11));
    } else if (line.find("Width:") == 0) {
      config_.width = std::stoi(line.substr(7));
    } else if (line.find("Height:") == 0) {
      config_.height = std::stoi(line.substr(8));
    } else if (line.find("Number:") == 0) {
        // ignore
    } else {
      grid.push_back(line);
    }
  }

  // Reverse grid for bottom-left origin
  std::reverse(grid.begin(), grid.end());

  // First pass: find player to initialize player_entity_
  for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
    for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
      if (grid[y][x] == 'P') {
        player_entity_ = registry_.CreateEntity();
        glm::vec2 pos = {x * kTileSize, y * kTileSize};
        registry_.AddComponent<engine::ecs::components::Transform>(
            player_entity_,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry_.AddComponent<engine::ecs::components::Velocity>(
            player_entity_, engine::ecs::components::Velocity{});
        registry_.AddComponent<engine::ecs::components::Gravity>(
            player_entity_, engine::ecs::components::Gravity{config_.gravity});
        registry_.AddComponent<engine::ecs::components::Quad>(
            player_entity_,
            engine::ecs::components::Quad{kPlayerColor, {0, 0}, 20.0f, kPlayerRoundness});
        registry_.AddComponent<engine::ecs::components::Collider>(
            player_entity_,
            engine::ecs::components::Collider{{kTileSize, kTileSize}});
        registry_.AddComponent<engine::ecs::components::CameraComponent>(
            player_entity_, engine::ecs::components::CameraComponent{
                                true, 1.0f, -640.0f, -360.0f});
      }
    }
  }

  // Second pass: platforms and goal
  for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
    for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
      char tile = grid[y][x];
      glm::vec2 pos = {x * kTileSize, y * kTileSize};

      if (tile == '#') {
        auto platform = registry_.CreateEntity();
        registry_.AddComponent<engine::ecs::components::Transform>(
            platform,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry_.AddComponent<engine::ecs::components::Quad>(
            platform,
            engine::ecs::components::Quad{kPlatformColor, {0, 0}, 10.0f, 0.0f, kPlatformThickness, kPlatformBorderColor});
        registry_.AddComponent<engine::ecs::components::Collider>(
            platform, engine::ecs::components::Collider{
                          {kTileSize, kTileSize}, {0, 0}, true});
      } else if (tile == 'G') {
        auto goal = registry_.CreateEntity();
        registry_.AddComponent<engine::ecs::components::Transform>(
            goal,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry_.AddComponent<engine::ecs::components::Quad>(
            goal, engine::ecs::components::Quad{kGoalColor, {0, 0}, 15.0f, kGoalRoundness});
        registry_.AddComponent<engine::ecs::components::Collider>(
            goal, engine::ecs::components::Collider{
                      {kTileSize, kTileSize}, {0, 0}, true, true,
                      [this](engine::ecs::EntityID self,
                             engine::ecs::EntityID other) {
                        if (other == player_entity_) {
                          level_complete_ = true;
                        }
                      }});
      }
    }
  }

  is_level_loaded_ = true;
}

}  // namespace platformer
