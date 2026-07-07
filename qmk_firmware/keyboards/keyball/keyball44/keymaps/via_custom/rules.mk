RGBLIGHT_ENABLE = yes

OLED_ENABLE = yes

VIA_ENABLE = yes

LTO_ENABLE = yes

# Command Layer
SRC += features/command_layer.c

# OLED Custom
SRC += features/oled.c

# KEM Core
SRC += features/kem.c

# KEM Layer
SRC += features/kem_layer.c

# KEM LED
SRC += features/kem_led.c

# KEM Debug
SRC += features/kem_debug.c

# KEM Debug Output
SRC += features/kem_debug_output.c