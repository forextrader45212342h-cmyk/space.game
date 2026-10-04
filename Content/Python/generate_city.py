# Content/Python/generate_city.py
# Run this script inside the Unreal Editor (Python console or File > Scripts > Run Script)

import unreal
import random
import math

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material(name, color):
    """Create a simple unlit material with a given base color."""
    mat_factory = unreal.MaterialFactoryNew()
    mat_asset = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name, "/Game/GeneratedMaterials", unreal.Material, mat_factory
    )
    if not mat_asset:
        unreal.log_error(f"Failed to create material {name}")
        return None

    # Set up the material graph
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.EMISSION_COLOR, color)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.ALPHA, 1.0)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.SHADING_MODEL, unreal.ShadingModel.EMISSION)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.CULL_MODE, unreal.CullMode.CULL_NONE)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.TRANSMISSION, 0.0)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.DEPTH_TEST, True)
    unreal.MaterialEditingLibrary.set_material_property(mat_asset, unreal.MaterialProperty.DOUBLE_SIDED, True)

    # Compile the material
    unreal.MaterialEditingLibrary.compile_material(mat_asset)
    return mat_asset

def spawn_actor(actor_class, location, rotation=unreal.Rotator(0,0,0), scale=unreal.Vector(1,1,1)):
    """Spawn an actor of the given class at the specified transform."""
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(actor_class, location, rotation)
    if actor:
        actor.set_actor_scale3d(scale)
    return actor

def add_spline_point(spline, location, tangent=None):
    """Add a point to a spline component."""
    if tangent is None:
        tangent = unreal.Vector(0,0,0)
    spline.add_spline_point(location, tangent, unreal.SplineCoordinateSpace.WORLD, True)

# ------------------------------------------------------------------
# Load base assets
# ------------------------------------------------------------------
# Base building mesh (e.g., a simple cube)
BASE_BUILDING = unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube")
# Base road mesh (e.g., a flat plane)
BASE_ROAD = unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Architecture/Arch_Concrete_Pavement_01.Arch_Concrete_Pavement_01")
# Monorail rail mesh
BASE_RAIL = unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Architecture/Arch_Concrete_Pavement_01.Arch_Concrete_Pavement_01")
# NPC blueprint (use a simple character)
NPC_BP = unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Characters/Character_BP_Mannequin.Character_BP_Mannequin")

if not all([BASE_BUILDING, BASE_ROAD, BASE_RAIL, NPC_BP]):
    unreal.log_error("One or more base assets could not be loaded. Ensure the paths are correct.")
    raise SystemExit

# ------------------------------------------------------------------
# Create city root actor
# ------------------------------------------------------------------
city_actor = spawn_actor(unreal.Actor, unreal.Vector(0,0,0))
city_actor.set_actor_label("GeneratedCity")

# ------------------------------------------------------------------
# Generate buildings
# ------------------------------------------------------------------
BUILDING_COUNT = 200
CITY_RADIUS = 2000  # units
for i in range(BUILDING_COUNT):
    # Random position within a circle
    angle = random.uniform(0, 2*math.pi)
    radius = random.uniform(0, CITY_RADIUS)
    x = radius * math.cos(angle)
    y = radius * math.sin(angle)
    z = 0

    # Random scale for height
    scale_z = random.uniform(1.0, 5.0)
    scale = unreal.Vector(1,1,scale_z)

    # Random color for material
    color = unreal.LinearColor(random.random(), random.random(), random.random(), 1.0)
    mat_name = f"BuildingMat_{i}"
    mat_asset = create_material(mat_name, color)

    # Spawn building
    building = spawn_actor(unreal.StaticMeshActor, unreal.Vector(x,y,z), scale=scale)
    building.set_actor_label(f"Building_{i}")
    building.get_static_mesh_component().set_static_mesh(BASE_BUILDING)
    if mat_asset:
        building.get_static_mesh_component().set_material(0, mat_asset)

    # Attach to city root
    building.attach_to_actor(city_actor, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)

# ------------------------------------------------------------------
# Generate roads (spline)
# ------------------------------------------------------------------
ROAD_COUNT = 10
road_spline = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0,0,0))
road_spline.set_actor_label("RoadSpline")
road_spline.attach_to_actor(city_actor, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)
road_spline_component = road_spline.get_spline_component()

for r in range(ROAD_COUNT):
    # Random start and end points
    start = unreal.Vector(random.uniform(-CITY_RADIUS, CITY_RADIUS),
                          random.uniform(-CITY_RADIUS, CITY_RADIUS), 0)
    end = unreal.Vector(random.uniform(-CITY_RADIUS, CITY_RADIUS),
                        random.uniform(-CITY_RADIUS, CITY_RADIUS), 0)
    add_spline_point(road_spline_component, start)
    add_spline_point(road_spline_component, end)

# Create road mesh along spline
road_mesh = unreal.StaticMeshActor()
road_mesh.set_actor_label("RoadMesh")
road_mesh.attach_to_actor(city_actor, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)
road_mesh.get_static_mesh_component().set_static_mesh(BASE_ROAD)
road_mesh.get_static_mesh_component().set_material(0, unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust"))

# ------------------------------------------------------------------
# Generate monorail spline
# ------------------------------------------------------------------
MONORAIL_COUNT = 3
for m in range(MONORAIL_COUNT):
    monorail_spline = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0,0,0))
    monorail_spline.set_actor_label(f"MonorailSpline_{m}")
    monorail_spline.attach_to_actor(city_actor, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)
    monorail_component = monorail_spline.get_spline_component()

    # Create a circular path
    for i in range(20):
        theta = (i / 20.0) * 2 * math.pi
        radius = 500 + m * 200
        x = radius * math.cos(theta)
        y = radius * math.sin(theta)
        z = 200 + m * 100  # elevated
        add_spline_point(monorail_component, unreal.Vector(x, y, z))

    # Create rail mesh along spline
    rail_actor = unreal.StaticMeshActor()
    rail_actor.set_actor_label(f"MonorailRail_{m}")
    rail_actor.attach_to_actor(city_actor, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)
    rail_actor.get_static_mesh_component().set_static_mesh(BASE_RAIL)
    rail_actor.get_static_mesh_component().set_material(0, unreal.EditorAssetLibrary.load_asset("/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust"))

# ------------------------------------------------------------------
# Spawn NPC traffic
# ------------------------------------------------------------------
NPC_COUNT = 60
for i in range(NPC_COUNT):
    # Randomly pick a road spline point
    road_point_index = random.randint(0, road_spline_component.get_number_of_spline_points() - 1)
    location = road_spline_component.get_location_at_spline_point(road_point_index, unreal.SplineCoordinateSpace.WORLD)
    rotation = road_spline_component.get_rotation_at_spline_point(road_point_index, unreal.SplineCoordinateSpace.WORLD)

    npc = spawn_actor(NPC_BP, location, rotation)
    npc.set_actor_label(f"NPC_{i}")

    # Attach to road spline for simple path following
    npc.attach_to_actor(road_spline, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD)

# ------------------------------------------------------------------
# Final log
# ------------------------------------------------------------------
unreal.log("City generation complete. Buildings: {}, Roads: {}, Monorails: {}, NPCs: {}"
           .format(BUILDING_COUNT, ROAD_COUNT, MONORAIL_COUNT, NPC_COUNT))
