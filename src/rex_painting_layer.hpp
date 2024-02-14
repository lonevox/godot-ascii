#pragma once

#include <godot_cpp/classes/resource.hpp>
#include "ascii_tile_map_character.hpp"

using namespace godot;

class RexPaintingLayer : public Resource {
    GDCLASS(RexPaintingLayer, Resource)

protected:
    static void _bind_methods();

public:
    void set_size(Vector2i p_size);
    Vector2i get_size() const;
    void set_characters(TypedArray<AsciiTileMapCharacter> p_characters);
    TypedArray<AsciiTileMapCharacter> get_characters() const;

    TypedArray<AsciiTileMapCharacter> characters = TypedArray<AsciiTileMapCharacter>();

private:
    Vector2i size;
};
