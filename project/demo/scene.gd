extends Node


@onready var ascii_tile_map := $AsciiTileMap


func _ready() -> void:
	ascii_tile_map.ascii_tileset_texture = load("res://demo/polyducks_12x12.png")


func _process(delta):
	for col in range(128):
		for row in range(128):
			#var color := Color(randf(), randf(), randf(), 1).to_abgr32()
			#var alt_color := Color(randf(), randf(), randf(), 1).to_abgr32()
			ascii_tile_map.set_cell_glyph(Vector2i(col, row), randi_range(0, 255))
			ascii_tile_map.set_cell_foreground_color(Vector2i(col, row), Color(randf(), randf(), randf(), 1))
			#ascii_tile_map.set_cell_background_color(Vector2i(col, row), Color(randf(), randf(), randf(), 1))
			#ascii_tile_map.set_cell(Vector2i(col, row), randi_range(0, 255), color, alt_color)
