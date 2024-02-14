#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/editor_plugin_registration.hpp>

#include "ascii_tile_map.hpp"
#include "chunk.hpp"
#include "rex_painting.hpp"
#include "rex_painting_layer.hpp"
#include "ascii_tile_map_character.hpp"
#include "ascii_tile_map_plugin.hpp"

using namespace godot;

static Ref<ResourceFormatLoaderRexPainting> rex_painting_loader;
//static Ref<ResourceFormatSaverJson> rex_painting_saver;

void initialize_asciitilemap_module(ModuleInitializationLevel p_level) {
	if (p_level == MODULE_INITIALIZATION_LEVEL_SCENE) {
		ClassDB::register_class<AsciiTileMap>();
		ClassDB::register_class<RexPainting>();
		ClassDB::register_class<RexPaintingLayer>();
		ClassDB::register_class<AsciiTileMapCharacter>();
		ClassDB::register_class<ResourceFormatLoaderRexPainting>();

		Chunk::initialize_statics();

		ResourceLoader *resource_loader = ResourceLoader::get_singleton();
		rex_painting_loader.instantiate();
		resource_loader->add_resource_format_loader(rex_painting_loader);
	}

	if (p_level == MODULE_INITIALIZATION_LEVEL_EDITOR) {
		ClassDB::register_class<AsciiTileMapPlugin>();
		EditorPlugins::add_by_type<AsciiTileMapPlugin>();
	}
}

void uninitialize_asciitilemap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
GDExtensionBool GDE_EXPORT asciitilemap_init(GDExtensionInterfaceGetProcAddress p_get_proc_address , GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_asciitilemap_module);
	init_obj.register_terminator(uninitialize_asciitilemap_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
