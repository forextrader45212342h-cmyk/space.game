# Content/Python/generate_city.py
# ------------------------------------------------------------------
# Unreal Engine 5 Python script – HAB‑12 City Auto‑Generator
# Creates a procedural city with buildings, roads, a monorail spline,
# and 60 NPC traffic.  Uses Unreal Python API (AssetTools, MaterialEditingLibrary, etc.)
# ------------------------------------------------------------------

import unreal
import random
import math

# ------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------
def log(msg, level=unreal.LogLevel.INFO):
    """Convenience wrapper for Unreal logging."""
    if level == unreal.LogLevel.INFO:
        unreal.log(msg)
    elif level == unreal.LogLevel.WARNING:
        unreal.log_warning(msg)
    elif level == unreal.LogLevel.ERROR:
        unreal.log_error(msg)

def create_material_instance(name, base_material_path, color, roughness=0.5, metallic=0.0):
    """
    Create a material instance with a random color.
    """
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    package_path = f"/Game/GeneratedMaterials/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(package_path):
        log(f"Material instance {name} already exists.", unreal.LogLevel.WARNING)
        return unreal.EditorAssetLibrary.load_asset(package_path)

    base_material = unreal.load_object(None, base_material_path)
    if not base_material:
        log(f"Base material {base_material_path} not found.", unreal.LogLevel.ERROR)
        return None

    mat_inst = asset_tools.create_asset(name, "/Game/GeneratedMaterials", unreal.MaterialInstanceConstant, unreal.MaterialInstanceConstantFactoryNew())
    if not mat_inst:
        log(f"Failed to create material instance {name}.", unreal.LogLevel.ERROR)
        return None

    # Set parameters
    unreal.MaterialEditingLibrary.set_material_instance_parameter_value_vector(mat_inst, "BaseColor", color)
    unreal.MaterialEditingLibrary.set_material_instance_parameter_value_scalar(mat_inst, "Roughness", roughness)
    unreal.MaterialEditingLibrary.set_material_instance_parameter_value_scalar(mat_inst, "Metallic", metallic)

    unreal.EditorAssetLibrary.save_asset(mat_inst.get_path_name())
    log(f"Created material instance: {mat_inst.get_path_name()}")
    return mat_inst

def spawn_static_mesh_actor(name, mesh_path, location, rotation, scale, material=None):
    """
    Spawn a StaticMeshActor with the given parameters.
    """
    mesh = unreal.load_object(None, mesh_path)
    if not mesh:
        log(f"Static mesh {mesh_path} not found.", unreal.LogLevel.ERROR)
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.set_actor_label(name)
    actor.static_mesh_component.set_static_mesh(mesh)
    actor.static_mesh_component.set_world_scale3d(scale)
    if material:
        actor.static_mesh_component.set_material(0, material)
    return actor

def create_road_spline(name, start, end, width, material):
    """
    Create a road using a spline and a repeated mesh along it.
    """
    # Create a SplineActor
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, start, unreal.Rotator(0, 0, 0))
    spline_actor.set_actor_label(name)

    spline = spline_actor.spline_component
    spline.clear_spline_points()
    spline.add_spline_point(start, unreal.SplineCoordinateSpace.WORLD)
    spline.add_spline_point(end, unreal.SplineCoordinateSpace.WORLD)
    spline.update_spline()

    # Create a road mesh (a thin cube) and attach it to the spline
    road_mesh_path = "/Engine/BasicShapes/Cube.Cube"
    road_mesh = unreal.load_object(None, road_mesh_path)
    if not road_mesh:
        log("Road mesh not found.", unreal.LogLevel.ERROR)
        return None

    # Create a StaticMeshComponent that follows the spline
    road_component = unreal.StaticMeshComponent()
    road_component.set_static_mesh(road_mesh)
    road_component.set_material(0, material)
    road_component.set_world_scale3d(unreal.Vector(width, 1.0, 0.1))  # thin along Y
    road_component.attach_to_component(spline, unreal.AttachmentRule.SNAP_TO_TARGET, "Spline")
    road_component.register_component()

    return spline_actor

def create_monorail_spline(name, points, width, material):
    """
    Create a monorail spline with a rail mesh.
    """
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, points[0], unreal.Rotator(0, 0, 0))
    spline_actor.set_actor_label(name)
    spline = spline_actor.spline_component
    spline.clear_spline_points()
    for pt in points:
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD)
    spline.update_spline()

    # Rail mesh
    rail_mesh_path = "/Engine/BasicShapes/Cylinder.Cylinder"
    rail_mesh = unreal.load_object(None, rail_mesh_path)
    if not rail_mesh:
        log("Rail mesh not found.", unreal.LogLevel.ERROR)
        return None

    rail_component = unreal.StaticMeshComponent()
    rail_component.set_static_mesh(rail_mesh)
    rail_component.set_material(0, material)
    rail_component.set_world_scale3d(unreal.Vector(width, 1.0, 0.1))
    rail_component.attach_to_component(spline, unreal.AttachmentRule.SNAP_TO_TARGET, "Spline")
    rail_component.register_component()

    return spline_actor

def spawn_npc(name, location, character