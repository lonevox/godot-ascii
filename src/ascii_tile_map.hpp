#pragma once

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/texture2d.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/templates/hash_map.hpp>
#include <memory>
#include "chunk.hpp"

using namespace godot;

class AsciiTileMap : public Node2D {
	GDCLASS(AsciiTileMap, Node2D)

private:
	static const Vector2i ASCII_TILESET_SIZE;
	Ref<Texture2D> ascii_tileset_texture;
	Color background_color = Color::hex(0x000000FF);
	Vector2i glyph_size = Vector2i();
	HashMap<Vector2i, Chunk*> chunks = HashMap<Vector2i, Chunk*>();
	HashMap<Vector2i, Chunk*> dirty_chunks = HashMap<Vector2i, Chunk*>();
	void draw_chunk_to_map(Chunk* p_chunk, const Vector2i &p_coord);

protected:
	static void _bind_methods();

	void add_chunk(Chunk *p_chunk, const Vector2i &p_coord);
	void remove_chunk(const Vector2i &p_coord);
	_FORCE_INLINE_ Chunk* get_chunk(const Vector2i &p_coord);

public:
	AsciiTileMap();
	~AsciiTileMap();

	void _draw() override;

	static int color_to_int(Color p_color);
	static AsciiTileMap* dummy_map();

	// Properties
	void set_ascii_tileset_texture(const Ref<Texture2D> &p_ascii_tileset_texture);
	Ref<Texture2D> get_ascii_tileset_texture() const;
	void set_background_color(const Color &p_color);
	Color get_background_color() const;

	Vector2i get_glyph_size() const;
	void set_cell(const Vector2i &p_coord, int p_glyph = -1, int p_fg_palette_color = -1, int p_bg_palette_color = -1, int p_fg_color = -1, int p_bg_color = -1);
	void set_cell_glyph(const Vector2i &p_coord, int p_glyph);
	void set_cell_foreground_color(const Vector2i &p_coord, const Color &p_color);
	void set_cell_background_color(const Vector2i &p_coord, const Color &p_color);
	void clear();
};
