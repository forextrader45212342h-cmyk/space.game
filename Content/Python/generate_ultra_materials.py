# Content/Python/generate_ultra_materials.py
# Run this script inside the Unreal Editor to automatically generate four ultra‑realistic materials.
# Requires Unreal Engine 5.0+ with the Python Editor Script Plugin enabled.

import unreal

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material(name: str, package_path: str, factory=None) -> unreal.Material:
    """
    Create a new material asset and return the Material object.
    """
    if factory is None:
        factory = unreal.MaterialFactoryNew()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material = asset_tools.create_asset(name, package_path, unreal.Material, factory)
    return material


def add_texture_sample(material: unreal.Material, texture_path: str, node_name: str = None):
    """
    Add a TextureSample node to the material.
    """
    tex = unreal.EditorAssetLibrary.load_asset(texture_path)
    if tex is None:
        unreal.log_warning(f"Texture {texture_path} not found. Skipping node.")
        return None
    node = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionTextureSample, 200, 200)
    node.texture = tex
    if node_name:
        node.set_editor_property("material_expression_name", node_name)
    return node


def add_constant3_vector(material: unreal.Material, color: unreal.LinearColor, node_name: str = None):
    """
    Add a Constant3Vector node to the material.
    """
    node = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionConstant3Vector, 200, 200)
    node.constant = color
    if node_name:
        node.set_editor_property("material_expression_name", node_name)
    return node


def add_constant(material: unreal.Material, value: float, node_name: str = None):
    """
    Add a Constant node to the material.
    """
    node = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionConstant, 200, 200)
    node.constant = value
    if node_name:
        node.set_editor_property("material_expression_name", node_name)
    return node


def add_multiply(material: unreal.Material, node_name: str = None):
    """
    Add a Multiply node to the material.
    """
    node = unreal.MaterialEditingLibrary.create_material_expression(material, unreal.MaterialExpressionMultiply, 200, 200)
    if node_name:
        node.set_editor_property("material_expression_name", node_name)
    return node


def connect_nodes(material: unreal.Material, src_node, src_output, dst_node, dst_input):
    """
    Connect the output of src_node to the input of dst_node.
    """
    unreal.MaterialEditingLibrary.connect_material_property(src_node, src_output, dst_node, dst_input)


# ------------------------------------------------------------------
# Material creation functions
# ------------------------------------------------------------------
def create_master_material_lumen_nanite():
    """
    Creates a master material that uses Lumen and Nanite ray‑traced reflections.
    """
    mat = create_material("Master_Lumen_Nanite", "/Game/Materials")
    # Enable Lumen and Nanite
    mat.set_editor_property("bUseLumen", True)
    mat.set_editor_property("bUseNanite", True)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MS_DEFAULT_LIT)
    mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_Opaque)

    # Roughness and Metallic defaults
    roughness_node = add_constant(mat, 0.2, "Roughness")
    metallic_node = add_constant(mat, 0.0, "Metallic")

    # Connect to material output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_nodes(mat, roughness_node, "R", output, "Roughness")
    connect_nodes(mat, metallic_node, "R", output, "Metallic")

    # Save changes
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    unreal.log(f"Created Master Material: {mat.get_name()}")
    return mat


def create_kobayashi_corp_emissive_neon():
    """
    Creates an emissive neon material for KOBAYASHI CORP branding.
    """
    mat = create_material("KobayashiCorp_EmissiveNeon", "/Game/Materials")
    mat.set_editor_property("bUseLumen", True)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MS_DEFAULT_LIT)
    mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_Opaque)

    # Emissive color (bright neon blue)
    emissive_color = unreal.LinearColor(0.0, 0.5, 1.0, 1.0)  # RGBA
    emissive_node = add_constant3_vector(mat, emissive_color, "EmissiveColor")

    # Optional: add a texture sample for pattern
    # pattern_tex_path = "/Game/Textures/NeonPattern"
    # pattern_node = add_texture_sample(mat, pattern_tex_path, "NeonPattern")

    # Multiply emissive by intensity
    intensity_node = add_constant(mat, 5.0, "Intensity")
    multiply_node = add_multiply(mat, "EmissiveMultiply")
    connect_nodes(mat, emissive_node, "RGB", multiply_node, "A")
    connect_nodes(mat, intensity_node, "R", multiply_node, "B")

    # Connect to material output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_nodes(mat, multiply_node, "RGB", output, "EmissiveColor")

    # Save changes
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    unreal.log(f"Created Emissive Neon Material: {mat.get_name()}")
    return mat


def create_wet_street_pbr_with_puddles():
    """
    Creates a wet street PBR material with puddle reflections.
    """
    mat = create_material("WetStreet_PBR_Puddles", "/Game/Materials")
    mat.set_editor_property("bUseLumen", True)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MS_DEFAULT_LIT)
    mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_Opaque)

    # Base color (dark gray)
    base_color = unreal.LinearColor(0.1, 0.1, 0.1, 1.0)
    base_node = add_constant3_vector(mat, base_color, "BaseColor")

    # Roughness (low for wet surface)
    roughness_node = add_constant(mat, 0.05, "Roughness")

    # Metallic (0 for non-metallic)
    metallic_node = add_constant(mat, 0.0, "Metallic")

    # Normal map (use default normal)
    normal_node = add_texture_sample(mat, "/Engine/EngineResources/DefaultNormal", "NormalMap")

    # Puddle reflection: use a simple Fresnel node
    fresnel_node = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionFresnel, 200, 200)
    fresnel_node.set_editor_property("Exponent", 5.0)
    fresnel_node.set_editor_property("Bias", 0.0)
    fresnel_node.set_editor_property("Scale", 1.0)

    # Connect nodes to output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_nodes(mat, base_node, "RGB", output, "BaseColor")
    connect_nodes(mat, roughness_node, "R", output, "Roughness")
    connect_nodes(mat, metallic_node, "R", output, "Metallic")
    connect_nodes(mat, normal_node, "RGB", output, "Normal")
    connect_nodes(mat, fresnel_node, "Fresnel", output, "EmissiveColor")  # simple reflection

    # Save changes
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    unreal.log(f"Created Wet Street PBR Material: {mat.get_name()}")
    return mat


def create_nectar_cafe_sunny_material():
    """
    Creates a sunny material for NECTAR CAFE interior/exterior.
    """
    mat = create_material("NectarCafe_Sunny", "/Game/Materials")
    mat.set_editor_property("bUseLumen", True)
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.MS_DEFAULT_LIT)
    mat.set_editor_property("blend_mode", unreal.BlendMode.BLEND_Opaque)

    # Base color (warm beige)
    base_color = unreal.LinearColor(0.9, 0.8, 0.6, 1.0)
    base_node = add_constant3_vector(mat, base_color, "BaseColor")

    # Roughness (medium)
    roughness_node = add_constant(mat, 0.4, "Roughness")

    # Metallic (low)
    metallic_node = add_constant(mat, 0.1, "Metallic")

    # Normal map (optional)
    normal_node = add_texture_sample(mat, "/Engine/EngineResources/DefaultNormal", "NormalMap")

    # Emissive for subtle glow (e.g., cafe lights)
    emissive_color = unreal.LinearColor(0.05, 0.04, 0.02, 1.0)
    emissive_node = add_constant3_vector(mat, emissive_color, "EmissiveColor")

    # Connect nodes to output
    output = unreal.MaterialEditingLibrary.get_material_output(mat)
    connect_nodes(mat, base_node, "RGB", output, "BaseColor")
    connect_nodes(mat, roughness_node, "R", output, "Roughness")
    connect_nodes(mat, metallic_node, "R", output, "Metallic")
    connect_nodes(mat, normal_node, "RGB", output, "Normal")
    connect_nodes(mat, emissive_node, "RGB", output, "EmissiveColor")

    # Save changes
    unreal.EditorAssetLibrary.save_loaded_asset(mat)
    unreal.log(f"Created Nectar Cafe Sunny Material: {mat.get_name()}")
    return mat


# ------------------------------------------------------------------
# Main execution
# ------------------------------------------------------------------
def main():
    unreal.log("=== Generating Ultra‑Realistic Materials ===")
    create_master_material_lumen_nanite()
    create_kobayashi_corp_emissive_neon()
    create_wet_street_pbr_with_puddles()
    create_nectar_cafe_sunny_material()
    unreal.log("=== Material generation complete ===")


if __name__ == "__main__":
    main()
