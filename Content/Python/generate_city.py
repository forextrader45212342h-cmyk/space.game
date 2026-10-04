# Content/Python/generate_city.py
# Unreal Engine 5 Python script to procedurally generate a city with buildings, roads, a monorail spline, and 60 NPCs.
# Requires the Unreal Editor to be running with the Python plugin enabled.

import unreal
import random
import math

# ------------------------------------------------------------------
# Utility functions
# ------------------------------------------------------------------
def get_asset_tools():
    return unreal.AssetToolsHelpers.get_asset_tools()

def create_material(name, package_path, base_color, roughness, metallic):
    """
    Creates a simple material with a constant base color, roughness, and metallic.
    """
    factory = unreal.MaterialFactoryNew()
    material = get_asset_tools().create_asset(name, package_path, None, factory)
    if not material:
        unreal.log_error(f"Failed to create material {name}")
        return None

    # Set up material properties
    unreal.MaterialEditingLibrary.set_material_property(material, unreal.MaterialProperty.MP_BASE_COLOR, base_color)
    unreal.MaterialEditingLibrary.set_material_property(material, unreal.MaterialProperty.MP_ROUGHNESS, roughness)
    unreal.MaterialEditingLibrary.set_material_property(material, unreal.MaterialProperty.MP_METALLIC, metallic)

    # Compile the material
    unreal.MaterialEditingLibrary.compile_material(material)
    return material

def create_box_mesh(name, package_path, width, depth, height):
    """
    Creates a simple box static mesh asset.
    """
    factory = unreal.StaticMeshFactoryNew()
    static_mesh = get_asset_tools().create_asset(name, package_path, None, factory)
    if not static_mesh:
        unreal.log_error(f"Failed to create static mesh {name}")
        return None

    # Build a simple box mesh description
    mesh_desc = unreal.MeshDescription()
    # Create vertices
    v0 = mesh_desc.create_vertex(unreal.Vector(0, 0, 0))
    v1 = mesh_desc.create_vertex(unreal.Vector(width, 0, 0))
    v2 = mesh_desc.create_vertex(unreal.Vector(width, depth, 0))
    v3 = mesh_desc.create_vertex(unreal.Vector(0, depth, 0))
    v4 = mesh_desc.create_vertex(unreal.Vector(0, 0, height))
    v5 = mesh_desc.create_vertex(unreal.Vector(width, 0, height))
    v6 = mesh_desc.create_vertex(unreal.Vector(width, depth, height))
    v7 = mesh_desc.create_vertex(unreal.Vector(0, depth, height))

    # Helper to create a face (polygon) from 4 vertices
    def create_face(v_a, v_b, v_c, v_d):
        poly = mesh_desc.create_polygon([v_a, v_b, v_c, v_d])
        # UVs
        mesh_desc.set_polygon_vertex_uv(poly, 0, unreal.Vector2D(0, 0))
        mesh_desc.set_polygon_vertex_uv(poly, 1, unreal.Vector2D(1, 0))
        mesh_desc.set_polygon_vertex_uv(poly, 2, unreal.Vector2D(1, 1))
        mesh_desc.set_polygon_vertex_uv(poly, 3, unreal.Vector2D(0, 1))

    # Bottom
    create_face(v0, v1, v2, v3)
    # Top
    create_face(v4, v5, v6, v7)
    # Front
    create_face(v0, v1, v5, v4)
    # Back
    create_face(v3, v2, v6, v7)
    # Left
    create_face(v0, v3, v7, v4)
    # Right
    create_face(v1, v2, v6, v5)

    # Assign mesh description to static mesh
    static_mesh.create_mesh_description(mesh_desc)
    static_mesh.build()
    static_mesh.post_edit_change()
    static_mesh.mark_package_dirty()
    return static_mesh

def spawn_static_mesh_actor(name, mesh, location, rotation=unreal.Rotator(0, 0, 0), scale=unreal.Vector(1, 1, 1), material=None):
    """
    Spawns a StaticMeshActor in the current level with the given mesh and optional material.
    """
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.set_actor_label(name)
    actor.static_mesh_component.set_static_mesh(mesh)
    actor.static_mesh_component.set_world_scale3d(scale)
    if material:
        actor.static_mesh_component.set_material(0, material)
    return actor

def create_spline_actor(name, points, spline_mesh=None, spline_mesh_scale=unreal.Vector(1, 1, 1)):
    """
    Creates a spline actor with the given points and optionally attaches a mesh along the spline.
    """
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.SplineActor, unreal.Vector(0, 0, 0))
    actor.set_actor_label(name)
    spline = actor.spline_component
    for pt in points:
        spline.add_spline_point(pt, unreal.SplineCoordinateSpace.WORLD, True)
    spline.update_trajectory()
    if spline_mesh:
        # Attach a static mesh component that follows the spline
        mesh_comp = unreal.StaticMeshComponent()
        mesh_comp.register_component()
        mesh_comp.set_static_mesh(spline_mesh)
        mesh_comp.set_world_scale3d(spline_mesh_scale)
        mesh_comp.attach_to_component(spline, unreal.AttachmentRule.SNAP_TO_TARGET, unreal.AttachmentRule.SNAP_TO_TARGET, unreal.AttachmentRule.SNAP_TO_TARGET, True)
        mesh_comp.set_relative_location(unreal.Vector(0, 0, 0))
    return actor

def spawn_npc(name, character_class, location, rotation=unreal.Rotator(0, 0, 0)):
    """
    Spawns an NPC character at the given location.
    """
    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(character_class, location, rotation)
    actor.set_actor_label(name)
    return actor

# ------------------------------------------------------------------
# City generation
# ------------------------------------------------------------------
def generate_city():
    # Configuration
    city_size = 10  # 10x10 grid
    building_spacing = 300.0
    building_min_height = 200.0
    building_max_height = 800.0
    road_width = 100.0
    road_length = building_spacing
    monorail_height = 200.0
    npc_count = 60

    # Asset directories
    mat_dir = "/Game/ProceduralAssets/Materials"
    mesh_dir = "/Game/ProceduralAssets/StaticMeshes"
    building_dir = "/Game/ProceduralAssets/Buildings"
    road_dir = "/Game/ProceduralAssets/Roads"
    monorail_dir = "/Game/ProceduralAssets/Monorail"

    # Create a simple building material
    building_mat = create_material(
        "Building_Mat",
        mat_dir,
        unreal.LinearColor(0.6, 0.6, 0.6, 1.0),
        0.8,
        0.0
    )

    # Create a simple road material
    road_mat = create_material(
        "Road_Mat",
        mat_dir,
        unreal.LinearColor(0.2, 0.2, 0.2, 1.0),
        0.9,
        0.0
    )

    # Create a simple monorail rail mesh
    rail_mesh = create_box_mesh(
        "Monorail_Rail",
        mesh_dir,
        width=20.0,
        depth=5.0,
        height=2.0
    )
    if rail_mesh:
        rail_mat = create_material(
            "Monorail_Rail_Mat",
            mat_dir,
            unreal.LinearColor(0.8, 0.8, 0.8, 1.0),
            0.3,
            0.5
        )
        rail_mesh.static_mesh_component.set_material(0, rail_mat)

    # Generate buildings
    for i in range(city_size):
        for j in range(city_size):
            # Random building dimensions
            width = random.uniform(80.0, 120.0)
            depth = random.uniform(80.0, 120.0)
            height = random.uniform(building_min_height, building_max_height)

            # Create building mesh
            mesh_name = f"Building_Mesh_{i}_{j}"
            building_mesh = create_box_mesh(
                mesh_name,
                building_dir,
                width,
                depth,
                height
            )
            if not building_mesh:
                continue

            # Apply material
            building_mesh.static_mesh_component.set_material(0, building_mat)

            # Position
            x = i * building_spacing
            y = j * building_spacing
            z = height / 2.0  # Center the building on the ground
            location = unreal.Vector(x, y, z)

            # Spawn actor
            spawn_static_mesh_actor(
                f"Building_{i}_{j}",
                building_mesh,
                location,
                scale=unreal.Vector(1, 1, 1),
                material=building_mat
            )

    # Generate roads (grid lines)
    for i in range(city_size + 1):
        # Horizontal roads
        x = i * building_spacing
        for j in range(city_size):
            y = j * building_spacing
            location = unreal.Vector(x, y, 0.0)
            spawn_static_mesh_actor(
                f"Road_H_{i}_{j}",
                create_box_mesh(
                    f"Road_H_Mesh_{i}_{j}",
                    road_dir,
                    width=road_width,
                    depth=road_length,
                    height=1.0
                ),
                location,
                rotation=unreal.Rotator(0, 90, 0),
                scale=unreal.Vector(1, 1, 1),
                material=road_mat
            )
        # Vertical roads
        y =