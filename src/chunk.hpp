#pragma once

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/variant/string_name.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/variant/rect2.hpp>

using namespace godot;

class Chunk {

private:
    Ref<ShaderMaterial> ascii_shader_material;
    RID ascii_canvas_item;
    RID background_canvas_item;
    Rect2 draw_rect = Rect2();

    PackedByteArray cell_glyphs;
    PackedByteArray cell_fg_palette_colors;
    PackedByteArray cell_bg_palette_colors;
    PackedInt32Array cell_fg_colors;
    PackedInt32Array cell_bg_colors;

    bool cell_glyphs_dirty;
    bool cell_fg_palette_colors_dirty;
    bool cell_bg_palette_colors_dirty;
    bool cell_fg_colors_dirty;
    bool cell_bg_colors_dirty;

public:
    Chunk();
	~Chunk();
    static void initialize_statics();
    static Chunk* dummy_chunk();

    static Ref<ShaderMaterial> DEFAULT_ASCII_SHADER_MATERIAL;

    /// @brief The number of cells in each Chunk.
    static const int CELLS = 256;
    /// @brief The width and height of each Chunk.
    static const int SIZE = 16;

    void set_ascii_tileset_texture(Ref<Texture2D> p_ascii_tileset_texture) const;
    void set_glyph_size(const Vector2i &p_glyph_size) const;
    void set_background_color(const Color &p_color) const;

    void send_dirty_arrays();
    void set_parent_map(const RID p_parent_canvas_item);
    void draw(const Rect2 &p_rect, const Color &p_background_color);
    
    void set_cell(const Vector2i &p_coord, int p_glyph = -1, int p_fg_color = -1, int p_bg_color = -1, int p_fg_palette_color = -1, int p_bg_palette_color = -1);
    void set_cell_glyph(const Vector2i &p_coord, int p_glyph);
    void set_cell_foreground_color(const Vector2i &p_coord, const Color &p_color);
    void set_cell_background_color(const Vector2i &p_coord, const Color &p_color);
};
