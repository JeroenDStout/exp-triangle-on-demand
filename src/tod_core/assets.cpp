#include "tod_core/assets.h"

#include <filesystem>
#include <iostream>

using namespace tod;
namespace fs = std::filesystem;

std::filesystem::path assets::main_path("");
std::filesystem::path assets::shader_path("");

auto assets::set_main_path(std::filesystem::path const &search_path) -> bool {
	// We try to find the /assets folder
	// We will walk backwards from the working directory to find it;
	// in a proper build it ought to be in the same folder, but in
	// dev it will (probably) be higher up the tree

	fs::path base_path  = fs::current_path();
	if (!search_path.empty())
	{
		if (search_path.is_relative())
		  base_path /= search_path;
		else
		  base_path = search_path;
	}
	std::cout << "Looking for asset folder starting in " << base_path << std::endl;

	assets::main_path = base_path;

	// Walk up the tree until we find the folder or fail
	for (;;) {
		main_path /= "assets";

		if (fs::exists(assets::main_path))
		  break;

		if (base_path.parent_path() == base_path) {
			std::cout << "ERROR: Could not find base asset path" << std::endl;
			return false;
		}

		base_path = base_path.parent_path();
		assets::main_path = base_path;
	}

	std::cout << "Assets located at " << assets::main_path << std::endl;
	return true;
}


auto assets::set_dep_paths() -> bool {

	// Set sound test asset path
	assets::shader_path = main_path;
	assets::shader_path /= "shaders";
	if (!fs::exists(assets::shader_path)) {
		std::cout << "ERROR: Could not find shader asset path (should be at " << assets::shader_path << ")" << std::endl;
		return false;
	}
	return true;
}