add_library( LCD_I2C
    LCD_I2C.h
    LCD_I2C.cpp
    )
    
target_include_directories(LCD_I2C PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(LCD_I2C
        hardware_i2c
)