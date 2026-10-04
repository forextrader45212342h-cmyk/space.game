# Content/Python/generate_ultra_materials.py
# --------------------------------------------------
# This script creates four high‑quality materials in Unreal Engine 5:
#   1. Master Lumen Nanite Material
#   2. KOBAYASHI CORP Emissive Neon Material
#   3. Wet Street PBR with Puddles
#   4. NECTAR CAFE Sunny Material
#
# All assets are created under /Game/Python/GeneratedMaterials
# --------------------------------------------------

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def make_directory(path):
    """Create a directory in the content browser if it doesn't exist."""
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)

def create_asset(name, package_path, asset_class, factory):
    """Create an asset of the given class using the provided factory."""
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    return asset_tools.create_asset(name, package_path, None, factory)

def create_texture_8k(name, package_path, color=(255, 255, 255)):
    """Create a blank 8K texture with the specified color."""
    tex_factory = unreal.Texture2DFactory()
    tex_factory.set_editor_property('compression_settings', unreal.TextureCompressionSettings.TC_Default)
    tex = create_asset(name, package_path, unreal.Texture2D, tex_factory)
    if tex:
        # Set the texture size to 8192x8192
        tex.set_editor_property('source', unreal.TextureSource())
        tex.source.init_uncompressed(8192, 8192, 1, unreal.TextureSourceFormat.RGBA8)
        # Fill with the specified color
        r, g, b = color
        pixel_data = bytes([r, g, b, 255] * 8192 * 8192)
        tex.source.set_pixels(pixel_data)
        tex.post_edit_change()
        tex.mark_package_dirty()
    return tex

def create_material(name, package_path):
    """Create a new material asset."""
    mat_factory = unreal.MaterialFactoryNew()
    return create_asset(name, package_path, unreal.Material, mat_factory)

def set_material_properties(mat, **kwargs):
    """Set multiple material properties at once."""
    for prop, value in kwargs.items():
        unreal.MaterialEditingLibrary.set_material_property(mat, prop, value)

def create_texture_sample(mat, tex):
    """Create a texture sample node and return it."""
    tex_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionTextureSample)
    tex_node.texture = tex
    return tex_node

def create_constant3(mat, color):
    """Create a constant 3‑vector node."""
    const_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant3Vector)
    const_node.const = unreal.LinearColor(*color)
    return const_node

def create_constant(mat, value):
    """Create a constant node."""
    const_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant)
    const_node.const = value
    return const_node

def connect(mat, src, src_index, dst, dst_index):
    """Connect two material nodes."""
    unreal.MaterialEditingLibrary.connect_material_expressions(mat, src, src_index, dst, dst_index)

def compile(mat):
    """Compile the material."""
    unreal.MaterialEditingLibrary.compile_material(mat)

# ------------------------------------------------------------------
# Main script
# ------------------------------------------------------------------
def main():
    # Base path for all assets
    base_path = "/Game/Python/GeneratedMaterials"
    make_directory(base_path)

    # ------------------------------------------------------------------
    # 1. Master Lumen Nanite Material
    # ------------------------------------------------------------------
    master_mat = create_material("Master_LumenNanite", base_path)

    # Set core properties
    set_material_properties(
        master_mat,
        unreal.MaterialProperty.MATERIAL_DOMAIN: unreal.MaterialDomain.MATERIAL_DOMAIN_SURFACE,
        unreal.MaterialProperty.SHADING_MODEL: unreal.ShadingModel.SMOOTH,
        unreal.MaterialProperty.BLEND_MODE: unreal.BlendMode.BLEND_Opaque,
        unreal.MaterialProperty.LUMEN_REFLECTIONS: True,
        unreal.MaterialProperty.LUMEN_GLOBAL_ILLUMINATION: True,
        unreal.MaterialProperty.NANITE: True
    )

    # Create placeholder textures
    base_tex = create_texture_8k("Master_BaseColor", base_path, (255, 255, 255))
    rough_tex = create_texture_8k("Master_Roughness", base_path, (128, 128, 128))
    metal_tex = create_texture_8k("Master_Metallic", base_path, (0, 0, 0))
    normal_tex = create_texture_8k("Master_Normal", base_path, (128, 128, 255))

