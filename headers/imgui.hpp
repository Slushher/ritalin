#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <camera.hpp>

void initIMGUI(GLFWwindow *window);
void setupIMGUI();
void drawIMGUI(int fps, glm::vec3 &selectedColor);
void shutdownIMGUI();