# Content/Python/generate_ultra_materials.py
# Run this script inside the Unreal Editor to automatically generate
# four high‑quality materials that emulate a human artist's workflow.
# The materials are:
#   1. Master_Lumen_Nanite_Raytraced
#   2. KOBAYASHI_CORP_Emissive_Neon
#   3. Wet_Street_PBR_Puddles
#   4. NECTAR_CAFE_Sunny
#
# All materials are created under /Game/Materials/UltraMaterials

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_folder(folder_path):
    """Create a folder in the content browser if it does not exist."""
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        unreal.EditorAssetLibrary.make_directory(folder_path)

def create_material_asset(name, folder_path):
    """Create a new material asset and return the material object."""
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.MaterialFactoryNew()
    asset = asset_tools.create_asset(name, folder_path, unreal.Material, factory)
    return asset

def add_texture_sample(material, texture_path, location):
    """Add a texture sample node to the material."""
    tex_sample = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, location)
    tex = unreal.EditorAssetLibrary.load_asset(texture_path)
    if tex:
        unreal.MaterialEditingLibrary.set_material_expression_property(
            tex_sample, 'Texture', tex)
    else:
        unreal.log_warning(f"Texture not found: {texture_path}")
    return tex_sample

def add_scalar_parameter(material, name, default_value, location):
    """Add a scalar parameter node."""
    scalar = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionScalarParameter, location)
    unreal.MaterialEditingLibrary.set_material_expression_property(
        scalar, 'ParameterName', name)
    unreal.MaterialEditingLibrary.set_material_expression_property(
        scalar, 'DefaultValue', default_value)
    return scalar

def add_vector_parameter(material, name, default_value, location):
    """Add a vector parameter node."""
    vector = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionVectorParameter, location)
    unreal.MaterialEditingLibrary.set_material_expression_property(
        vector, 'ParameterName', name)
    unreal.MaterialEditingLibrary.set_material_expression_property(
        vector, 'DefaultValue', default_value)
    return vector

def connect_expression(material, output_node, input_name, input_node):
    """Connect an input of the material output node to an expression."""
    unreal.MaterialEditingLibrary.set_material_expression_input(
        output_node, input_name, input_node, 0)

def set_material_property(material, prop_name, value):
    """Set a material property via the MaterialEditingLibrary."""
    unreal.MaterialEditingLibrary.set_material_property(material, prop_name, value)

# ------------------------------------------------------------------
# Material creation functions
# ------------------------------------------------------------------
def create_master_material(folder_path):
    """Create the master material with Lumen, Nanite, and Ray‑traced reflections."""
    mat = create_material_asset('Master_Lumen_Nanite_Raytraced', folder_path)

    # Basic properties
    set_material_property(mat, 'BlendMode', unreal.BlendMode.BLEND_Opaque)
    set_material_property(mat, 'ShadingModel', unreal.MaterialShadingModel.MS_DEFAULT_LIT)
    set_material_property(mat, 'bUseRayTracing', True)          # Ray‑traced reflections
    set_material_property(mat, 'bUseRayTracingReflections', True)

    # No base color or other inputs – this is a pure master material.
    # The output node will be used by child materials.
    return mat

def create_emissive_neon_material(folder_path, master_mat):
    """Create a neon emissive material for KOBAYASHI CORP."""
    mat = create_material_asset('KOBAYASHI_CORP_Emissive_Neon', folder_path)

    # Use the master material as a base
    unreal.MaterialEditingLibrary.set_material_property(mat, 'Parent', master_mat)

    # Emissive color (neon blue)
    neon_color = unreal.LinearColor(0.0, 0.8, 1.0, 1.0)  # Cyan‑blue
    emissive_vec = add_vector_parameter(mat, 'EmissiveColor', neon_color, unreal.Vector2D(200, 200))

    # Optional: add a noise mask for a pulsing effect
    noise_tex = add_texture_sample(mat, '/Engine/EngineResources/Noise', unreal.Vector2D(400, 200))
    noise_scalar = add_scalar_parameter(mat, 'PulseSpeed', 1.0, unreal.Vector2D(600, 200))

    # Connect nodes to the material output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_expression(mat, output, 'EmissiveColor', emissive_vec)
    # Multiply emissive by noise to create pulsing
    multiply = unreal.MaterialEditingLibrary.create_material_expression(
        mat, unreal.MaterialExpressionMultiply, unreal.Vector2D(400, 400))
    unreal.MaterialEditingLibrary.set_material_expression_input(multiply, 'A', emissive_vec, 0)
    unreal.MaterialEditingLibrary.set_material_expression_input(multiply, 'B', noise_tex, 0)
    connect_expression(mat, output, 'EmissiveColor', multiply)

    # Set roughness to low for a shiny neon look
    set_material_property(mat, 'Roughness', 0.1)
    return mat

def create_wet_street_material(folder_path, master_mat):
    """Create a wet street PBR material with puddles."""
    mat = create_material_asset('Wet_Street_PBR_Puddles', folder_path)

    # Use the master material as a base
    unreal.MaterialEditingLibrary.set_material_property(mat, 'Parent', master_mat)

    # Base color (dark gray)
    base_color = unreal.LinearColor(0.15, 0.15, 0.15, 1.0)
    base_vec = add_vector_parameter(mat, 'BaseColor', base_color, unreal.Vector2D(200, 200))

    # Roughness (low for wet surface)
    rough_scalar = add_scalar_parameter(mat, 'Roughness', 0.05, unreal.Vector2D(400, 200))

    # Normal map (placeholder)
    normal_tex = add_texture_sample(mat, '/Engine/EngineResources/DefaultNormal', unreal.Vector2D(600, 200))

    # Puddle mask (use a grayscale texture)
    puddle_mask = add_texture_sample(mat, '/Game/Textures/PuddleMask', unreal.Vector2D(800, 200))

    # Wet color overlay (blueish tint)
    wet_color = unreal.LinearColor(0.0, 0.1, 0.2, 1.0)
    wet_vec = add_vector_parameter(mat, 'WetColor', wet_color, unreal.Vector2D(1000, 200))

    # Blend base color with wet color using the puddle mask
    lerp = unreal.MaterialEditingLibrary.create_material_expression(
        mat, unreal.MaterialExpressionLinearInterpolate, unreal.Vector2D(1200, 200))
    unreal.MaterialEditingLibrary.set_material_expression_input(lerp, 'A', base_vec, 0)
    unreal.MaterialEditingLibrary.set_material_expression_input(lerp, 'B', wet_vec, 0)
    unreal.MaterialEditingLibrary.set_material_expression_input(lerp, 'Alpha', puddle_mask, 0)

    # Connect to material output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_expression(mat, output, 'BaseColor', lerp)
    connect_expression(mat, output, 'Normal', normal_tex)
    connect_expression(mat, output, 'Roughness', rough_scalar)

    # Metallic set to 0
    set_material_property(mat, 'Metallic', 0.0)
    return mat

def create_nectar_cafe_material(folder_path, master_mat):
    """Create a sunny, bright material for NECTAR CAFE."""
    mat = create_material_asset('NECTAR_CAFE_Sunny', folder_path)

    # Use the master material as a base
    unreal.MaterialEditingLibrary.set_material_property(mat, 'Parent', master_mat)

    # Base color (warm yellow)
    base_color = unreal.LinearColor(1.0, 0.9, 0.6, 1.0)
    base_vec = add_vector_parameter(mat, 'BaseColor', base_color, unreal.Vector2D(200, 200))

    # Roughness (medium)
    rough_scalar = add_scalar_parameter(mat, 'Roughness', 0.4, unreal.Vector2D(400, 200))

    # Normal map (placeholder)
    normal_tex = add_texture_sample(mat, '/Engine/EngineResources/DefaultNormal', unreal.Vector2D(600, 200))

    # Emissive for subtle glow
    emissive_vec = add_vector_parameter(mat, 'EmissiveColor', unreal.LinearColor(0.1, 0.1, 0.05, 1.0), unreal.Vector