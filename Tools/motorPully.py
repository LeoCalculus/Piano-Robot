"""
GT2 Timing Belt Wheel
Radius = Box side / 2 = 22mm (diameter 44mm)
So wheel touches ground when box sits flat.
"""

import cadquery as cq
import math

# ============================================
# WHEEL SIZE (matches box)
# ============================================
BOX_SIDE = 44.0             # mm - from motor box
WHEEL_DIAMETER = BOX_SIDE   # 44mm - radius = 22mm = box_side/2
WHEEL_RADIUS = WHEEL_DIAMETER / 2

# ============================================
# GT2 BELT PARAMETERS
# ============================================
GT2_PITCH = 2.0             # mm
BELT_WIDTH = 6.0            # mm
PULLEY_WIDTH = 8.0          # mm
FLANGE_WIDTH = 1.5          # mm
FLANGE_HEIGHT = 2.0         # mm

TOOTH_DEPTH = 0.75          # mm

# Calculate teeth for this diameter
NUM_TEETH = int((WHEEL_DIAMETER * math.pi) / GT2_PITCH)
ACTUAL_DIAMETER = (NUM_TEETH * GT2_PITCH) / math.pi

print(f"Wheel: {ACTUAL_DIAMETER:.2f}mm diameter, {NUM_TEETH} teeth")

# ============================================
# SHAFT (GB37Y motor)
# ============================================
SHAFT_DIA = 6.0             # mm
SHAFT_FLAT_DEPTH = 0.5      # mm
BORE_DEPTH = 15.0           # mm

HUB_DIA = 14.0              # mm
HUB_LENGTH = 12.0           # mm

SET_SCREW_DIA = 3.0         # mm (M3)

# ============================================
# GT2 TOOTH PROFILE
# ============================================
def gt2_tooth_profile(num_teeth, pitch=2.0, tooth_depth=0.75):
    pitch_radius = (num_teeth * pitch) / (2 * math.pi)
    outer_radius = pitch_radius + tooth_depth
    root_radius = pitch_radius - tooth_depth
    
    points = []
    tooth_angle = 2 * math.pi / num_teeth
    
    for i in range(num_teeth):
        base_angle = i * tooth_angle
        
        # Root -> rise -> tip -> fall -> root
        a1 = base_angle
        points.append((root_radius * math.cos(a1), root_radius * math.sin(a1)))
        
        a2 = base_angle + tooth_angle * 0.15
        points.append((root_radius * math.cos(a2), root_radius * math.sin(a2)))
        
        a3 = base_angle + tooth_angle * 0.25
        points.append((outer_radius * math.cos(a3), outer_radius * math.sin(a3)))
        
        a4 = base_angle + tooth_angle * 0.50
        points.append((outer_radius * math.cos(a4), outer_radius * math.sin(a4)))
        
        a5 = base_angle + tooth_angle * 0.60
        points.append((root_radius * math.cos(a5), root_radius * math.sin(a5)))
        
        a6 = base_angle + tooth_angle * 0.85
        points.append((root_radius * math.cos(a6), root_radius * math.sin(a6)))
    
    return points

# ============================================
# BUILD WHEEL
# ============================================

# Toothed profile
tooth_points = gt2_tooth_profile(NUM_TEETH, GT2_PITCH, TOOTH_DEPTH)

wheel = (
    cq.Workplane("XY")
    .polyline(tooth_points)
    .close()
    .extrude(PULLEY_WIDTH)
)

# Flanges
pitch_radius = (NUM_TEETH * GT2_PITCH) / (2 * math.pi)
flange_outer = pitch_radius + TOOTH_DEPTH + FLANGE_HEIGHT

bottom_flange = (
    cq.Workplane("XY")
    .circle(flange_outer)
    .extrude(FLANGE_WIDTH)
)

top_flange = (
    cq.Workplane("XY")
    .workplane(offset=PULLEY_WIDTH - FLANGE_WIDTH)
    .circle(flange_outer)
    .extrude(FLANGE_WIDTH)
)

wheel = wheel.union(bottom_flange).union(top_flange)

# Hub
hub = (
    cq.Workplane("XY")
    .circle(HUB_DIA / 2)
    .extrude(HUB_LENGTH)
)
wheel = wheel.union(hub)

# Shaft bore
wheel = (
    wheel
    .faces("<Z")
    .workplane()
    .circle(SHAFT_DIA / 2 + 0.1)
    .cutBlind(BORE_DEPTH)
)

# D-flat cut
wheel = (
    wheel
    .faces("<Z")
    .workplane()
    .center(SHAFT_DIA / 2, 0)
    .rect(SHAFT_FLAT_DEPTH * 2, SHAFT_DIA)
    .cutBlind(BORE_DEPTH)
)

# Set screw hole
wheel = (
    wheel
    .faces(">X")
    .workplane(centerOption="CenterOfMass")
    .center(0, -HUB_LENGTH / 2 + BORE_DEPTH / 2)
    .hole(SET_SCREW_DIA, 8)
)

# ============================================
# EXPORT
# ============================================
cq.exporters.export(wheel, "./Tools/models/gt2_wheel.step")
#cq.exporters.export(wheel, "./Tools/models/gt2_wheel.stl")

print(f"\nExported: gt2_wheel.step")
print(f"Exported: gt2_wheel.stl")
print(f"\nWheel radius ({WHEEL_RADIUS:.1f}mm) = Box side/2 ({BOX_SIDE/2:.1f}mm) ✓")