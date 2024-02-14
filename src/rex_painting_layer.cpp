#include "rex_painting_layer.hpp"

using namespace godot;

void RexPaintingLayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_size", "size"), &RexPaintingLayer::set_size);
    ClassDB::bind_method(D_METHOD("get_size"), &RexPaintingLayer::get_size);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2I, "size"), "set_size", "get_size");
    ClassDB::bind_method(D_METHOD("set_characters", "characters"), &RexPaintingLayer::set_characters);
    ClassDB::bind_method(D_METHOD("get_characters"), &RexPaintingLayer::get_characters);
    ADD_PROPERTY(
        PropertyInfo(
            Variant::ARRAY, "characters", PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
            vformat("%s/%s:%s", Variant::OBJECT, PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                    AsciiTileMapCharacter::get_class_static())),
        "set_characters", "get_characters");
}

void RexPaintingLayer::set_size(Vector2i p_size) {
    size = p_size;
}

Vector2i RexPaintingLayer::get_size() const {
    return size;
}

void RexPaintingLayer::set_characters(TypedArray<AsciiTileMapCharacter> p_characters) {
    characters = p_characters;
}

TypedArray<AsciiTileMapCharacter> RexPaintingLayer::get_characters() const {
    return characters;
}
