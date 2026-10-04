# Content/Python/generate_ultra_materials.py
# Run this script inside the Unreal Editor to generate four high‑quality materials:
# 1. Master Material – Lumen, Nanite, Ray‑traced reflections
# 2. KOBAYASHI CORP – Emissive neon
# 3. Wet Street – PBR with puddle effect
# 4. NECTAR CAFE – Sunny, warm material

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material(asset_name, package_path="/Game/GeneratedMaterials"):
    """
    Create a new material asset and return its reference.
    """
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material_factory = unreal.MaterialFactoryNew()
    asset = asset_tools.create_asset(
        asset_name,
        package_path,
        unreal.Material,
        material_factory
    )
    return asset

def set_material_properties(mat, use_nanite=True, enable_ray_tracing=True, shading_model=unreal.MaterialShadingModel.DEFAULT_LIT):
    """
    Apply common material settings.
    """
    unreal.MaterialEditingLibrary.set_material_property(mat, unreal.MaterialProperty.SHADING_MODEL, shading_model)
    unreal.MaterialEditingLibrary.set_material_property(mat, unreal.MaterialProperty.BOOL_USE_NANITE, use_nanite)
    unreal.MaterialEditingLibrary.set_material_property(mat, unreal.MaterialProperty.BOOL_ENABLE_RAY_TRACING, enable_ray_tracing)

# ------------------------------------------------------------------
# 1. Master Material – Lumen, Nanite, Ray‑traced reflections
# ------------------------------------------------------------------
def create_master_material():
    mat = create_material("Master_Lumen_Nanite_Raytraced")
    set_material_properties(mat, use_nanite=True, enable_ray_tracing=True)

    # Base color – neutral gray
    base_color = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.5, 0.5, 0.5)
    unreal.MaterialEditingLibrary.connect_material_property(base_color, unreal.MaterialProperty.BRDF_BASE_COLOR)

    # Roughness – low for glossy
    roughness = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.05)
    unreal.MaterialEditingLibrary.connect_material_property(roughness, unreal.MaterialProperty.BRDF_ROUGHNESS)

    # Metallic – 0.5
    metallic = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.5)
    unreal.MaterialEditingLibrary.connect_material_property(metallic, unreal.MaterialProperty.BRDF_METALLIC)

    # Normal – flat
    normal = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.0, 0.0, 1.0)
    unreal.MaterialEditingLibrary.connect_material_property(normal, unreal.MaterialProperty.BRDF_NORMAL)

    # Enable Lumen (global setting, but we set the material to be Lumen compatible)
    mat.set_editor_property("bUseLumen", True)

    return mat

# ------------------------------------------------------------------
# 2. KOBAYASHI CORP – Emissive neon
# ------------------------------------------------------------------
def create_kobayashi_neon_material():
    mat = create_material("Kobayashi_Corp_Neon")
    set_material_properties(mat, use_nanite=False, enable_ray_tracing=False)

    # Emissive color – neon blue
    emissive_color = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.0, 0.8, 1.0)
    unreal.MaterialEditingLibrary.connect_material_property(emissive_color, unreal.MaterialProperty.EMISSIVE_COLOR)

    # Emissive strength
    emissive_strength = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 5.0)
    unreal.MaterialEditingLibrary.connect_material_property(emissive_strength, unreal.MaterialProperty.EMISSIVE_STRENGTH)

    # Base color – dark gray
    base_color = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.1, 0.1, 0.1)
    unreal.MaterialEditingLibrary.connect_material_property(base_color, unreal.MaterialProperty.BRDF_BASE_COLOR)

    # Roughness – medium
    roughness = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.4)
    unreal.MaterialEditingLibrary.connect_material_property(roughness, unreal.MaterialProperty.BRDF_ROUGHNESS)

    # Metallic – 0.2
    metallic = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.2)
    unreal.MaterialEditingLibrary.connect_material_property(metallic, unreal.MaterialProperty.BRDF_METALLIC)

    return mat

# ------------------------------------------------------------------
# 3. Wet Street – PBR with puddle effect
# ------------------------------------------------------------------
def create_wet_street_material():
    mat = create_material("Wet_Street_PBR_Puddles")
    set_material_properties(mat, use_nanite=False, enable_ray_tracing=True)

    # Base color – dark asphalt
    base_color = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.05, 0.05, 0.05)
    unreal.MaterialEditingLibrary.connect_material_property(base_color, unreal.MaterialProperty.BRDF_BASE_COLOR)

    # Roughness – low for wet look
    roughness = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.1)
    unreal.MaterialEditingLibrary.connect_material_property(roughness, unreal.MaterialProperty.BRDF_ROUGHNESS)

    # Metallic – 0.0
    metallic = unreal.MaterialEditingLibrary.create_material_expression_constant(mat, 0.0)
    unreal.MaterialEditingLibrary.connect_material_property(metallic, unreal.MaterialProperty.BRDF_METALLIC)

    # Normal – flat
    normal = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.0, 0.0, 1.0)
    unreal.MaterialEditingLibrary.connect_material_property(normal, unreal.MaterialProperty.BRDF_NORMAL)

    # Puddle mask – use a simple noise texture (placeholder)
    puddle_mask = unreal.MaterialEditingLibrary.create_material_expression_texture_sample(mat, "/Engine/EngineResources/NoiseTexture")
    puddle_mask.set_editor_property("sampler_type", unreal.TextureSamplerType.TSM_WRAP)
    puddle_mask.set_editor_property("filter", unreal.TextureFilter.TF_ANISOTROPIC)

    # Multiply mask with roughness to create wet patches
    puddle_roughness = unreal.MaterialEditingLibrary.create_material_expression_scalar_parameter(mat, "PuddleRoughness", 0.05)
    multiply = unreal.MaterialEditingLibrary.create_material_expression_multiply(mat)
    unreal.MaterialEditingLibrary.connect_material_property(puddle_mask, unreal.MaterialProperty.MATERIAL_INPUT0, multiply, unreal.MaterialProperty.MATERIAL_INPUT0)
    unreal.MaterialEditingLibrary.connect_material_property(puddle_roughness, unreal.MaterialProperty.MATERIAL_INPUT1, multiply, unreal.MaterialProperty.MATERIAL_INPUT1)
    unreal.MaterialEditingLibrary.connect_material_property(multiply, unreal.MaterialProperty.BRDF_ROUGHNESS)

    # Emissive for puddle highlights
    emissive = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat, 0.0, 0.1, 