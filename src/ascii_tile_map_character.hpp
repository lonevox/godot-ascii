#pragma once

#include <godot_cpp/classes/resource.hpp>

using namespace godot;

class AsciiTileMapCharacter : public Resource {
    GDCLASS(AsciiTileMapCharacter, Resource)

protected:
    static void _bind_methods();

public:
    void set_ascii(int p_ascii);
    int get_ascii() const;
    void set_foreground_color(Color p_color);
    Color get_foreground_color() const;
    void set_background_color(Color p_color);
    Color get_background_color() const;

private:
    int ascii = 0;
    Color foreground_color;
    Color background_color;
};
