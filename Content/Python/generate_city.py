# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a city layout
# with buildings, roads, a monorail spline, and 60 NPC traffic.
# ------------------------------------------------------------
import unreal
import random
import math

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------
def create_material_instance(name: str, base_material_path: str, color: unreal.LinearColor) -> str:
    """
    Creates a Material Instance Constant from a base material and sets its BaseColor.
    Returns the full asset path of the created instance.
    """
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.MaterialInstanceConstantFactoryNew()
    package_name = f"/Game/GeneratedMaterials/{name}"
    asset = asset_tools.create_asset(name, "/Game/GeneratedMaterials", unreal.MaterialInstanceConstant, factory)
    if not asset:
        unreal.log_error(f"Failed to create material instance {name}")
        return ""

    # Set the parent material
    asset.set_parent_editor_only(unreal.load_object(None, base_material_path))
    # Apply color
    unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
        asset, "BaseColor", color
    )
    # Save the asset
    unreal.EditorAssetLibrary.save_asset(package_name)
    return package_name


def spawn_actor_from_class(actor_class, location, rotation=unreal.Rotator(0, 0, 0), scale=unreal.Vector(1, 1, 1), material=None):
    """
    Spawns an actor of the given class at the specified location.
    Optionally applies a material to the first mesh component.
    """
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(actor_class, location, rotation)
    if actor and material:
        mesh_comp = actor.get_component_by_class(unreal.StaticMeshComponent)
        if mesh_comp:
            mesh_comp.set_material(0, unreal.load_object(None, material))
    if actor:
        actor.set_actor_scale3d(scale)
    return actor


def create_building(location, size, material_path):
    """
    Creates a building by scaling a cube mesh.
    """
    cube_path = "/Engine/BasicShapes/Cube.Cube"
    cube = unreal.load_object(None, cube_path)
    if not cube:
        unreal.log_error("Cube mesh not found.")
        return
    actor = spawn_actor_from_class(unreal.StaticMeshActor, location, scale=size, material=material_path)
    actor.set_actor_label(f"Building_{location.x}_{location.y}")
    return actor


def create_road(location, length, width, material_path):
    """
    Creates a road segment using a scaled cube (or plane).
    """
    cube_path = "/Engine/BasicShapes/Cube.Cube"
    cube = unreal.load_object(None, cube_path)
    if not cube:
        unreal.log_error("Cube mesh not found.")
        return
    scale = unreal.Vector(length / 100.0, width / 100.0, 0.1)  # Thin slab
    actor = spawn_actor_from_class(unreal.StaticMeshActor, location, scale=scale, material=material_path)
    actor.set_actor_label(f"Road_{location.x}_{location.y}")
    return actor


def create_monorail_spline(points, rail_mesh_path, material_path):
    """
    Creates a spline actor and attaches a rail mesh along it.
    """
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0, 0, 0))
    spline_actor.set_actor_label("MonorailSpline")
    spline = spline_actor.get_spline_component()

    # Add points
    for i, pt in enumerate(points):
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD, True)

    # Create a rail mesh component and attach to spline
    rail_mesh = unreal.StaticMeshComponent()
    rail_mesh.register_component()
    rail_mesh.set_static_mesh(unreal.load_object(None, rail_mesh_path))
    rail_mesh.set_material(0, unreal.load_object(None, material_path))
    rail_mesh.attach_to_component(spline, unreal.AttachmentTransformRules(keep_world_transform=False))
    rail_mesh.set_relative_location(unreal.Vector(0, 0, 0))
    rail_mesh.set_relative_rotation(unreal.Rotator(0, 0, 0))
    rail_mesh.set_relative_scale3d(unreal.Vector(1, 1, 1))

    # Optionally, add a spline mesh component for each segment
    for i in range(len(points) - 1):
        segment = unreal.SplineMeshComponent()
        segment.register_component()
        segment.set_static_mesh(unreal.load_object(None, rail_mesh_path))
        segment.set_material(0, unreal.load_object(None, material_path))
        segment.attach_to_component(spline, unreal.AttachmentTransformRules(keep_world_transform=False))
        segment.set_start_and_end(
            spline.get_location_at_spline_point(i, unreal.SplineCoordinateSpace.WORLD),
            spline.get_location_at_spline_point(i + 1, unreal.SplineCoordinateSpace.WORLD),
            spline.get_tangent_at_spline_point(i, unreal.SplineCoordinateSpace.WORLD),
            spline.get_tangent_at_spline_point(i + 1, unreal.SplineCoordinateSpace.WORLD),
        )
    return spline_actor


def spawn_npc(location, npc_class, material_path=None):
    """
    Spawns an NPC character at the given location.
    """
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(npc_class, location)
    actor.set_actor_label(f"NPC_{location.x}_{location.y}")
    if material_path:
        mesh_comp = actor.get_component_by_class(unreal.SkeletalMeshComponent)
        if mesh_comp:
            mesh_comp.set_material(0, unreal.load_object(None, material_path))
    return actor


# ------------------------------------------------------------------
# Main city generation function
# ------------------------------------------------------------------
def generate_city():
    # Configuration
    grid_size = 10          # 10x10 grid
    spacing = 2000.0        # Distance between building centers
    building_size = unreal.Vector(1.5, 1.5, 5.0)  # Scale of the cube
    road_width = 200.0
    road_length = spacing
    num_npcs = 60

    # Ensure directories exist
    unreal.EditorAssetLibrary.make_directory("/Game/GeneratedMaterials")
    unreal.EditorAssetLibrary.make_directory("/Game/GeneratedCity")

    # Create base materials
    base_material_path = "/Engine/BasicShapes/BasicMaterial.BasicMaterial"

    building_material = create_material_instance(
        "BuildingMaterial",
        base_material_path,
        unreal.LinearColor(random.random(), random.random(), random.random(), 1.0)
    )
    road_material = create_material_instance(
        "RoadMaterial",
        base_material_path,
        unreal.LinearColor(0.2, 0.2, 0.2, 1.0)
    )
    monorail_material = create_material_instance(
        "MonorailMaterial",
        base_material_path,
        unreal.LinearColor(0.8, 0.8, 0.8, 1.0)
    )

    # Create buildings and roads
    for i in range(grid_size):
        for j in range(grid_size):
            # Building position
            x = i * spacing
            y = j * spacing
            location = unreal.Vector(x, y, 0)
            create_building(location, building_size, building_material)

            # Horizontal road (between buildings)
            if i < grid_size - 1:
                road_loc = unreal.Vector(x + spacing / 2, y, 0)
                create_road(road_loc, spacing, road_width, road_material)

            # Vertical road (between buildings)
            if j < grid_size - 1:
                road_loc = unreal.Vector(x, y + spacing / 2, 0)
                create_road(road_loc, spacing, road_width, road_material)

    # Create monorail spline (circle around the city)
    center = unreal.Vector((grid_size - 1) * spacing / 2, (grid_size - 1) * spacing / 2, 0)
    radius = (grid_size * spacing) / 2 + 500.0
    num_points = 20
    points = []
    for k in range(num_points + 1):
        angle = 2 * math.pi * k / num_points
        px = center.x + radius * math.cos(angle)
        py = center.y + radius * math.sin(angle)
        points.append(unreal.Vector(px, py, 200.0))  # Elevated above ground

    rail_mesh_path = "/Engine/BasicShapes/Cylinder.Cylinder"  # Simple cylinder as rail
    create_monorail_spline(points, rail_mesh_path, monorail_material)

    # Spawn NPCs on roads
    npc_class = unreal.load_class(None, "/Game/StarterContent/Characters/Character_C.Character_C_C")
    if not npc_class:
        unreal.log_error("NPC class not found. Using default Character_C.")
        npc_class = unreal.Character

    for _ in range(num_npcs):
        # Randomly pick a road segment
        road_actor = random.choice(unreal.EditorLevelLibrary.get_all_level_actors_of_class(unreal.StaticMeshActor))
        if not road_actor:
            continue
        mesh_comp = road_actor.get_component_by_class(unreal.StaticMeshComponent)
        if not mesh_comp:
            continue
        bounds = mesh_comp.get_local_bounds()
        # Random point within bounds
        local_point = unreal.Vector(
            random.uniform(bounds.min.x, bounds.max.x),
            random.uniform(bounds.min.y, bounds.max.y),
            0
        )
        world_point = road_actor.get_actor_transform().transform_position(local_point)
        spawn_npc(world_point, npc_class)

    unreal.log("City generation complete.")


# ------------------------------------------------------------------
# Execute the generation
# ------------------------------------------------------------------
if __name__ == "__main__":
    generate_city()
