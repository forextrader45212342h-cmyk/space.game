# Content/Python/generate_ultra_materials.py
# Run this script inside the Unreal Editor to automatically generate four ultra‑realistic materials.
# The materials are created in the /Game/Python/GeneratedMaterials folder.

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material_asset(name: str, package_path: str) -> unreal.Material:
    """
    Creates a new material asset at the specified package path.
    """
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material_factory = unreal.MaterialFactoryNew()
    material = asset_tools.create_asset(name, package_path, unreal.Material, material_factory)
    return material


def set_material_property(material: unreal.Material, property_name: str, value):
    """
    Sets a property on the material using the MaterialEditingLibrary.
    """
    unreal.MaterialEditingLibrary.set_material_property(material, property_name, value)


def add_constant3_vector(material: unreal.Material, name: str, value: unreal.Vector) -> unreal.MaterialExpressionConstant3Vector:
    """
    Adds a Constant3Vector expression to the material.
    """
    expr = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionConstant3Vector, -200, 0)
    expr.constant = value
    expr.set_editor_property("name", name)
    return expr


def add_scalar_parameter(material: unreal.Material, name: str, default_value: float) -> unreal.MaterialExpressionScalarParameter:
    """
    Adds a ScalarParameter expression to the material.
    """
    expr = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionScalarParameter, -200, 200)
    expr.parameter_name = name
    expr.default_value = default_value
    expr.set_editor_property("name", name)
    return expr


def add_texture_sample(material: unreal.Material, texture_path: str, name: str, pos_x: int, pos_y: int) -> unreal.MaterialExpressionTextureSample:
    """
    Adds a TextureSample expression to the material.
    """
    tex = unreal.load_asset(texture_path)
    if not tex:
        unreal.log_warning(f"Texture {texture_path} not found. Skipping texture sample.")
        return None
    expr = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionTextureSample, pos_x, pos_y)
    expr.texture = tex
    expr.set_editor_property("name", name)
    return expr


# ------------------------------------------------------------------
# Material creation functions
# ------------------------------------------------------------------
def create_master_material_lumen_nanite():
    """
    Creates a master material that uses Lumen and Nanite features.
    """
    mat = create_material_asset("Master_Lumen_Nanite", "/Game/Python/GeneratedMaterials")
    if not mat:
        unreal.log_error("Failed to create Master_Lumen_Nanite material.")
        return

    # Basic properties
    set_material_property(mat, unreal.MaterialProperty.MATERIAL_DOMAIN, unreal.MaterialDomain.SURFACE)
    set_material_property(mat, unreal.MaterialProperty.SHADING_MODEL, unreal.ShadingModel.DEFAULT)
    set_material_property(mat, unreal.MaterialProperty.BLEND_MODE, unreal.BlendMode.BLEND_Opaque)
    set_material_property(mat, unreal.MaterialProperty.TWO_SIDED, False)
    set_material_property(mat, unreal.MaterialProperty.CUSTOM_DEPTH_STENCIL, False)

    # Enable Lumen features
    set_material_property(mat, unreal.MaterialProperty.USE_LUMEN_REFLECTIONS, True)
    set_material_property(mat, unreal.MaterialProperty.USE_LUMEN_GLOBAL_ILLUMINATION, True)

    # Enable Nanite
    set_material_property(mat, unreal.MaterialProperty.USE_NANITE, True)

    # Base color: simple constant
    base_color = add_constant3_vector(mat, "BaseColor", unreal.Vector(0.8, 0.8, 0.8))
    unreal.MaterialEditingLibrary.connect_material_expressions(base_color, "RGB", mat, "Base Color")

    # Roughness
    roughness = add_scalar_parameter(mat, "Roughness", 0.5)
    unreal.MaterialEditingLibrary.connect_material_expressions(roughness, "R", mat, "Roughness")

    # Metallic
    metallic = add_scalar_parameter(mat, "Metallic", 0.0)
    unreal.MaterialEditingLibrary.connect_material_expressions(metallic, "R", mat, "Metallic")

    unreal.log("Master_Lumen_Nanite material created.")


def create_kobayashi_corp_neon_emissive():
    """
    Creates an emissive neon material for KOBAYASHI CORP branding.
    """
    mat = create_material_asset("Kobayashi_Corp_Neon", "/Game/Python/GeneratedMaterials")
    if not mat:
        unreal.log_error("Failed to create Kobayashi_Corp_Neon material.")
        return

    # Basic properties
    set_material_property(mat, unreal.MaterialProperty.MATERIAL_DOMAIN, unreal.MaterialDomain.SURFACE)
    set_material_property(mat, unreal.MaterialProperty.SHADING_MODEL, unreal.ShadingModel.UNLIT)
    set_material_property(mat, unreal.MaterialProperty.BLEND_MODE, unreal.BlendMode.BLEND_Opaque)
    set_material_property(mat, unreal.MaterialProperty.TWO_SIDED, False)

    # Emissive color: bright neon blue
    emissive = add_constant3_vector(mat, "EmissiveColor", unreal.Vector(0.0, 0.8, 1.0))
    unreal.MaterialEditingLibrary.connect_material_expressions(emissive, "RGB", mat, "Emissive Color")

    # Optional: add a simple glow multiplier
    glow = add_scalar_parameter(mat, "GlowIntensity", 5.0)
    unreal.MaterialEditingLibrary.connect_material_expressions(glow, "R", mat, "Emissive Color")

    unreal.log("Kobayashi_Corp_Neon material created.")


def create_wet_street_pbr():
    """
    Creates a wet street PBR material with puddle effect.
    """
    mat = create_material_asset("Wet_Street_PBR", "/Game/Python/GeneratedMaterials")
    if not mat:
        unreal.log_error("Failed to create Wet_Street_PBR material.")
        return

    # Basic properties
    set_material_property(mat, unreal.MaterialProperty.MATERIAL_DOMAIN, unreal.MaterialDomain.SURFACE)
    set_material_property(mat, unreal.MaterialProperty.SHADING_MODEL, unreal.ShadingModel.DEFAULT)
    set_material_property(mat, unreal.MaterialProperty.BLEND_MODE, unreal.BlendMode.BLEND_Opaque)
    set_material_property(mat, unreal.MaterialProperty.TWO_SIDED, False)

    # Base Color (use a placeholder texture)
    base_tex = add_texture_sample(mat, "/Game/StarterContent/Textures/Stone_01", "BaseColorTex", -400, -200)
    if base_tex:
        unreal.MaterialEditingLibrary.connect_material_expressions(base_tex, "RGB", mat, "Base Color")

    # Normal Map
    normal_tex = add_texture_sample(mat, "/Game/StarterContent/Textures/Stone_01_NM", "NormalTex", -400, 0)
    if normal_tex:
        unreal.MaterialEditingLibrary.connect_material_expressions(normal_tex, "RGB", mat, "Normal")

    # Roughness
    roughness = add_scalar_parameter(mat, "Roughness", 0.2)
    unreal.MaterialEditingLibrary.connect_material_expressions(roughness, "R", mat, "Roughness")

    # Metallic
    metallic = add_scalar_parameter(mat, "Metallic", 0.0)
    unreal.MaterialEditingLibrary.connect_material_expressions(metallic, "R", mat, "Metallic")

    # Puddle effect: use a simple multiply of a puddle texture with roughness
    puddle_tex = add_texture_sample(mat, "/Game/StarterContent/Textures/Water_01", "PuddleTex", -200, -200)
    if puddle_tex:
        # Multiply puddle alpha with roughness to simulate wetness
        puddle_alpha = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionTextureSampleParameter, -200, 0)
        puddle_alpha.texture = puddle_tex.texture
        puddle_alpha.parameter_name = "PuddleTex"
        puddle_alpha.set_editor_property("name", "PuddleTex")

        multiply = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionMultiply, -200, 200)
        unreal.MaterialEditingLibrary.connect_material_expressions(puddle_alpha, "R", multiply, "A")
        unreal.MaterialEditingLibrary.connect_material_expressions(roughness, "R", multiply, "B")
        unreal.MaterialEditingLibrary.connect_material_expressions(multiply, "R", mat, "Roughness")

    unreal.log("Wet_Street_PBR material created.")


def create_nectar_cafe_sunny():
    """
    Creates a sunny material for NECTAR CAFE interior.
    """
    mat = create_material_asset("Nectar_Cafe_Sunny", "/Game/Python/GeneratedMaterials")
    if not mat:
        unreal.log_error("Failed to create Nectar_Cafe_Sunny material.")
        return

    # Basic properties
    set_material_property(mat, unreal.MaterialProperty.MATERIAL_DOMAIN, unreal.MaterialDomain.SURFACE)
    set_material_property(mat, unreal.MaterialProperty.SHADING_MODEL, unreal.ShadingModel.DEFAULT)
    set_material_property(mat, unreal.MaterialProperty.BLEND_MODE, unreal.BlendMode.BLEND_Opaque)
    set_material_property(mat, unreal.MaterialProperty.TWO_SIDED, False)

    # Base Color (placeholder)
    base_tex = add_texture_sample(mat, "/Game/StarterContent/Textures/Brick_01", "BaseColorTex", -400, -200)
    if base_tex:
        unreal.MaterialEditingLibrary.connect_material_expressions(base_tex, "RGB", mat, "Base Color")

    # Normal Map
    normal_tex = add_texture_sample(mat, "/Game/StarterContent/Textures/Brick_01_NM", "NormalTex", -400, 0)
    if normal_tex:
        unreal.MaterialEditingLibrary.connect_material_expressions(normal_tex, "RGB", mat, "Normal")

    # Roughness
    roughness = add_scalar_parameter(mat, "Roughness", 0.6)
    unreal.MaterialEditingLibrary.connect_material_expressions(roughness, "R", mat, "Roughness")

    # Metallic
    metallic = add_scalar_parameter(mat, "Metallic", 0.0)
    unreal.MaterialEditingLibrary.connect_material_expressions(metallic, "R", mat, "Metallic")

    # Specular (sunny highlight)
    specular = add_scalar_parameter(mat, "Specular", 0.5)
    unreal.MaterialEditingLibrary.connect_material_expressions(specular, "R", mat, "Specular")

    unreal.log("Nectar_Cafe_Sunny material created.")


# ------------------------------------------------------------------
# Main execution
# ------------------------------------------------------------------
if __name__ == "__main__":
    # Ensure the output folder exists
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    package_path = "/Game/Python/GeneratedMaterials"
    if not unreal.EditorAssetLibrary.does_directory_exist(package_path):
        unreal.EditorAssetLibrary.make_directory(package_path)

    create_master_material_lumen_nanite()
    create_kobayashi_corp_neon_emissive()
    create_wet_street_pbr()
    create_nectar_cafe_sunny()
