#include "ascii_tile_map_character.hpp"

using namespace godot;

void AsciiTileMapCharacter::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_ascii", "size"), &AsciiTileMapCharacter::set_ascii);
    ClassDB::bind_method(D_METHOD("get_ascii"), &AsciiTileMapCharacter::get_ascii);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "ascii"), "set_ascii", "get_ascii");
    ClassDB::bind_method(D_METHOD("set_foreground_color", "size"), &AsciiTileMapCharacter::set_foreground_color);
    ClassDB::bind_method(D_METHOD("get_foreground_color"), &AsciiTileMapCharacter::get_foreground_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "foreground_color"), "set_foreground_color", "get_foreground_color");
    ClassDB::bind_method(D_METHOD("set_background_color", "size"), &AsciiTileMapCharacter::set_background_color);
    ClassDB::bind_method(D_METHOD("get_background_color"), &AsciiTileMapCharacter::get_background_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");
}

void AsciiTileMapCharacter::set_ascii(int p_ascii) {
    ascii = p_ascii;
}

int AsciiTileMapCharacter::get_ascii() const {
    return ascii;
}

void AsciiTileMapCharacter::set_foreground_color(Color p_color) {
    foreground_color = p_color;
}

Color AsciiTileMapCharacter::get_foreground_color() const {
    return foreground_color;
}

void AsciiTileMapCharacter::set_background_color(Color p_color) {
    background_color = p_color;
}

Color AsciiTileMapCharacter::get_background_color() const {
    return background_color;
}
