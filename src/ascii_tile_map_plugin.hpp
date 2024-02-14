#pragma once

#include <godot_cpp/classes/editor_plugin.hpp>

using namespace godot;

class AsciiTileMapPlugin : public EditorPlugin {
	GDCLASS(AsciiTileMapPlugin, EditorPlugin)

protected:
    static void _bind_methods() {}

public:
    virtual bool _handles(Object *p_object) const;
};
