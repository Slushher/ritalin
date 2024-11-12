#include <imgui.hpp>

void initIMGUI(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void setupIMGUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void drawIMGUI(int fps)
{
    ImGui::Begin("ImGUI");
    ImGui::Text("Player XYZ : %f, %f, %f", cameraPos.x, cameraPos.y, cameraPos.z);
    ImGui::Text("Player direction : %f, %f, %f", direction.x, direction.y, direction.z);
    ImGui::Text("FPS : %i", fps);
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdownIMGUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}