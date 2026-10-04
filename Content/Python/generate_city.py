# Content/Python/generate_city.py
# ------------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a realistic city
# with buildings, roads, a monorail spline, 60 NPCs and traffic.
# ------------------------------------------------------------------
# Requires: Unreal Engine 5.1+ with Python support enabled.
# ------------------------------------------------------------------

import unreal
import random
import math
from pathlib import Path

# ------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------
CITY_GRID_SIZE = 10          # 10x10 grid
CELL_SIZE = 2000.0           # Distance between building centers (in cm)
ROAD_WIDTH = 400.0           # Road width (in cm)
ROAD_LENGTH = CELL_SIZE      # Road length between buildings
BUILDING_MIN_HEIGHT = 200.0  # Minimum building height (cm)
BUILDING_MAX_HEIGHT = 1200.0 # Maximum building height (cm)
NUM_NPCS = 60
NUM_VEHICLES = 60
MONORAIL_POINTS = 5          # Number of points on the monorail spline
ASSET_ROOT = "/Game/CityAssets"

# ------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------
def log(msg):
    unreal.log("[CityGen] " + msg)

def get_asset_tools():
    return unreal.AssetToolsHelpers.get_asset_tools()

def create_folder(folder_path):
    if not unreal.EditorAssetLibrary.does_directory_exist(folder_path):
        unreal.EditorAssetLibrary.make_directory(folder_path)
        log(f"Created folder: {folder_path}")

def create_material(name, parent_path=None, base_color=None, roughness=0.5, metallic=0.0):
    """
    Create a new material asset with optional parent and parameters.
    """
    asset_path = f"{ASSET_ROOT}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        log(f"Material {name} already exists.")
        return unreal.EditorAssetLibrary.load_asset(asset_path)

    factory = unreal.MaterialFactoryNew()
    if parent_path:
        parent = unreal.EditorAssetLibrary.load_asset(parent_path)
        if parent:
            factory.set_parent(parent)

    material = get_asset_tools().create_asset(name, ASSET_ROOT, unreal.Material, factory)
    if not material:
        log(f"Failed to create material {name}.")
        return None

    # Set parameters
    if base_color:
        unreal.MaterialEditingLibrary.set_material_input_value_by_name(
            material, "BaseColor", unreal.LinearColor(base_color[0], base_color[1], base_color[2], 1.0)
        )
    unreal.MaterialEditingLibrary.set_material_input_value_by_name(
        material, "Roughness", roughness
    )
    unreal.MaterialEditingLibrary.set_material_input_value_by_name(
        material, "Metallic", metallic
    )

    unreal.EditorAssetLibrary.save_asset(asset_path)
    log(f"Created material: {asset_path}")
    return material

def spawn_actor_from_class(actor_class, location, rotation=unreal.Rotator(0,0,0), scale=unreal.Vector(1,1,1)):
    """
    Spawn an actor of the given class at the specified location.
    """
    world = unreal.EditorLevelLibrary.get_editor_world()
    spawn_params = unreal.ActorSpawnParameters()
    actor = world.spawn_actor(actor_class, location, rotation, spawn_params)
    if actor:
        actor.set_actor_scale3d(scale)
    return actor

def set_actor_material(actor, material):
    """
    Set the first static mesh component's material of the actor.
    """
    if not actor:
        return
    mesh_comp = actor.get_component_by_class(unreal.StaticMeshComponent)
    if mesh_comp:
        mesh_comp.set_material(0, material)

def create_building(location, height, material):
    """
    Create a building at the given location with specified height and material.
    """
    cube_path = "/Engine/BasicShapes/Cube"
    cube = unreal.EditorAssetLibrary.load_asset(cube_path)
    if not cube:
        log("Cube asset not found.")
        return None

    actor = spawn_actor_from_class(unreal.StaticMeshActor, location)
    if not actor:
        return None

    actor.static_mesh_component.set_static_mesh(cube)
    actor.set_actor_scale3d(unreal.Vector(1,1,height/100.0))  # Cube default size is 100cm
    set_actor_material(actor, material)
    return actor

def create_road(location, length, width, material):
    """
    Create a road segment at the given location.
    """
    plane_path = "/Engine/BasicShapes/Plane"
    plane = unreal.EditorAssetLibrary.load_asset(plane_path)
    if not plane:
        log("Plane asset not found.")
        return None

    actor = spawn_actor_from_class(unreal.StaticMeshActor, location)
    if not actor:
        return None

    actor.static_mesh_component.set_static_mesh(plane)
    actor.set_actor_scale3d(unreal.Vector(length/100.0, width/100.0, 1))
    set_actor_material(actor, material)
    return actor

def create_monorail_spline(points, rail_mesh_path="/Engine/BasicShapes/Cylinder"):
    """
    Create a spline actor with rail mesh along the given points.
    """
    spline_actor = spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0,0,0))
    if not spline_actor:
        return None

    spline = spline_actor.spline_component
    for i, pt in enumerate(points):
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD)

    # Attach rail mesh along spline
    rail_mesh = unreal.EditorAssetLibrary.load_asset(rail_mesh_path)
    if not rail_mesh:
        log("Rail mesh not found.")
        return spline_actor

    for i in range(len(points)-1):
        start = points[i]
        end = points[i+1]
        segment = unreal