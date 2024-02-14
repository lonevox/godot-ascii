#include "chunk.hpp"
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

Ref<ShaderMaterial> Chunk::DEFAULT_ASCII_SHADER_MATERIAL;

void Chunk::initialize_statics() {
    ResourceLoader *resource_loader = ResourceLoader::get_singleton();
    DEFAULT_ASCII_SHADER_MATERIAL = resource_loader->load("res://addons/asciitilemap/ascii_tile_map_shader_material.tres");
}

Chunk::Chunk() {
    ascii_shader_material = *DEFAULT_ASCII_SHADER_MATERIAL->duplicate();
    RenderingServer *rendering_server = RenderingServer::get_singleton();
    ascii_canvas_item = rendering_server->canvas_item_create();
    rendering_server->canvas_item_set_material(ascii_canvas_item, ascii_shader_material->get_rid());
    background_canvas_item = rendering_server->canvas_item_create();

    cell_glyphs = *memnew(PackedByteArray);
    cell_glyphs.resize(256);
    cell_fg_palette_colors = *memnew(PackedByteArray);
    cell_fg_palette_colors.resize(256);
    cell_bg_palette_colors = *memnew(PackedByteArray);
    cell_bg_palette_colors.resize(256);
    cell_fg_colors = *memnew(PackedInt32Array);
    cell_fg_colors.resize(256);
    cell_fg_colors.fill(0xFFFFFFFF);
    cell_fg_colors_dirty = true;
    send_dirty_arrays();
    cell_bg_colors = *memnew(PackedInt32Array);
    cell_bg_colors.resize(256);
}

Chunk::~Chunk() {
    RenderingServer::get_singleton()->canvas_item_clear(ascii_canvas_item);
}

void Chunk::set_ascii_tileset_texture(Ref<Texture2D> p_ascii_tileset_texture) const {
    static StringName PARAM_NAME = "tileset";
    ascii_shader_material->set_shader_parameter(PARAM_NAME, p_ascii_tileset_texture);
}

void Chunk::set_glyph_size(const Vector2i &p_glyph_size) const {
    static StringName PARAM_NAME = "glyph_size";
    ascii_shader_material->set_shader_parameter(PARAM_NAME, p_glyph_size);
}

void Chunk::set_background_color(const Color &p_color) const {
    RenderingServer *rendering_server = RenderingServer::get_singleton();
    rendering_server->canvas_item_clear(background_canvas_item);
    rendering_server->canvas_item_add_rect(background_canvas_item, draw_rect, p_color);
}

void Chunk::send_dirty_arrays() {
    if (cell_glyphs_dirty)
    {
        static StringName PARAM_NAME = "tileset_ids";
        ascii_shader_material->set_shader_parameter(PARAM_NAME, cell_glyphs);
        cell_glyphs_dirty = false;
    }
    if (cell_fg_palette_colors_dirty)
    {
        static StringName PARAM_NAME = "fg_palette_ids";
        ascii_shader_material->set_shader_parameter(PARAM_NAME, cell_fg_palette_colors);
        cell_fg_palette_colors_dirty = false;
    }
    if (cell_bg_palette_colors_dirty)
    {
        static StringName PARAM_NAME = "bg_palette_ids";
        ascii_shader_material->set_shader_parameter(PARAM_NAME, cell_bg_palette_colors);
        cell_bg_palette_colors_dirty = false;
    }
    if (cell_fg_colors_dirty)
    {
        static StringName PARAM_NAME = "fg_colors";
        ascii_shader_material->set_shader_parameter(PARAM_NAME, cell_fg_colors);
        cell_fg_colors_dirty = false;
    }
    if (cell_bg_colors_dirty)
    {
        static StringName PARAM_NAME = "bg_colors";
        ascii_shader_material->set_shader_parameter(PARAM_NAME, cell_bg_colors);
        cell_bg_colors_dirty = false;
    }
}

void Chunk::set_parent_map(const RID p_parent_canvas_item) {
    auto rendering_server = RenderingServer::get_singleton();
    rendering_server->canvas_item_set_parent(ascii_canvas_item, p_parent_canvas_item);
    rendering_server->canvas_item_set_parent(background_canvas_item, p_parent_canvas_item);
}

void Chunk::draw(const Rect2 &p_rect, const Color &p_background_color) {
    draw_rect = p_rect;
    auto rendering_server = RenderingServer::get_singleton();
    rendering_server->canvas_item_clear(ascii_canvas_item);
    rendering_server->canvas_item_add_rect(ascii_canvas_item, draw_rect, Color::hex(0xFFFFFFFF));
    rendering_server->canvas_item_clear(background_canvas_item);
    rendering_server->canvas_item_add_rect(background_canvas_item, draw_rect, p_background_color);
}

void Chunk::set_cell(const Vector2i &p_coord, int p_glyph, int p_fg_color, int p_bg_color, int p_fg_palette_color, int p_bg_palette_color) {
    int index = p_coord.y * Chunk::SIZE + p_coord.x;
    if (p_glyph != -1) {
        cell_glyphs.encode_u8(index, p_glyph);
        cell_glyphs_dirty = true;
    }
    if (p_fg_color != -1) {
        cell_fg_colors.set(index, p_fg_color);
        cell_fg_colors_dirty = true;
    }
    if (p_bg_color != -1) {
        cell_bg_colors.set(index, p_bg_color);
        cell_bg_colors_dirty = true;
    }
    if (p_fg_palette_color != -1) {
        cell_fg_palette_colors.encode_u8(index, p_fg_palette_color);
        cell_fg_palette_colors_dirty = true;
    }
    if (p_bg_palette_color != -1) {
        cell_bg_palette_colors.encode_u8(index, p_bg_palette_color);
        cell_bg_palette_colors_dirty = true;
    }
}

void Chunk::set_cell_glyph(const Vector2i &p_coord, int p_glyph) {
    int index = p_coord.y * Chunk::SIZE + p_coord.x;
    cell_glyphs.encode_u8(index, p_glyph);
    cell_glyphs_dirty = true;
}

void Chunk::set_cell_foreground_color(const Vector2i &p_coord, const Color &p_color) {
    int index = p_coord.y * Chunk::SIZE + p_coord.x;
    cell_fg_colors.set(index, p_color.to_abgr32());
    cell_fg_colors_dirty = true;
}

void Chunk::set_cell_background_color(const Vector2i &p_coord, const Color &p_color) {
    int index = p_coord.y * Chunk::SIZE + p_coord.x;
    cell_bg_colors.set(index, p_color.to_abgr32());
    cell_bg_colors_dirty = true;
}

Chunk* Chunk::dummy_chunk() {
    auto chunk = memnew(Chunk);
    for (size_t col = 0; col < SIZE; col++)
    {
        for (size_t row = 0; row < SIZE; row++)
        {
            auto glyph = col + row * SIZE;
            auto fg_color = Color::from_hsv((float)(col + row) / (SIZE * 2), 1, 1);
            auto bg_color = fg_color;
            bg_color.set_h(fg_color.get_h() + 0.5);
            chunk->set_cell(Vector2i(row, col), glyph, -1, -1, fg_color.to_abgr32(), bg_color.to_abgr32());
        }
    }
    return chunk;
}
