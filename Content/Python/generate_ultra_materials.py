# Content/Python/generate_ultra_materials.py
# ----------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate ultra‑realistic materials.
# Creates:
#   1. Master material with Lumen Nanite ray‑traced reflections.
#   2. KOBAYASHI CORP emissive neon material.
#   3. Wet street PBR with puddles material.
#   4. NECTAR CAFE sunny material.
# ----------------------------------------------------
# Run this script from the Unreal Editor's Python console or via the
# Content Browser's "Run Python Script" button.
# ----------------------------------------------------

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material(name, package_path):
    """Create a new material asset."""
    factory = unreal.MaterialFactoryNew()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material = asset_tools.create_asset(name, package_path, unreal.Material, factory)
    return material

def add_parameter_nodes(material):
    """Add parameter nodes to the master material and connect them to the material outputs."""
    # Base Color
    base_color_tex = unreal.MaterialEditingLibrary.create_material_expression(
        material, unreal.MaterialExpressionTextureSample, 200, 200)
    base_color_tex.texture = None  # Placeholder, will be set in instances
    unreal.MaterialEditingLibrary.connect_material_property(
        base_color_tex, unreal.MaterialProperty.MP_BASE_COLOR)

    #