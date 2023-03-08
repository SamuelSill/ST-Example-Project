from os import listdir

x = listdir("Drivers/STM32H7xx_HAL_Driver/Src")
for f in x:
    if not f.endswith("template.c"):
        print(f"BSP/Drivers/STM32H7xx_HAL_Driver/Src/{f}")
