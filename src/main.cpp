#include <memory>

#include <engine/core/application.h>
#include <engine/core/engine.h>
#include <engine/scene/scene_manager.h>
#include <engine/util/logger.h>

#include "scenes/main_menu_scene.h"

/**
 * @brief Basic Platformer application.
 */
class PlatformerApp : public engine::Application {
 public:
  void OnInit() override {
    LOG_INFO("Platformer Initialized");
    engine::SceneManager::Get().SetScene(
        std::make_unique<platformer::MainMenuScene>());
  }

  void OnUpdate(double delta_time_seconds) override {
    // Basic application loop - no logic for now, just keep the window open.
  }

  void OnShutdown() override {
    LOG_INFO("Platformer Shutting Down");
  }
};

int main(int argc, char** argv) {
  engine::EngineConfig config;
  config.window_title = "Platformer";
  config.window_width = 1280;
  config.window_height = 720;
#ifdef ASSETS_PATH
  config.asset_path = ASSETS_PATH;
#endif

  engine::Engine::Init(config);
  PlatformerApp app;
  app.Run();

  return 0;
}