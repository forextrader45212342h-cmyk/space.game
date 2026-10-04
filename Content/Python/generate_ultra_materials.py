# Content/Python/generate_ultra_materials.py
# ------------------------------------------------------------
# This script automatically creates four high‑quality UE5 materials:
#   1. MasterMaterial_LumenNanite          – Lumen‑compatible, Nanite & Ray‑traced reflections
#   2. Kobayashi_EmissiveNeon              – Neon emissive material
#   3. WetStreet_PBR                       – Wet pavement with puddle effect
#   4. NectarCafe_Sunny                    – Sunny cafe interior material
#
# Place this file in Content/Python and run it from the UE5 Editor.
# ------------------------------------------------------------

import