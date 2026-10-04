# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a city with
# buildings, roads, a monorail spline, and 60 NPC traffic.
# ------------------------------------------------------------
import unreal
import random
import math

# ------------------------------------------------------------------
# Utility helpers
# ------------------------------------------------------------------
def make_directory(path: str):
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)

def create_asset(asset_name: str, package_path: str, asset_class, factory):
    """Create a new asset and return it."""
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    return asset_tools.create_asset(asset_name, package_path, asset_class, factory)

def set_material_parameter(mat, param_name: str, value):
    """Set a scalar or vector parameter on a material."""
    if isinstance(value, unreal.LinearColor):
        unreal.MaterialEditingLibrary.set_material_input_value(mat, param_name, value)
    else:
        unreal.MaterialEditingLibrary.set_material_input_value(mat, param_name, float(value))

# ------------------------------------------------------------------
# Material creation
# ------------------------------------------------------------------
def create_base_material(name: str, base_color: unreal.LinearColor,
                         roughness: float = 0.5, metallic: float = 0.0) -> unreal.Material:
    """Create a simple base material."""
    factory = unreal.MaterialFactoryNew()
    mat = create_asset(name, "/Game/City/Materials", unreal.Material, factory)
    if not mat:
        unreal.log_error(f"Failed to create material {name}")
        return None

    # Set default shading model to Lit
    mat.set_editor_property("shading_model", unreal.MaterialShadingModel.LIT)

    # Set parameters
    set_material_parameter(mat, "BaseColor", base_color)
    set_material_parameter(mat, "Roughness", roughness)
    set_material_parameter(mat, "Metallic", metallic)

    # Compile material
    mat.post_edit_change()
    mat.mark_package_dirty()
    return mat

def create_material_instance(name: str, parent: unreal.Material,
                             overrides: dict = None) -> unreal.MaterialInstanceConstant:
    """Create a material instance with optional parameter overrides."""
    factory = unreal.MaterialInstanceConstantFactoryNew()
    mat_inst = create_asset(name, "/Game/City/Materials", unreal.MaterialInstanceConstant, factory)
    if not mat_inst:
        unreal.log_error(f"Failed to create material instance {name}")
        return None

    mat_inst.set_editor_property("parent", parent)

    if overrides:
        for param, value in overrides.items():
            set_material_parameter(mat_inst, param, value)

    mat_inst.post_edit_change()
    mat_inst.mark_package_dirty()
    return mat_inst

# ------------------------------------------------------------------
# Building creation
# ------------------------------------------------------------------
def create_building(name: str, location: unreal.Vector,
                    height: float, material: unreal.MaterialInterface) -> unreal.StaticMeshActor:
    """Spawn a building as a scaled cube."""
    cube_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cube.Cube")
    if not cube_mesh:
        unreal.log_error("Cube mesh not found.")
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location)
    actor.set_actor_label(name)
    sm_comp = actor.get_static_mesh_component()
    sm_comp.set_static_mesh(cube_mesh)
    # Cube is 100 units, scale height accordingly
    scale = unreal.Vector(1.0, 1.0, height / 100.0)
    sm_comp.set_world_scale3d(scale)
    sm_comp.set_material(0, material)
    sm_comp.set_collision_enabled(unreal.CollisionEnabled.QUERY_AND_PHYSICS)
    sm_comp.set_simulate_physics(False)
    return actor

# ------------------------------------------------------------------
# Road creation
# ------------------------------------------------------------------
def create_road(name: str, start: unreal.Vector, end: unreal.Vector,
                width: float, material: unreal.MaterialInterface) -> unreal.StaticMeshActor:
    """Spawn a road as a scaled plane."""
    plane_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Plane.Plane")
    if not plane_mesh:
        unreal.log_error("Plane mesh not found.")
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, unreal.Vector(0,0,0))
    actor.set_actor_label(name)
    sm_comp = actor.get_static_mesh_component()
