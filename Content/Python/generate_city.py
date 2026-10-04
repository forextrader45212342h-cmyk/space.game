# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a simple city
# with buildings, roads, a monorail spline, and 60 NPCs.
# ------------------------------------------------------------
import unreal
import random
import math

# ------------------------------------------------------------------
# Utility helpers
# ------------------------------------------------------------------
def log(msg):
    unreal.log("[GenerateCity] " + msg)

# ------------------------------------------------------------------
# Asset creation helpers
# ------------------------------------------------------------------
def create_material(name, base_color, package_path="/Game/GeneratedCity"):
    """
    Create a simple material with a given base color.
    """
    factory = unreal.MaterialFactoryNew()
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    material = asset_tools.create_asset(name, package_path, None, factory)
    if not material:
        log(f"Failed to create material {name}")
        return None

    # Set the Base Color parameter
    unreal.MaterialEditingLibrary.set_material_property_value(
        material, "BaseColor", unreal.MaterialEditingLibrary.get_material_parameter_value(material, "BaseColor")
    )
    unreal.MaterialEditingLibrary.set_material_property_value(
        material, "BaseColor", unreal.LinearColor(base_color.r, base_color.g, base_color.b, 1.0)
    )
    unreal.MaterialEditingLibrary.set_material_property(material, "BlendMode", unreal.BlendMode.BLEND_Opaque)
    unreal.MaterialEditingLibrary.set_material_property(material, "ShadingModel", unreal.ShadingModel.SM_DefaultLit)

    # Compile the material
    unreal.MaterialEditingLibrary.compile_material(material)
    log(f"Created material {name}")
    return material

def create_building_material(name, color):
    return create_material(name, color)

def create_road_material(name):
    # Grey road
    return create_material(name, unreal.LinearColor(0.3, 0.3, 0.3))

def create_monorail_material(name):
    # Metallic blue
    return create_material(name, unreal.LinearColor(0.1, 0.2, 0.8))

# ------------------------------------------------------------------
# Actor spawning helpers
# ------------------------------------------------------------------
def spawn_static_mesh_actor(mesh_path, location, scale, material=None, rotation=unreal.Rotator(0,0,0)):
    """
    Spawn a static mesh actor at the given location.
    """
    mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not mesh:
        log(f"Failed to load mesh {mesh_path}")
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.static_mesh_component.set_static_mesh(mesh)
    actor.static_mesh_component.set_world_scale3d(scale)
    if material:
        actor.static_mesh_component.set_material(0, material)
    return actor

def spawn_building(location, height, material):
    """
    Spawn a simple box building with random height.
    """
    scale = unreal.Vector(1.0, 1.0, height)
    return spawn_static_mesh_actor(
        "/Engine/BasicShapes/Cube.Cube",
        location,
        scale,
        material
    )

def spawn_road_spline(start, end, material):
    """
    Create a simple straight road spline between start and end.
    """
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, start)
    spline = spline_actor.get_spline_component()
    spline.add_spline_point(start, unreal.SplineCoordinateSpace.WORLD)
    spline.add_spline_point(end, unreal.SplineCoordinateSpace.WORLD)
    spline.update_spline()

    # Create a road mesh along the spline
    road_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Plane.Plane")
    if not road_mesh:
        log("Failed to load plane mesh for road")
        return None

    # Add a spline mesh component
    spline_mesh = spline_actor.add_spline_mesh_component()
    spline_mesh.set_static_mesh(road_mesh)
    spline_mesh.set_material(0, material)
    spline_mesh.set_start_and_end(
        spline.get_location_at_spline_point(0, unreal.SplineCoordinateSpace.WORLD),
        spline.get_location_at_spline_point(1, unreal.SplineCoordinateSpace.WORLD),
        spline.get_tangent_at_spline_point(0, unreal.SplineCoordinateSpace.WORLD),
        spline.get_tangent_at_spline_point(1, unreal.SplineCoordinateSpace.WORLD)
    )
    spline_mesh.set_start_scale(unreal.Vector(1.0, 0.1, 1.0))
    spline_mesh.set_end_scale(unreal.Vector(1.0, 0.1, 1.0))
    return spline_actor

def spawn_monorail_spline(points, material):
    """
    Create a monorail spline passing through the given points.
    """
    if not points:
        return None
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, points[0])
    spline = spline_actor.get_spline_component()
    for pt in points:
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD)
    spline.update_spline()

    # Add a rail mesh along the spline
    rail_mesh = unreal.EditorAssetLibrary.load_asset("/Engine/BasicShapes/Cylinder.Cylinder")
    if not rail_mesh:
        log("Failed to load cylinder mesh for monorail")
        return None

    # For each segment, add a spline mesh component
    for i in range(len(points) - 1):
        seg_start = spline.get_location_at_spline_point(i, unreal.SplineCoordinateSpace.WORLD)
        seg_end = spline.get_location_at_spline_point(i+1, unreal.SplineCoordinateSpace.WORLD)
        seg_tangent_start = spline.get_tangent_at_spline_point(i, unreal.SplineCoordinateSpace.WORLD)
        seg_tangent_end = spline.get_tangent_at_spline_point(i+1, unreal.SplineCoordinateSpace.WORLD)

        spline_mesh = spline_actor.add_spline_mesh_component()
        spline_mesh.set_static_mesh(rail_mesh)
        spline_mesh.set_material(0, material)
        spline_mesh.set_start_and_end(seg_start, seg_end, seg_tangent_start, seg_tangent_end)
        spline_mesh.set_start_scale(unreal.Vector(0.1, 0.1, 0.1))
        spline_mesh.set_end_scale(unreal.Vector(0.1, 0.1, 0.1))
    return spline_actor

def spawn_npc(location):
    """
    Spawn a default character at the given location.
    """
    # Use the default character blueprint
    char_bp = "/Engine/Character/DefaultPawn.DefaultPawn"
    if not unreal.EditorAssetLibrary.does_asset_exist(char_bp):
        log("DefaultPawn asset not found")
        return None
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.Character, location)
    return actor

# ------------------------------------------------------------------
# City generation logic
# ------------------------------------------------------------------
def generate_city():
    log("Starting city generation...")

    # Create a folder for generated assets
    package_path = "/Game/GeneratedCity"
    if not unreal.EditorAssetLibrary.does_directory_exist(package_path):
        unreal.EditorAssetLibrary.make_directory(package_path)

    # Materials
    building_mat = create_building_material("CityBuildingMat", unreal.LinearColor(random.random(), random.random(), random.random()))
    road_mat = create_road_material("CityRoadMat")
    monorail_mat = create_monorail_material("CityMonorailMat")

    # City parameters
    city_size = 2000  # units
    grid_spacing = 200
    building_height_min = 3
    building_height_max = 10

    # Generate buildings on a grid
    for x in range(-city_size, city_size, grid_spacing):
        for y in range(-city_size, city_size, grid_spacing):
            # Skip center to leave space for roads
            if abs(x) < grid_spacing and abs(y) < grid_spacing:
                continue
            # Random height
            height = random.uniform(building_height_min, building_height_max)
            location = unreal.Vector(x, y, height * 50)  # 50 units per height unit
            spawn_building(location, height, building_mat)

    # Generate roads (simple grid)
    for x in range(-city_size, city_size + grid_spacing, grid_spacing):
        start = unreal.Vector(x, -city_size, 0)
        end = unreal.Vector(x, city_size, 0)
        spawn_road_spline(start, end, road_mat)

    for y in range(-city_size, city_size + grid_spacing, grid_spacing):
        start = unreal.Vector(-city_size, y, 0)
        end = unreal.Vector(city_size, y, 0)
        spawn_road_spline(start, end, road_mat)

    # Generate a simple monorail loop
    monorail_points = [
        unreal.Vector(-city_size, -city_size, 300),
        unreal.Vector(city_size, -city_size, 300),
        unreal.Vector(city_size, city_size, 300),
        unreal.Vector(-city_size, city_size, 300),
        unreal.Vector(-city_size, -city_size, 300)
    ]
    spawn_monorail_spline(monorail_points, monorail_mat)

    # Spawn NPCs along roads
    road_splines = unreal.EditorLevelLibrary.get_all_level_actors_of_class(unreal.SplineActor)
    for i in range(60):
        # Pick a random road spline
        spline_actor = random.choice(road_splines)
        spline = spline_actor.get_spline_component()
        spline_length = spline.get_spline_length()
        # Random distance along spline
        distance = random.uniform(0, spline_length)
        location = spline.get_location_at_distance_along_spline(distance, unreal.SplineCoordinateSpace.WORLD)
        # Slight offset to avoid overlapping with road
        offset = unreal.Vector(0, 0, 100)
        spawn_npc(location + offset)

    log("City generation complete.")

# ------------------------------------------------------------------
# Execute
# ------------------------------------------------------------------
if __name__ == "__main__":
    generate_city()
