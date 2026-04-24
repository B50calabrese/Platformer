#include "util/level_loader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <engine/ecs/components/camera_component.h>
#include <engine/ecs/components/collider.h>
#include <engine/ecs/components/gravity.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/components/velocity.h>
#include <engine/graphics/renderer.h>
#include <engine/util/logger.h>

#include "util/constants.h"

namespace platformer {

bool LevelLoader::Load(int level_index, engine::ecs::Registry* registry,
                       LevelConfig* config,
                       engine::ecs::EntityID* player_entity) {
  if (!registry || !config || !player_entity) {
    return false;
  }

  std::string filename = "levels/level" + std::to_string(level_index) + ".txt";
  std::string full_path =
      engine::graphics::Renderer::Get().ResolveAssetPath(filename);
  std::ifstream file(full_path);

  if (!file.is_open()) {
    LOG_ERR("Failed to load level file: %s", full_path.c_str());
    return false;
  }

  std::string line;
  std::vector<std::string> grid;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    if (line.find("Name:") == 0) {
      config->name = line.substr(6);
    } else if (line.find("Gravity:") == 0) {
      config->gravity = std::stof(line.substr(9));
    } else if (line.find("Acceleration:") == 0) {
      config->acceleration = std::stof(line.substr(14));
    } else if (line.find("Deceleration:") == 0) {
      config->deceleration = std::stof(line.substr(14));
    } else if (line.find("MaxSpeed:") == 0) {
      config->max_speed = std::stof(line.substr(10));
    } else if (line.find("JumpForce:") == 0) {
      config->jump_force = std::stof(line.substr(11));
    } else if (line.find("Width:") == 0) {
      config->width = std::stoi(line.substr(7));
    } else if (line.find("Height:") == 0) {
      config->height = std::stoi(line.substr(8));
    } else if (line.find("Number:") == 0) {
      // ignore
    } else {
      grid.push_back(line);
    }
  }

  // Reverse grid for bottom-left origin
  std::reverse(grid.begin(), grid.end());

  // First pass: find player to initialize player_entity
  for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
    for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
      if (grid[y][x] == 'P') {
        *player_entity = registry->CreateEntity();
        glm::vec2 pos = {static_cast<float>(x) * kTileSize,
                         static_cast<float>(y) * kTileSize};
        registry->AddComponent<engine::ecs::components::Transform>(
            *player_entity,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry->AddComponent<engine::ecs::components::Velocity>(
            *player_entity, engine::ecs::components::Velocity{});
        registry->AddComponent<engine::ecs::components::Gravity>(
            *player_entity, engine::ecs::components::Gravity{config->gravity});
        registry->AddComponent<engine::ecs::components::Quad>(
            *player_entity, engine::ecs::components::Quad{
                                kPlayerColor, {0, 0}, 20.0f, kPlayerRoundness});
        registry->AddComponent<engine::ecs::components::Collider>(
            *player_entity,
            engine::ecs::components::Collider{{kTileSize, kTileSize}});
        registry->AddComponent<engine::ecs::components::CameraComponent>(
            *player_entity, engine::ecs::components::CameraComponent{
                                true, 1.0f, -640.0f, -360.0f});
      }
    }
  }

  // Second pass: platforms and goal
  for (int y = 0; y < static_cast<int>(grid.size()); ++y) {
    for (int x = 0; x < static_cast<int>(grid[y].size()); ++x) {
      char tile = grid[y][x];
      glm::vec2 pos = {static_cast<float>(x) * kTileSize,
                       static_cast<float>(y) * kTileSize};

      if (tile == '#') {
        engine::ecs::EntityID platform = registry->CreateEntity();
        registry->AddComponent<engine::ecs::components::Transform>(
            platform,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry->AddComponent<engine::ecs::components::Quad>(
            platform,
            engine::ecs::components::Quad{kPlatformColor,
                                          {0, 0},
                                          10.0f,
                                          0.0f,
                                          kPlatformThickness,
                                          kPlatformBorderColor});
        registry->AddComponent<engine::ecs::components::Collider>(
            platform, engine::ecs::components::Collider{
                          {kTileSize, kTileSize}, {0, 0}, true});
      } else if (tile == 'G') {
        engine::ecs::EntityID goal = registry->CreateEntity();
        registry->AddComponent<engine::ecs::components::Transform>(
            goal,
            engine::ecs::components::Transform{pos, {kTileSize, kTileSize}});
        registry->AddComponent<engine::ecs::components::Quad>(
            goal, engine::ecs::components::Quad{
                      kGoalColor, {0, 0}, 15.0f, kGoalRoundness});
        registry->AddComponent<engine::ecs::components::Collider>(
            goal, engine::ecs::components::Collider{
                      {kTileSize, kTileSize}, {0, 0}, true, true});
      }
    }
  }

  return true;
}

}  // namespace platformer
