# Content/Python/generate_ultra_materials.py
# Run this script inside the Unreal Editor to automatically generate four
# ultra‑realistic materials that emulate a human artist’s workflow.

import unreal

# ----------------------------------------------------------------------
# Utility helpers
# ----------------------------------------------------------------------
def ensure_folder(path: str):
    """
    Create the folder if it does not exist.
    """
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)

def create_material_asset(name: str, package_path: str) -> unreal.Material:
    """
    Create a new Material asset at the given package path.
    """
    factory = unreal.MaterialFactoryNew()
    asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name=name,
        package_path=package_path,
        asset_class=unreal.Material,
        factory=factory
    )
    return asset

def compile_and_save(mat: unreal.Material):
    """
    Compile the material and save the asset.
    """
    unreal.MaterialEditingLibrary.compile_material(mat)
    unreal.EditorAssetLibrary.save_asset(mat.get_path_name())

# ----------------------------------------------------------------------
# Master Material: Lumen Nanite Ray‑traced Reflections
# ----------------------------------------------------------------------
def create_master_material():
    package_path = "/Game/GeneratedMaterials"
    ensure_folder(package_path)
    mat = create_material_asset("Master_Lumen_Nanite_Raytraced", package_path)

    # Set material properties
    mat.set_shading_model(unreal.MaterialShadingModel.MSM_DEFAULT)
    mat.set_blend_mode(unreal.MaterialBlendMode.BLEND_Opaque)
    mat.set_use_lumen(True)
    mat.set_use_nanite(True)
    mat.set_enable_ray_tracing(True)

    # Base color: neutral gray
    base_color = unreal.MaterialEditingLibrary.create_material_expression_constant3_vector(mat)
    base_color.set_vector_value(unreal.LinearColor(0.5, 0.5, 0.5))
    base_color.set_editor_property("MaterialExpressionEditorX", -200)
    base_color.set_editor_property("MaterialExpressionEditorY", 0)

    # Roughness: low for glossy
    roughness = unreal.MaterialEditingLibrary.create_material_expression_scalar(mat)
    roughness.set_editor_property("MaterialExpressionEditorX", -200)
    roughness.set_editor_property("MaterialExpressionEditorY", 200)
    roughness.set_editor_property("DefaultValue", 0.05)

    # Metallic: high for metal look
    metallic = unreal.MaterialEditingLibrary.create_material_expression_scalar(mat)
    metallic.set_editor_property("MaterialExpressionEditorX", -200)
    metallic.set_editor_property("MaterialExpressionEditorY", 400)
    metallic.set_editor_property("DefaultValue", 1.0)

    # Normal: use default normal map
    normal_tex = unreal.MaterialEditingLibrary.create_material_expression_texture_sample(mat)
    normal_tex.set_editor_property("MaterialExpressionEditorX", -200)
    normal_tex.set_editor_property("MaterialExpressionEditorY", 600)
    normal_tex.set_editor_property("Texture", unreal.EditorAssetLibrary.load_asset("/Engine/EngineResources/DefaultNormal"))

    # Connect nodes to Material Output
    unreal.MaterialEditingLibrary.connect_material_expressions(
        base_color, 0,  # Base Color
        mat.get_material_expression_output(), 0
    )
    unreal.MaterialEditingLibrary.connect_material_expressions(
        roughness, 0,  # Roughness
        mat.get_material_expression_output(), 1
    )
    unreal.MaterialEditingLibrary.connect_material_expressions(
        metallic, 0,  # Metallic
        mat.get_material_expression_output(), 2
    )
   