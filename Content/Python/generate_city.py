# Content/Python/generate_city.py
# ------------------------------------------------------------
# Unreal Engine 5 Python script to auto‑generate a city with
# buildings, roads, a monorail spline, 60 NPCs and traffic lights.
# ------------------------------------------------------------
# Run this script inside the Unreal Editor (Python console or
# via the Content Browser > Scripts > Run Script).
# ------------------------------------------------------------

import unreal
import random
import math

# ------------------------------------------------------------------
# Configuration
# ------------------------------------------------------------------
ASSET_DIR = "/Game/GeneratedCity"
BUILDING