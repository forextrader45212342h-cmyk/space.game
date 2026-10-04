# Content/Python/generate_city.py
# Unreal Engine 5 Python script to auto‑generate a city with buildings, roads, monorail, and NPC traffic.
# Run this script inside the Unreal Editor (Python console or via the Content Browser).

import unreal
import random
import math

# ------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------
CITY_GRID_SIZE = 10          # 10x10 grid
BLOCK_SIZE = 100.0           # Distance between grid intersections
ROAD_WIDTH = 20.0
MONORAIL_WIDTH = 5.0
MONORAIL_HEIGHT = 50.0
NUM_NPCS = 60
BUILDING_MIN_HEIGHT = 10.0
BUILDING_MAX_HEIGHT = 50.0

# Asset paths
ASSET_ROOT = "/Game/GeneratedCity"
MATERIAL_ROOT = f"{ASSET_ROOT}/Materials"
MESH_ROOT = f"{ASSET_ROOT}/Meshes"
NPC_ROOT = f"{ASSET_ROOT}/NPCs"

# Base assets (must exist in the project)
BASE_CUBE = "/Engine/BasicShapes/Cube"
BASE_PLANE = "/Engine/BasicShapes/Plane"
BASE_CHARACTER_BP = "/Game/StarterContent/Blueprints/BasicCharacter"  # Replace with your character BP

# ------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------
def ensure_directory(path):
    if not unreal.EditorAssetLibrary.does_directory_exist(path):
        unreal.EditorAssetLibrary.make_directory(path)

def create_material(name, base_material_path, color, roughness):
    """Create a material instance with specified base color and roughness."""
    asset_path = f"{MATERIAL_ROOT}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        return unreal.EditorAssetLibrary.load_asset(asset_path)

    base_mat = unreal.EditorAssetLibrary.load_asset(base_material_path)
    if not base_mat:
        unreal.log_error(f"Base material not found: {base_material_path}")
        return None

    mat_instance = unreal.MaterialEditingLibrary.create_material_instance(asset_path, base_mat)
    unreal.MaterialEditingLibrary.set_material_property(mat_instance, unreal.MaterialProperty.MP_BASE_COLOR, color)
    unreal.MaterialEditingLibrary.set_material_property(mat_instance, unreal.MaterialProperty.MP_ROUGHNESS, roughness)
    unreal.EditorAssetLibrary.save_asset(asset_path)
    return mat_instance

def create_static_mesh_asset(name, source_mesh_path):
    """Create a static mesh asset from an existing source mesh."""
    asset_path = f"{MESH_ROOT}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        return unreal.EditorAssetLibrary.load_asset(asset_path)

    source_mesh = unreal.EditorAssetLibrary.load_asset(source_mesh_path)
    if not source_mesh:
        unreal.log_error(f"Source mesh not found: {source_mesh_path}")
        return None

    # Duplicate the mesh asset
    new_mesh = unreal.EditorAssetLibrary.duplicate_asset(source_mesh_path, asset_path)
    unreal.EditorAssetLibrary.save_asset(asset_path)
    return new_mesh

def spawn_static_mesh_actor(location, mesh, material, scale=(1,1,1)):
    """Spawn a static mesh actor at the given location."""
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location)
    actor.static_mesh_component.set_static_mesh(mesh)
    actor.static_mesh_component.set_material(0, material)
    actor.set_actor_scale3d(scale)
    return actor

def create_spline_actor(name, points, material, mesh, segment_scale=(1,1,1)):
    """Create a spline actor with spline mesh components along the given points."""
    asset_path = f"{ASSET_ROOT}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(asset_path):
        return unreal.EditorAssetLibrary.load_asset(asset_path)

    # Create a new spline actor
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0,0,0))
    spline_actor.set_actor_label(name)
    spline = spline_actor.get_spline_component()

    # Add spline points
    for pt in points:
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD, unreal.SplinePointType.CATMULL_ROM)

    # Create spline mesh components for each segment
    for i in range(len(points)-1):
        start = points[i]
        end = points[i+1]
        spline_mesh = unreal.SplineMeshComponent()
        spline_mesh.set_static_mesh(mesh)
        spline_mesh.set_material(0, material)
        spline_mesh.set_start_and_end(start, unreal.Vector(0,0,0), end, unreal.Vector(0,0,0))
        spline_mesh.set_start_scale(segment_scale)
        spline_mesh.set_end_scale(segment_scale)
        spline_mesh.attach_to_component(spline, None)
        spline_mesh.register_component()

    unreal.EditorAssetLibrary.save_asset(asset_path)
    return spline_actor

def spawn_npc(location, character_class):
    """Spawn an NPC character at the specified location."""
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(character_class, location)
    return actor

# ------------------------------------------------------------------
# Main generation logic
# ------------------------------------------------------------------
def main():
    # Create directories
    ensure_directory(ASSET_ROOT)
    ensure_directory(MATERIAL_ROOT)
    ensure_directory(MESH_ROOT)
    ensure_directory(NPC_ROOT)

    # Create materials
    building_mat = create_material(
        "Building_Mat",
        "/Engine/BasicMaterials/BasicMaterial",
        unreal.LinearColor(random.random(), random.random(), random.random(), 1.0),
        random.uniform(0.3, 0.9)
    )
    road_mat = create_material(
        "Road_Mat",
        "/Engine/BasicMaterials/BasicMaterial",
        unreal.LinearColor(0.2, 0.2, 0.2, 1.0),
        0.8
    )
    rail_mat = create_material(
        "Rail_Mat",
        "/Engine/BasicMaterials/BasicMaterial",
        unreal.LinearColor(0.5, 0.5, 0.5, 1.0),
        0.5
    )

    # Create meshes
    building_mesh = create_static_mesh_asset("Building_Mesh", BASE_CUBE)
    road_mesh = create_static_mesh_asset("Road_Mesh", BASE_PLANE)
    rail_mesh = create_static_mesh_asset("Rail_Mesh", BASE_CUBE)

    # Generate buildings
    for i in range(CITY_GRID_SIZE):
        for j in range(CITY_GRID_SIZE):
            # Skip intersections where roads will be
            if i % 2 == 0 or j % 2 == 0:
                continue
            x = i * BLOCK_SIZE
            y = j * BLOCK_SIZE
            z = 0
            height = random.uniform(BUILDING_MIN_HEIGHT, BUILDING_MAX_HEIGHT)
            scale = (1.0, 1.0, height / 100.0)  # Cube is 100 units tall by default
            spawn_static_mesh_actor(
                unreal.Vector(x, y, z + height / 2