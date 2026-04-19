#include "scenes/main_menu_scene.h"

#include <GLFW/glfw3.h>

#include <engine/core/engine.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/text.h>
#include <engine/scene/scene_manager.h>
#include <engine/graphics/text_renderer.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/logger.h>

#include "scenes/level_scene.h"
#include "scenes/level_select_scene.h"

namespace platformer {

namespace {
const glm::vec4 kBackgroundColor = {0.05f, 0.05f, 0.1f, 1.0f};
const glm::vec4 kButtonColor = {0.15f, 0.15f, 0.25f, 1.0f};
const glm::vec4 kButtonHoverColor = {0.25f, 0.25f, 0.4f, 1.0f};
const glm::vec4 kButtonBorderColor = {0.4f, 0.4f, 0.6f, 1.0f};
const glm::vec4 kTextColor = {0.9f, 0.9f, 1.0f, 1.0f};
const float kButtonWidthPercent = 0.7f;
const float kButtonHeightPercent = 0.2f;
const float kButtonRoundness = 0.05f;
const float kButtonBorderThickness = 0.03f;
}  // namespace

MainMenuScene::MainMenuScene() : engine::Scene("MainMenuScene") {}

void MainMenuScene::OnAttach() {
  LOG_INFO("MainMenuScene Attached");

  engine::graphics::TextRenderer::Get().LoadFont("default", "fonts/default.ttf",
                                                 32);

  ui_render_system_ = std::make_unique<engine::ui::UIRenderSystem>();

  auto& win = engine::Engine::window();
  float width = static_cast<float>(win.width());
  float height = static_cast<float>(win.height());

  // Background
  auto background = registry_.CreateEntity();
  registry_.AddComponent<engine::ui::UITransform>(
      background, engine::ui::UITransform{{0, 0}, {width, height}});
  registry_.AddComponent<engine::ecs::components::Quad>(
      background, engine::ecs::components::Quad{kBackgroundColor, {0, 0}, 100});

  // Buttons configuration
  float button_width = width * kButtonWidthPercent;
  float button_height = height * kButtonHeightPercent;
  float x_pos = (width - button_width) / 2.0f;

  // Even spacing: 3 buttons, 4 spaces
  float total_button_height = 3.0f * button_height;
  float spacing = (height - total_button_height) / 4.0f;

  auto create_button = [&](const std::string& label, float y_pos,
                           std::function<void()> on_click) {
    auto button = registry_.CreateEntity();
    registry_.AddComponent<engine::ui::UITransform>(
        button, engine::ui::UITransform{{x_pos, y_pos},
                                        {button_width, button_height}});
    registry_.AddComponent<engine::ui::UIInteractable>(
        button, engine::ui::UIInteractable{
                    false, false, on_click, [this, button](bool hovered) {
                      if (registry_.HasComponent<engine::ecs::components::Quad>(
                              button)) {
                        auto& quad =
                            registry_.GetComponent<engine::ecs::components::Quad>(
                                button);
                        quad.color = hovered ? kButtonHoverColor : kButtonColor;
                      }
                    }});
    engine::ecs::components::Quad button_quad{kButtonColor, {0, 0}, 110,
                                              kButtonRoundness,
                                              kButtonBorderThickness,
                                              kButtonBorderColor};
    registry_.AddComponent<engine::ecs::components::Quad>(button, button_quad);

    auto text_ent = registry_.CreateEntity();
    // Center text in button
    float text_scale = 1.2f;
    float text_width = engine::graphics::TextRenderer::Get().GetTextWidth(
        "default", label, text_scale);
    float text_x = (button_width - text_width) / 2.0f;
    float text_y = button_height * 0.35f;  // Visual center for text

    registry_.AddComponent<engine::ui::UITransform>(
        text_ent,
        engine::ui::UITransform{{text_x, text_y}, {button_width, button_height}});
    registry_.AddComponent<engine::ecs::components::Text>(
        text_ent, engine::ecs::components::Text{
                      label, "default", text_scale, kTextColor, 120.0f});

    // Add to hierarchy
    engine::ui::UIHierarchy hierarchy;
    hierarchy.children.push_back(text_ent);
    registry_.AddComponent<engine::ui::UIHierarchy>(button, hierarchy);

    engine::ui::UIHierarchy text_hierarchy;
    text_hierarchy.parent = button;
    registry_.AddComponent<engine::ui::UIHierarchy>(text_ent, text_hierarchy);
  };

  create_button("New Game", spacing * 3.0f + button_height * 2.0f, []() {
    engine::SceneManager::Get().SetScene(std::make_unique<LevelScene>(1));
  });

  create_button("Level Select", spacing * 2.0f + button_height, []() {
    engine::SceneManager::Get().SetScene(std::make_unique<LevelSelectScene>());
  });

  create_button("Quit", spacing, []() {
    glfwSetWindowShouldClose(engine::Engine::window().native_handle(), GLFW_TRUE);
  });
}

void MainMenuScene::OnDetach() {
  LOG_INFO("MainMenuScene Detached");
}

void MainMenuScene::OnUpdate(float delta_time_seconds) {
  auto& win = engine::Engine::window();
  engine::ui::UILayoutSystem::Update(registry_, win.width(), win.height());
  engine::ui::UIInputSystem::Update(registry_);
  engine::ui::UISyncSystem::Update(registry_);
}

void MainMenuScene::OnRender() {
  auto& win = engine::Engine::window();
  if (ui_render_system_) {
    ui_render_system_->Render(registry_, win.width(), win.height());
  }
}

}  // namespace platformer
