# Content/Python/generate_ultra_materials.py
# ------------------------------------------------------------
# This script creates four high‑quality UE5 materials:
# 1. Master_Lumen_Nanite_Raytraced
# 2. KOBAYASHI_CORP_Emissive_Neon
# 3. Wet_Street_PBR_Puddles
# 4. NECTAR_CAFE_Sunny
#
# Run this script inside the Unreal Editor (Python console or
# Content Browser > Scripts > Run Python Script).
# ------------------------------------------------------------

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material_asset(name: str, package_path: str = "/Game/Python") -> unreal.Material:
    """
    Create a new Material asset in the specified package path.
    """
    factory = unreal.MaterialFactoryNew()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material = asset_tools.create_asset(name, package_path, None, factory)
    if not material:
        unreal.log_error(f"Failed to create material asset: {name}")
    return material


def set_material_properties(
    material: unreal.Material,
    shading_model=unreal.ShadingModel.SHADING_MODEL_DEFAULT_LIT,
    blend_mode=unreal.BlendMode.BLEND_Opaque,
    enable_lumen=True,
    enable_raytracing=True,
):
    """
    Set common material properties.
    """
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MATERIAL_PROPERTY_SHADING_MODEL, shading_model
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MATERIAL_PROPERTY_BLEND_MODE, blend_mode
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MATERIAL_PROPERTY_ENABLE_LUMEN, enable_lumen
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MATERIAL_PROPERTY_ENABLE_RAYTRACING, enable_raytracing
    )


def connect_to_material(material, node, input_name: str):
    """
    Connect a node's output to a material input.
    """
    unreal.MaterialEditingLibrary.connect_material_expressions(
        material, node, material, input_name
    )


# ------------------------------------------------------------------
# Material 1: Master_Lumen_Nanite_Raytraced
# ------------------------------------------------------------------
def create_master_material():
    mat = create_material_asset("Master_Lumen_Nanite_Raytraced")
    set_material_properties(mat)

    # Base color: simple