#include "ascii_tile_map.hpp"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "chunk.hpp"
#include <memory>

using namespace godot;

void AsciiTileMap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_glyph_size"), &AsciiTileMap::get_glyph_size);
	ClassDB::bind_method(D_METHOD("set_cell", "coord", "glyph", "fg_color", "bg_color", "fg_palette_color", "bg_palette_color"), &AsciiTileMap::set_cell, DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1), DEFVAL(-1));
	ClassDB::bind_method(D_METHOD("set_cell_glyph", "coord", "glyph"), &AsciiTileMap::set_cell_glyph);
	ClassDB::bind_method(D_METHOD("set_cell_foreground_color", "coord", "color"), &AsciiTileMap::set_cell_foreground_color);
	ClassDB::bind_method(D_METHOD("set_cell_background_color", "coord", "color"), &AsciiTileMap::set_cell_background_color);
	ClassDB::bind_method(D_METHOD("clear"), &AsciiTileMap::clear);
	ClassDB::bind_static_method(AsciiTileMap::get_class_static(), D_METHOD("dummy_map"), &AsciiTileMap::dummy_map);

	// Properties
	ClassDB::bind_method(D_METHOD("set_ascii_tileset_texture", "ascii_tileset_texture"), &AsciiTileMap::set_ascii_tileset_texture);
	ClassDB::bind_method(D_METHOD("get_ascii_tileset_texture"), &AsciiTileMap::get_ascii_tileset_texture);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "ascii_tileset_texture", PropertyHint::PROPERTY_HINT_RESOURCE_TYPE, "Texture2D"), "set_ascii_tileset_texture", "get_ascii_tileset_texture");
	ClassDB::bind_method(D_METHOD("set_background_color", "color"), &AsciiTileMap::set_background_color);
	ClassDB::bind_method(D_METHOD("get_background_color"), &AsciiTileMap::get_background_color);
	ADD_PROPERTY(PropertyInfo(Variant::COLOR, "background_color"), "set_background_color", "get_background_color");
}

AsciiTileMap::AsciiTileMap() {
	set_texture_filter(TextureFilter::TEXTURE_FILTER_NEAREST);
}

AsciiTileMap::~AsciiTileMap() {
}

const Vector2i AsciiTileMap::ASCII_TILESET_SIZE = {16, 16};

void AsciiTileMap::_draw() {
	for (auto &pair : dirty_chunks) {
		auto chunk = pair.value;
		chunk->send_dirty_arrays();
	}
	dirty_chunks.clear();
}

void AsciiTileMap::set_ascii_tileset_texture(const Ref<Texture2D> &p_ascii_tileset_texture) {
	ascii_tileset_texture = p_ascii_tileset_texture;
	if (ascii_tileset_texture.is_valid()) {
		glyph_size = ascii_tileset_texture->get_size() / Chunk::SIZE;
		
		// Set all chunk tileset textures.
		for (auto &pair : chunks) {
			auto chunk = pair.value;
			chunk->set_ascii_tileset_texture(ascii_tileset_texture);
			chunk->set_glyph_size(glyph_size);
			draw_chunk_to_map(chunk, pair.key);
		}
	} else {
		// Clear all chunk tileset textures.
		for (auto &pair : chunks) {
			auto chunk = pair.value;
			chunk->set_ascii_tileset_texture(nullptr);
			chunk->set_glyph_size({});
		}
	}
	queue_redraw();
}

Ref<Texture2D> AsciiTileMap::get_ascii_tileset_texture() const {
	return ascii_tileset_texture;
}

void AsciiTileMap::set_background_color(const Color &p_color) {
	background_color = p_color;
	for (auto &pair : chunks) {
		auto chunk = pair.value;
		chunk->set_background_color(p_color);
	}
}

Color AsciiTileMap::get_background_color() const {
	return background_color;
}

Vector2i AsciiTileMap::get_glyph_size() const {
	return glyph_size;
}

void AsciiTileMap::add_chunk(Chunk* p_chunk, const Vector2i &p_coord) {
	p_chunk->set_ascii_tileset_texture(ascii_tileset_texture);
	p_chunk->set_glyph_size(glyph_size);
	p_chunk->set_background_color(background_color);
	draw_chunk_to_map(p_chunk, p_coord);
	chunks.insert(p_coord, p_chunk);
}

void AsciiTileMap::remove_chunk(const Vector2i &p_coord) {
	auto chunk = chunks.get(p_coord);
	chunks.erase(p_coord);
	delete chunk;
}

void AsciiTileMap::draw_chunk_to_map(Chunk* p_chunk, const Vector2i &p_coord) {
	p_chunk->set_parent_map(get_canvas_item());
	auto size = glyph_size * Chunk::SIZE;
	auto position = p_coord * size;
	auto rect = Rect2(position, size);
	p_chunk->draw(rect, background_color);
}

Chunk* AsciiTileMap::get_chunk(const Vector2i &p_coord) {
	if (!chunks.has(p_coord)) {
		auto chunk = new Chunk();
		add_chunk(chunk, p_coord);
		return chunk;
	}
	return chunks.get(p_coord);
}

void AsciiTileMap::set_cell(const Vector2i &p_coord, int p_glyph, int p_fg_color, int p_bg_color, int p_fg_palette_color, int p_bg_palette_color) {
	auto chunk_coord = p_coord / Chunk::SIZE;
	auto chunk = get_chunk(chunk_coord);
	chunk->set_cell(p_coord % Chunk::SIZE, p_glyph, p_fg_color, p_bg_color, p_fg_palette_color, p_bg_palette_color);
	dirty_chunks.insert(chunk_coord, chunk);
	queue_redraw();
}

void AsciiTileMap::set_cell_glyph(const Vector2i &p_coord, int p_glyph) {
	auto chunk_coord = p_coord / Chunk::SIZE;
	auto chunk = get_chunk(chunk_coord);
	chunk->set_cell_glyph(p_coord % Chunk::SIZE, p_glyph);
	dirty_chunks.insert(chunk_coord, chunk);
	queue_redraw();
}

void AsciiTileMap::set_cell_foreground_color(const Vector2i &p_coord, const Color &p_color) {
	auto chunk_coord = p_coord / Chunk::SIZE;
	auto chunk = get_chunk(chunk_coord);
	chunk->set_cell_foreground_color(p_coord % Chunk::SIZE, p_color);
	dirty_chunks.insert(chunk_coord, chunk);
	queue_redraw();
}

void AsciiTileMap::set_cell_background_color(const Vector2i &p_coord, const Color &p_color) {
	auto chunk_coord = p_coord / Chunk::SIZE;
	auto chunk = get_chunk(chunk_coord);
	chunk->set_cell_background_color(p_coord % Chunk::SIZE, p_color);
	dirty_chunks.insert(chunk_coord, chunk);
	queue_redraw();
}

void AsciiTileMap::clear() {
	chunks.clear();
	dirty_chunks.clear();
}

AsciiTileMap* AsciiTileMap::dummy_map() {
	AsciiTileMap* map = memnew(AsciiTileMap);
	map->add_chunk(Chunk::dummy_chunk(), { 0, 0 });
	map->add_chunk(Chunk::dummy_chunk(), { 0, 1 });
	map->add_chunk(Chunk::dummy_chunk(), { 1, 0 });
	map->add_chunk(Chunk::dummy_chunk(), { 1, 1 });
	return map;
}
