"""
GB37Y Motor Mount - SQUARE BOX
Square enclosure prevents motor from rotating inside.
"""

import cadquery as cq

# ============================================
# MOTOR PARAMETERS (GB37Y)
# ============================================
MOTOR_BODY_DIA = 37.0       # mm
GEARBOX_LENGTH = 24.0       # mm
MOTOR_LENGTH = 29.0         # mm
TOTAL_MOTOR_LENGTH = 6 + GEARBOX_LENGTH + MOTOR_LENGTH  # ~59mm

SHAFT_DIA = 6.0             # mm
MOUNT_BOLT_CIRCLE = 31.0    # mm
MOUNT_HOLE_DIA = 3.2        # mm (M3 clearance)
NUM_MOUNT_HOLES = 6

# ============================================
# BOX PARAMETERS
# ============================================
WALL_THICKNESS = 3.0        # mm
CLEARANCE = 0.5             # mm
BASE_THICKNESS = 4.0        # mm

# Square box - inner side must fit motor diameter
INNER_SIDE = MOTOR_BODY_DIA + CLEARANCE * 2  # 38mm
OUTER_SIDE = INNER_SIDE + WALL_THICKNESS * 2  # 44mm
BOX_HEIGHT = TOTAL_MOTOR_LENGTH + BASE_THICKNESS

print(f"Box dimensions: {OUTER_SIDE:.1f} x {OUTER_SIDE:.1f} x {BOX_HEIGHT:.1f} mm")
print(f"Wheel radius should be: {OUTER_SIDE/2:.1f} mm")

# ============================================
# BUILD SQUARE MOTOR BOX
# ============================================

# Outer box
box = (
    cq.Workplane("XY")
    .box(OUTER_SIDE, OUTER_SIDE, BOX_HEIGHT)
    .translate((0, 0, BOX_HEIGHT / 2))  # bottom at Z=0
)

# Hollow out (cylindrical hole for motor)
box = (
    box
    .faces(">Z")
    .workplane()
    .circle(MOTOR_BODY_DIA / 2 + CLEARANCE)
    .cutBlind(-(BOX_HEIGHT - BASE_THICKNESS))
)

# Shaft clearance hole through base
box = (
    box
    .faces("<Z")
    .workplane()
    .circle(SHAFT_DIA / 2 + 1)
    .cutThruAll()
)

# Motor mounting holes (6x M3)
box = (
    box
    .faces("<Z")
    .workplane()
    .polygon(NUM_MOUNT_HOLES, MOUNT_BOLT_CIRCLE, forConstruction=True)
    .vertices()
    .circle(MOUNT_HOLE_DIA / 2)
    .cutThruAll()
)

# ============================================
# EXPORT
# ============================================
# cq.exporters.export(box, "gb37y_motor_box.step")
cq.exporters.export(box, "./Tools/models/gb37y_motor_box.stl")

print(f"\nExported: gb37y_motor_box.step")
print(f"Exported: gb37y_motor_box.stl")
print(f"\nUse wheel diameter = {OUTER_SIDE:.1f} mm (radius = {OUTER_SIDE/2:.1f} mm)")