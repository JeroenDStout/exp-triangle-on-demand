#pragma once

#include <filesystem>

namespace tod::assets {

	auto set_main_path(std::filesystem::path const &) -> bool /* success */;
	auto set_dep_paths() -> bool /* success */;
	
	extern std::filesystem::path main_path;
	extern std::filesystem::path shader_path;

}