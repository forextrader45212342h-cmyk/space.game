# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script to procedurally generate a city
# with buildings, roads, a monorail, and NPC traffic.
# ------------------------------------------------------------
import unreal
import random
import math

# ------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------
CITY_SIZE = 2000          # Size of the city grid (units)
BUILDING_COUNT = 50       # Number of buildings
ROAD_COUNT = 10           # Number of roads
MONORAIL_COUNT = 1        # Number of monorail lines
NPC_COUNT = 60            # Number of NPCs

# Asset paths
CUBE_MESH_PATH = "/Engine/BasicShapes/Cube.Cube"
PLANE_MESH_PATH = "/Engine/BasicShapes/Plane.Plane"
CYLINDER_MESH_PATH = "/Engine/BasicShapes/Cylinder.Cylinder"

# ------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------
def log(msg):
    unreal.log("[GenerateCity] " + msg)

def create_folder(folder_path):
    """Create a folder in the content browser if it doesn't exist."""
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        unreal.EditorAssetLibrary.make_directory(folder_path)
        log(f"Created folder: {folder_path}")

def create_material(name, base_color, roughness=0.5, metallic=0.2):
    """Create a simple material with a base color, roughness, and metallic."""
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material_path = f"/Game/GeneratedCity/Materials/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(material_path):
        mat = unreal.EditorAssetLibrary.load_asset(material_path)
        log(f"Material already exists: {material_path}")
        return mat

    mat = asset_tools.create_asset(name, "/Game/GeneratedCity/Materials", unreal.Material, unreal.MaterialFactoryNew())
    if not mat:
        log(f"Failed to create material: {name}")
        return None

    # Base Color
    base_color_expr = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant3Vector, 0, 0)
    base_color_expr.constant = base_color

    # Roughness
    roughness_expr = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant, 0, 0)
    roughness_expr.constant = roughness

    # Metallic
    metallic_expr = unreal.MaterialEditingLibrary.create_material_expression(mat, unreal.MaterialExpressionConstant, 0, 0)
    metallic_expr.constant = metallic

    # Connect expressions
    unreal.MaterialEditingLibrary.connect_material_property(base_color_expr, unreal.MaterialProperty.MP_BASE_COLOR, mat, unreal.MaterialProperty.MP_BASE_COLOR)
    unreal.MaterialEditingLibrary.connect_material_property(roughness_expr, unreal.MaterialProperty.MP_ROUGHNESS, mat, unreal.MaterialProperty.MP_ROUGHNESS)
    unreal.MaterialEditingLibrary.connect_material_property(metallic_expr, unreal.MaterialProperty.MP_METALLIC, mat, unreal.MaterialProperty.MP_METALLIC)

    # Compile material
    unreal.MaterialEditingLibrary.set_material_property(mat, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.ShadingModel.SMOOTH_SURFACE)
    mat.post_edit_change()
    mat.mark_package_dirty()
    unreal.EditorAssetLibrary.save_asset(material_path)
    log(f"Created material: {material_path}")
    return mat

def spawn_static_mesh_actor(name, mesh_path, location, rotation, scale, material=None):
    """Spawn a StaticMeshActor with optional material."""
    static_mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not static_mesh:
        log(f"Failed to load mesh: {mesh_path}")
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.set_actor_label(name)
    actor.static_mesh_component.set_static_mesh