idf_component_register(SRCS "main.c"
							"srcs/bluetooth.c"
							"srcs/blufi_security.c"
							"srcs/wifi.c"
							"srcs/timers.c"
							"srcs/gpio_intr.c"
							"srcs/misc.c"
							"srcs/tasks.c"
							"srcs/tftspi.c"
							"srcs/tft.c"
							"srcs/memory.c"
                    INCLUDE_DIRS "include/")
spiffs_create_partition_image(storage ../spiffs_image FLASH_IN_PROJECT)
