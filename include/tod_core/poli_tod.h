#pragma once

/*
 *  ...
 */

 #include <filesystem>
 enum SDL_GPUTextureFormat : int;

namespace tod {

    struct poli_tod_init {

        std::filesystem::path asset_main_path{};

    };

    struct poli_tod_context {

        SDL_GPUTextureFormat format;

    };

}
