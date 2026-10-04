# Content/Python/generate_city.py
# ------------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a realistic city
# with buildings, roads, a monorail spline, and 60 NPC traffic.
# ------------------------------------------------------------------

import unreal
import random
import math

# ------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------

def create_material(name, base_color, roughness, metallic):
    """
    Create a simple material with a base color, roughness, and metallic
    properties. Returns the path to the created material.
    """
    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    factory = unreal.MaterialFactoryNew()
    package_path = "/Game/CityAssets/Materials"
    asset_name = name

    # Create the material asset
    material = asset_tools.create_asset(asset_name, package_path, unreal.Material, factory)
    if not material:
        unreal.log_error(f"Failed to create material {asset_name}")
        return None

    # Set up the material properties
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MP_BASE_COLOR, base_color
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MP_ROUGHNESS, roughness
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MP_METALLIC, metallic
    )
    unreal.MaterialEditingLibrary.set_material_property(
        material, unreal.MaterialProperty.MP_SHADING_MODEL, unreal.ShadingModel.SMOOTH
    )

    # Save the asset
    unreal.EditorAssetLibrary.save_asset(material.get_path_name())
    return material.get_path_name()


def spawn_static_mesh_actor(mesh_path, location, rotation, scale, material_path=None):
    """
    Spawn a static mesh actor at the given location/rotation/scale.
    Optionally apply a material.
    """
    mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not mesh:
        unreal.log_error(f"Mesh not found: {mesh_path}")
        return None

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.StaticMeshActor, location, rotation)
    actor.static_mesh_component.set_static_mesh(mesh)
    actor.static_mesh_component.set_world_scale3d(scale)

    if material_path:
        material = unreal.EditorAssetLibrary.load_asset(material_path)
        if material:
            actor.static_mesh_component.set_material(0, material)

    return actor


def create_building_grid(grid_size, spacing, building_mesh_path, material_paths):
    """
    Create a grid of buildings. Each building gets a random material from material_paths.
    """
    for i in range(grid_size):
        for j in range(grid_size):
            # Random scale for variation
            scale_x = random.uniform(0.8, 1.5)
            scale_y = random.uniform(0.8, 1.5)
            scale_z = random.uniform(3.0, 10.0)

            location = unreal.Vector(
                i * spacing,
                j * spacing,
                scale_z * 50.0  # half the height to place on ground
            )
            rotation = unreal.Rotator(0, 0, 0)
            scale = unreal.Vector(scale_x, scale_y, scale_z)

            material_path = random.choice(material_paths)
            spawn_static_mesh_actor(
                building_mesh_path,
                location,
                rotation,
                scale,
                material_path
            )


def