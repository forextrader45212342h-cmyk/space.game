# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script – City Generator (HAB‑12)
# ------------------------------------------------------------
# This script creates a simple procedural city with:
#   • Buildings (randomly sized, using a base static mesh)
#   • Roads (grid layout)
#   • Monorail spline (with rail and support meshes)
#   • 60 NPC traffic actors (simple AI pawns)
#   • Auto‑generated materials (basic color + roughness)
#
# Run this script from the Unreal Editor's Python console or
# via the "Run Python Script" button in the Content Browser.
#
# ------------------------------------------------------------
# Imports
# ------------------------------------------------------------
import unreal
import random
import math

# ------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------

def create_material(name, color, roughness=0.5, metallic=0.0):
    """
    Creates a simple material instance with a base color, roughness, and metallic.
    """
    # Load the base material (must exist in the project)
    base_mat_path = "/Game/StarterContent/Materials/M_Basic_Wall.M_Basic_Wall"
    base_mat = unreal.EditorAssetLibrary.load_asset(base_mat_path)

    # Create a new material instance
    mat_tools = unreal.MaterialEditingLibrary
    mat_path = f"/Game/CityMaterials/{name}"
    mat = mat_tools.create_material_instance(mat_path, base_mat)

    # Set parameters
    mat_tools.set_material_instance_vector_parameter_value(mat, "BaseColor", unreal.LinearColor(*color))
    mat_tools.set_material_instance_scalar_parameter_value(mat, "Roughness", roughness)
    mat_tools.set_material_instance_scalar_parameter_value(mat, "Metallic", metallic)

    return mat

def spawn_static_mesh(mesh_path, location, rotation, scale, material=None):
    """
    Spawns a static mesh actor at the given transform.
    """
    # Load the mesh
    mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not mesh:
        unreal.log_error(f"Mesh not found: {mesh_path}")
        return None

    # Spawn the actor
    actor = unreal.EditorLevelLibrary.spawn_actor_from_object(mesh, location, rotation)
    if material:
        # Apply material to the first component
        component = actor.get_component_by_class(unreal.StaticMeshComponent)
        if component:
            component.set_material(0, material)

    # Scale
    actor.set_actor_scale3d(scale)
    return actor

def create_building(location, size, material):
    """
    Creates a simple building using a cube mesh scaled to the given size.
    """
    # Use a cube mesh from StarterContent
    cube_mesh_path = "/Game/StarterContent/Architecture/SM_Cube.SM_Cube"
    scale = unreal.Vector(size.x / 100.0, size.y / 100.0, size.z / 100.0)  # Cube is 100 units
    return spawn_static_mesh(cube_mesh_path, location, unreal.Rotator(0, 0, 0), scale, material)

def create_road_segment(start, end, width=200.0):
    """
    Creates a road segment as a flat plane between two points.
    """
    # Use a plane mesh
    plane_mesh_path = "/Game/StarterContent/Architecture/SM_Plane.SM_Plane"
    length = (end - start).size()
    location = (start + end) * 0.5
    rotation = unreal.Rotator(0, math.degrees(math.atan2(end.y - start.y, end.x - start.x)), 0)
    scale = unreal.Vector(length / 100.0, width / 100.0, 1.0)  # Plane is 100x100 units
    mat = create_material("RoadMat", (0.1, 0.1, 0.1), roughness=0.8)
    return spawn_static_mesh(plane_mesh_path, location, rotation, scale, mat)

def create_monorail_spline(points, rail_mesh_path, support_mesh_path):
    """
    Creates a spline for the monorail and spawns rail and support meshes along it.
    """
    # Create a spline actor
    spline_actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0,0,0))
    spline = spline_actor.get_spline_component()

    # Add points
    for i, pt in enumerate(points):
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD, True)

    # Create rail mesh along spline
    for i in range(len(points) - 1):
        start = points[i]
        end = points[i+1]
        mid = (start + end) * 0.5
        dir_vec = (end - start).get_safe_normal()
        rot = unreal.Rotator(0, math.degrees(math.atan2(dir_vec.y, dir_vec.x)), 0)
        length = (end - start).size()
        scale = unreal.Vector(length / 100.0, 1.0, 1.0)
        spawn_static_mesh(rail_mesh_path, mid, rot, scale)

    # Create support meshes at each point
    for pt in points:
        spawn_static_mesh(support_mesh_path, pt, unreal.Rotator(0,0,0), unreal.Vector(1,1,1))

    return spline_actor

def spawn_npc(location, rotation):
    """
    Spawns a simple AI pawn (e.g., a character) at the given location.
    """
    # Use a default character blueprint
    char_bp_path = "/Game/Blueprints/AI/BP_StreetPerson.BP_StreetPerson"
    char_bp = unreal.EditorAssetLibrary.load_asset(char_bp_path)
    if not char_bp:
        unreal.log_error(f"Character BP not found: {char_bp_path}")
        return None
    return unreal.EditorLevelLibrary.spawn_actor_from_object(char_bp, location, rotation)

# ------------------------------------------------------------
# Main city generation logic
# ------------------------------------------------------------

def generate_city():
    # City parameters
    city_size = 2000  # 2000x2000 units
    grid_spacing = 400
    building_min_size = unreal.Vector(200, 200, 200)
    building_max_size = unreal.Vector(400, 400, 800)
    road_width = 200

    # Create a folder for city assets
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    city_folder = "/Game/City"
    if not unreal.EditorAssetLibrary.does_directory_exist(city_folder):
        asset_tools.create_unique_asset_name(city_folder, "")

    # Generate roads (grid)
    for i in range(-city_size // grid_spacing, city_size // grid_spacing + 1):
        # Horizontal roads
        start = unreal.Vector(i * grid_spacing, -city_size, 0)
        end = unreal.Vector(i * grid_spacing, city_size, 0)
        create_road_segment(start, end, width=road_width)

        # Vertical roads
        start = unreal.Vector(-city_size, i * grid_spacing, 0)
        end = unreal.Vector(city_size, i * grid_spacing, 0)
        create_road_segment(start, end, width=road_width)

    # Generate buildings
    for i in range(-city_size // grid_spacing, city_size // grid_spacing):
        for j in range(-city_size // grid_spacing, city_size // grid_spacing):
            # Skip road cells
            if i % 2 == 0 or j % 2 == 0:
                continue

            # Random building size
            size = unreal.Vector(
                random.uniform(building_min_size.x, building_max_size.x),
                random.uniform(building_min_size.y, building_max_size.y),
                random.uniform(building_min_size.z, building_max_size.z)
            )

            # Position
            pos = unreal.Vector(
                i * grid_spacing + random.uniform(-grid_spacing/4, grid_spacing/4),
                j * grid_spacing + random.uniform(-grid_spacing/4, grid_spacing/4),
                size.z / 2
            )

            # Random color for building
            color = (
                random.uniform(0.3, 0.9),
                random.uniform(0.3, 0.9),
                random.uniform(0.3, 0.9)
            )
            mat = create_material(f"BuildingMat_{i}_{j}", color, roughness=0.6)

            create_building(pos, size, mat)

    # Create monorail spline
    monorail_points = [
        unreal.Vector(-city_size, -city_size, 300),
        unreal.Vector(-city_size, city_size, 300),
        unreal.Vector(city_size, city_size, 300),
        unreal.Vector(city_size, -city_size, 300)
    ]
    rail_mesh_path = "/Game/StarterContent/Architecture/SM_Rail.SM_Rail"  # placeholder
    support_mesh_path = "/Game/StarterContent/Architecture/SM_Support.SM_Support"  # placeholder
    create_monorail_spline(monorail_points, rail_mesh_path, support_mesh_path)

    # Spawn NPC traffic
    for _ in range(60):
        # Random position near roads
        road_x = random.choice([-city_size, city_size])  # left or right side
        road_y = random.uniform(-city_size, city_size)
        pos = unreal.Vector(road_x, road_y, 0)
        rot = unreal.Rotator(0, random.uniform(0, 360), 0)
        spawn_npc(pos, rot)

    unreal.log("City generation complete!")

# ------------------------------------------------------------
# Execute
# ------------------------------------------------------------
if __name__ == "__main__":
    generate_city()
