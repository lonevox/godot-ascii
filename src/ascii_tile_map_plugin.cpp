#include "ascii_tile_map_plugin.hpp"

using namespace godot;

bool AsciiTileMapPlugin::_handles(Object* p_object) const {
    return p_object->get_class() == "RexPainting";
}
