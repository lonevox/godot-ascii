#pragma once

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_format_loader.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include "rex_painting_layer.hpp"

using namespace godot;

class RexPainting : public Resource {
    GDCLASS(RexPainting, Resource)

protected:
    static void _bind_methods();

public:
    void set_format_version(int p_format_version);
    int get_format_version() const;
    void set_number_of_layers(int p_format_version);
    int get_number_of_layers() const;
    void set_layers(TypedArray<RexPaintingLayer> p_layers);
    TypedArray<RexPaintingLayer> get_layers() const;

private:
    int format_version = 0;
    int number_of_layers = 0;
    TypedArray<RexPaintingLayer> layers = TypedArray<RexPaintingLayer>();
};


class ResourceFormatLoaderRexPainting : public ResourceFormatLoader {
	GDCLASS(ResourceFormatLoaderRexPainting, ResourceFormatLoader);

private:
    static Ref<FileAccess> open_external_compressed(const String &path, FileAccess::ModeFlags flags, FileAccess::CompressionMode compression_mode = (FileAccess::CompressionMode)0); // TODO: Could be moved to a Util class.
    static void close_external_compressed(Ref<FileAccess> file); // TODO: Could be moved to a Util class.

protected:
	static void _bind_methods() {}

public:
    virtual Variant _load(const String &path, const String &original_path, bool use_sub_threads, int32_t cache_mode) const override;
    virtual PackedStringArray _get_recognized_extensions() const override;
    virtual bool _handles_type(const StringName &type) const override;
    virtual String _get_resource_type(const String &path) const override;
};
