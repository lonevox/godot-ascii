#include "rex_painting.hpp"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "ascii_tile_map_character.hpp"

using namespace godot;

void RexPainting::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_format_version", "version"), &RexPainting::set_format_version);
    ClassDB::bind_method(D_METHOD("get_format_version"), &RexPainting::get_format_version);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "format_version"), "set_format_version", "get_format_version");
    ClassDB::bind_method(D_METHOD("set_number_of_layers", "number_of_layers"), &RexPainting::set_number_of_layers);
    ClassDB::bind_method(D_METHOD("get_number_of_layers"), &RexPainting::get_number_of_layers);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "number_of_layers"), "set_number_of_layers", "get_number_of_layers");
    ClassDB::bind_method(D_METHOD("set_layers", "layers"), &RexPainting::set_layers);
    ClassDB::bind_method(D_METHOD("get_layers"), &RexPainting::get_layers);
    ADD_PROPERTY(
        PropertyInfo(
            Variant::ARRAY, "layers", PropertyHint::PROPERTY_HINT_ARRAY_TYPE,
            vformat("%s/%s:%s", Variant::OBJECT, PropertyHint::PROPERTY_HINT_RESOURCE_TYPE,
                    RexPaintingLayer::get_class_static())),
        "set_layers", "get_layers");
}

void RexPainting::set_format_version(int p_format_version) {
    format_version = p_format_version;
}

int RexPainting::get_format_version() const {
    return format_version;
}

void RexPainting::set_number_of_layers(int p_number_of_layers) {
    number_of_layers = p_number_of_layers;
}

int RexPainting::get_number_of_layers() const {
    return number_of_layers;
}

void RexPainting::set_layers(TypedArray<RexPaintingLayer> p_layers) {
    layers = p_layers;
}

TypedArray<RexPaintingLayer> RexPainting::get_layers() const {
    return layers;
}

Ref<FileAccess> ResourceFormatLoaderRexPainting::open_external_compressed(const String &path, FileAccess::ModeFlags flags, FileAccess::CompressionMode compression_mode) {
    // Get buffer from compressed file.
    Ref<FileAccess> compressed_file = FileAccess::open(path, FileAccess::ModeFlags::READ);
    int length = compressed_file->get_length();
    PackedByteArray buffer = compressed_file->get_buffer(length);
    compressed_file->close();

	// Decompress buffer and save to temporary file.
    static String temp_gd_compressed_file_path = "res://temp_external_compressed";
	Ref<FileAccess> gd_compressed_file = FileAccess::open(temp_gd_compressed_file_path, FileAccess::ModeFlags::WRITE);
    gd_compressed_file->store_buffer(buffer.decompress_dynamic(-1, compression_mode));
    gd_compressed_file->close();

    return FileAccess::open(temp_gd_compressed_file_path, flags);
}

void ResourceFormatLoaderRexPainting::close_external_compressed(Ref<FileAccess> file) {
    file->close();
    static String temp_gd_compressed_file_path = "res://temp_external_compressed";
    DirAccess::remove_absolute(temp_gd_compressed_file_path);
}

Variant ResourceFormatLoaderRexPainting::_load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode) const {
    Ref<FileAccess> file = open_external_compressed(path, FileAccess::ModeFlags::READ, FileAccess::CompressionMode::COMPRESSION_GZIP);

    Error error = FileAccess::get_open_error();
    if (error != OK) {
        return error;
    }

    Ref<RexPainting> res;
    res.instantiate();
    res->set_format_version(file->get_32());
    res->set_number_of_layers(file->get_32());
    while (file->get_position() < file->get_length()) {
        int width = file->get_32();
        int height = file->get_32();
        Ref<RexPaintingLayer> layer = memnew(RexPaintingLayer);
        layer->set_size(Vector2i(width, height));
        for (size_t i = 0; i < width * height; i++) {
            int ascii_code = file->get_32();
            Color fg_color = Color();
            fg_color.set_r8(file->get_8());
            fg_color.set_g8(file->get_8());
            fg_color.set_b8(file->get_8());
            fg_color.a = 1;
            Color bg_color = Color();
            bg_color.set_r8(file->get_8());
            bg_color.set_g8(file->get_8());
            bg_color.set_b8(file->get_8());
            bg_color.a = 1;
            if (bg_color.get_r8() == 255 && bg_color.get_g8() == 0 && bg_color.get_b8() == 255) {  // Background of (255, 0, 255) is transparent.
                bg_color.a = 0;
            }

            Ref<AsciiTileMapCharacter> character = memnew(AsciiTileMapCharacter);
            character->set_ascii(ascii_code);
            character->set_foreground_color(fg_color);
            character->set_background_color(bg_color);
            layer->get_characters().append(character);
        }     
        res->get_layers().append(layer);
    }

    close_external_compressed(file);
    return res;
}

PackedStringArray ResourceFormatLoaderRexPainting::_get_recognized_extensions() const {
    PackedStringArray extensions;
    extensions.push_back("xp");
    return extensions;
}

bool ResourceFormatLoaderRexPainting::_handles_type(const StringName &type) const {
    return ClassDB::is_parent_class(type, "RexPainting");
}

String ResourceFormatLoaderRexPainting::_get_resource_type(const String &path) const {
    return "RexPainting";
}
