# Content/Python/generate_ultra_materials.py
# ----------------------------------------------------
# This script creates four high‑quality UE5 materials directly in the editor.
# It uses the Unreal Python API (unreal module) and the MaterialEditingLibrary
# to build node graphs, set material properties, and enable Lumen, Nanite,
# and ray‑traced reflections where appropriate.
#
# Run this script from the UE5 Editor's Python console or by double‑clicking
# it in the Content Browser (Content/Python folder).
# ----------------------------------------------------

import unreal

# ------------------------------------------------------------------
# Utility helpers
# ------------------------------------------------------------------
def ensure_folder(folder_path: str):
    """Create the folder if it doesn't exist."""
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        unreal.EditorAssetLibrary.make_directory(folder_path)

def create_material_asset(name: str, folder_path: str) -> unreal.Material:
    """Create a new material asset and return the reference."""
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material_factory = unreal.MaterialFactoryNew()
    asset = asset_tools.create_asset(name, folder_path, unreal.Material, material_factory)
    return asset

def set_material_property(mat: unreal.Material, prop_name: str, value):
    """Set a material property via the MaterialEditingLibrary."""
    unreal.MaterialEditingLibrary.set_material_property(mat, prop_name, value)

def add_texture_sample(mat: unreal.Material, tex_path: str, output_name: str):
    """Add a TextureSample node and return its reference."""
    tex_sample = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionTextureSample, 200, 200)
    tex_sample.texture = unreal.EditorAssetLibrary.load_asset(tex_path)
    tex_sample.output_name = output_name
    return tex_sample

def add_scalar_parameter(mat: unreal.Material, param_name: str, default_value: float):
    """Add a ScalarParameter node."""
    param = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionScalarParameter, 200, 200)
    param.parameter_name = param_name
    param.default_value = default_value
    return param

def add_vector_parameter(mat: unreal.Material, param_name: str, default_value: unreal.LinearColor):
    """Add a VectorParameter node."""
    param = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionVectorParameter, 200, 200)
    param.parameter_name = param_name
    param.default_value = default_value
    return param

def connect_expressions(mat: unreal.Material, src, src_output, dst, dst_input):
    """Connect two material expressions."""
    unreal.MaterialEditingLibrary.connect_material_expressions(mat, src, src_output, dst, dst_input)

# ------------------------------------------------------------------
# Material builders
# ------------------------------------------------------------------
def build_master_lumen_nanite(mat: unreal.Material):
    """
    Master material that uses Lumen, Nanite, and ray‑traced reflections.
    It expects 8K textures for BaseColor, Normal, Roughness, Metallic.
    """
    # Texture paths (replace with actual 8K assets in your project)
    base_color_tex = "/Game/Textures/8K_Master_BaseColor"
    normal_tex     = "/Game/Textures/8K_Master_Normal"
    roughness_tex  = "/Game/Textures/8K_Master_Roughness"
    metallic_tex   = "/Game/Textures/8K_Master_Metallic"

    # Base Color
    base_color = add_texture_sample(mat, base_color_tex, "BaseColor")
    # Normal
    normal = add_texture_sample(mat, normal_tex, "Normal")
    # Roughness
    roughness = add_texture_sample(mat, roughness_tex, "Roughness")
    # Metallic
    metallic = add_texture_sample(mat, metallic_tex, "Metallic")

    # Connect to material inputs
    connect_expressions(mat, base_color, "RGBA", mat, "Base Color")
    connect_expressions(mat, normal, "RGBA", mat, "Normal")
    connect_expressions(mat, roughness, "RGBA", mat, "Roughness")
    connect_expressions(mat, metallic, "RGBA", mat, "Metallic")

    # Enable Lumen, Nanite, Ray‑traced reflections
    set_material_property(mat, unreal.MaterialProperty.MP_LUMEN, True)
    set_material_property(mat, unreal.MaterialProperty.MP_NANITE, True)
    set_material_property(mat, unreal.MaterialProperty.MP_RAY_TRACING, True)

    # Set shading model to Default Lit (PBR)
    set_material_property(mat, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.MaterialShadingModel.MSM_DEFAULT_LIT)

def build_kobayashi_emissive_neon(mat: unreal.Material):
    """
    Emissive neon material for KOBAYASHI CORP branding.
    Uses a bright neon color and a translucent blend mode.
    """
    # Neon color (VectorParameter)
    neon_color = add_vector_parameter(mat, "NeonColor", unreal.LinearColor(0.0, 1.0, 1.0, 1.0))  # Cyan
    # Emissive intensity
    emissive_intensity = add_scalar_parameter(mat, "EmissiveIntensity", 10.0)

    # Multiply NeonColor * EmissiveIntensity
    multiply = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionMultiply, 200, 200)
    connect_expressions(mat, neon_color, "RGBA", multiply, "A")
    connect_expressions(mat, emissive_intensity, "Scalar", multiply, "B")

    # Connect to Emissive Color
    connect_expressions(mat, multiply, "RGBA", mat, "Emissive Color")

    # Set blend mode to Translucent for neon glow
    set_material_property(mat, unreal.MaterialProperty.MP_BLEND_MODE, unreal.BlendMode.BLEND_TRANSLUCENT)

    # Enable Lumen for glow
    set_material_property(mat, unreal.MaterialProperty.MP_LUMEN, True)

    # Set shading model to Default Lit
    set_material_property(mat, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.MaterialShadingModel.MSM_DEFAULT_LIT)

def build_wet_street_pbr(mat: unreal.Material):
    """
    Wet street PBR material with puddle effect.
    Uses a Fresnel node to blend a puddle texture on top of the base.
    """
    # Base textures
    base_color_tex = "/Game/Textures/8K_WetStreet_BaseColor"
    normal_tex     = "/Game/Textures/8K_WetStreet_Normal"
    roughness_tex  = "/Game/Textures/8K_WetStreet_Roughness"
    metallic_tex   = "/Game/Textures/8K_WetStreet_Metallic"

    # Puddle texture (grayscale for transparency)
    puddle_tex = "/Game/Textures/8K_WetStreet_Puddle"

    # Base Color
    base_color = add_texture_sample(mat, base_color_tex, "BaseColor")
    # Normal
    normal = add_texture_sample(mat, normal_tex, "Normal")
    # Roughness
    roughness = add_texture_sample(mat, roughness_tex, "Roughness")
    # Metallic
    metallic = add_texture_sample(mat, metallic_tex, "Metallic")

    # Puddle (grayscale) as alpha
    puddle = add_texture_sample(mat, puddle_tex, "Puddle")

    # Fresnel for puddle blending
    fresnel = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionFresnel, 200, 200)
    fresnel.fresnel_exponent = 1.0
    fresnel.fresnel_bias = 0.0
    fresnel.fresnel_scale = 1.0

    # Multiply Fresnel with Puddle to get alpha
    puddle_alpha = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionMultiply, 200, 200)
    connect_expressions(mat, fresnel, "Fresnel", puddle_alpha, "A")
    connect_expressions(mat, puddle, "RGBA", puddle_alpha, "B")

    # Blend BaseColor with Puddle using the alpha
    blend = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionLinearInterpolate, 200, 200)
    connect_expressions(mat, puddle_alpha, "Scalar", blend, "Alpha")
    connect_expressions(mat, puddle, "RGBA", blend, "B")  # Puddle color (could be black)
    connect_expressions(mat, base_color, "RGBA", blend, "A")

    # Connect to material inputs
    connect_expressions(mat, blend, "RGBA", mat, "Base Color")
    connect_expressions(mat, normal, "RGBA", mat, "Normal")
    connect_expressions(mat, roughness, "RGBA", mat, "Roughness")
    connect_expressions(mat, metallic, "RGBA", mat, "Metallic")

    # Set shading model to Default Lit
    set_material_property(mat, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.MaterialShadingModel.MSM_DEFAULT_LIT)

    # Enable Lumen and Nanite for high‑detail surfaces
    set_material_property(mat, unreal.MaterialProperty.MP_LUMEN, True)
    set_material_property(mat, unreal.MaterialProperty.MP_NANITE, True)

def build_nectar_cafe_sunny(mat: unreal.Material):
    """
    Sunny material for NECTAR CAFE interior.
    Uses a warm base color, subtle specular highlights, and a light
    directional light simulation via a simple Fresnel.
    """
    # Base textures
    base_color_tex = "/Game/Textures/8K_NectarCafe_BaseColor"
    normal_tex     = "/Game/Textures/8K_NectarCafe_Normal"
    roughness_tex  = "/Game/Textures/8K_NectarCafe_Roughness"
    metallic_tex   = "/Game/Textures/8K_NectarCafe_Metallic"

    # Base Color
    base_color = add_texture_sample(mat, base_color_tex, "BaseColor")
    # Normal
    normal = add_texture_sample(mat, normal_tex, "Normal")
    # Roughness
    roughness = add_texture_sample(mat, roughness_tex, "Roughness")
    # Metallic
    metallic = add_texture_sample(mat, metallic_tex, "Metallic")

    # Warm ambient light (VectorParameter)
    ambient_light = add_vector_parameter(mat, "AmbientLight", unreal.LinearColor(1.0, 0.9, 0.8, 1.0))

    # Multiply BaseColor by AmbientLight
    ambient = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionMultiply, 200, 200)
    connect_expressions(mat, base_color, "RGBA", ambient, "A")
    connect_expressions(mat, ambient_light, "RGBA", ambient, "B")

    # Connect to material inputs
    connect_expressions(mat, ambient, "RGBA", mat, "Base Color")
    connect_expressions(mat, normal, "RGBA", mat, "Normal")
    connect_expressions(mat, roughness, "RGBA", mat, "Roughness")
    connect_expressions(mat, metallic, "RGBA", mat, "Metallic")

    # Set shading model to Default Lit
    set_material_property(mat, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.MaterialShadingModel.MSM_DEFAULT_LIT)

    # Enable Lumen for realistic lighting
    set_material_property(mat, unreal.MaterialProperty.MP_LUMEN, True)

# ------------------------------------------------------------------
# Main execution
# ------------------------------------------------------------------
def main():
    # Folder where the materials will be created
    folder_path = "/Game/GeneratedMaterials"
    ensure_folder(folder_path)

    # Master Lumen Nanite material
    master_mat = create_material_asset("Master_Lumen_Nanite", folder_path)
    build_master_lumen_nanite(master_mat)

    # KOBAYASHI Corp emissive neon material
    neon_mat = create_material_asset("KOBAYASHI_Corp_Emissive_Neon", folder_path)
    build_kobayashi_emissive_neon(neon_mat)

    # Wet street PBR with puddles
    wet_mat = create_material_asset("Wet_Street_PBR_Puddles", folder_path)
    build_wet_street_pbr(wet_mat)

    # NECTAR CAFE sunny material
    cafe_mat = create_material_asset("NECTAR_Cafe_Sunny", folder_path)
    build_nectar_cafe_sunny(cafe_mat)

    # Mark assets as dirty so the editor refreshes
    unreal.EditorAssetLibrary.save_loaded_asset(master_mat)
    unreal.EditorAssetLibrary.save_loaded_asset(neon_mat)
    unreal.EditorAssetLibrary.save_loaded_asset(wet_mat)
    unreal.EditorAssetLibrary.save_loaded_asset(cafe_mat)

    unreal.log("✅ Ultra‑realistic materials generated in /Game/GeneratedMaterials")

if __name__ == "__main__":
    main()
