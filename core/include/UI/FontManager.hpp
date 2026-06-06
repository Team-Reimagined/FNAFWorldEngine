#pragma once

#include <string>
#include <utility>
#include <vector>

#include <imgui.h>

#include "Util/Singleton.hpp"

namespace FWE::UI {
    class FontManager : public Util::Singleton<FontManager> {
    public:
        void Init();

        ImFont* headerFont;
    };
}