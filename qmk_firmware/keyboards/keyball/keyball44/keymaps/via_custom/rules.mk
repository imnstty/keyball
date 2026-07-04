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