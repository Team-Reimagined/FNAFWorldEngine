#include "UI/FontManager.hpp"

#include "Util/Logging.hpp"

#include <backends/imgui_impl_sdlrenderer3.h>

namespace FWE::UI {
    void FontManager::Init() {
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->AddFontDefault();
    }
}