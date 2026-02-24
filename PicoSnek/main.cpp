#include "pch.h"

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "snek/snek.h"
#include "snek/entity.h"
#include <string>

extern "C" {
    #include "EPD_4in2_V2.h"
    #include "DEV_Config.h"
    #include "GUI_Paint.h"
}

UBYTE *BlackImage;
UWORD Imagesize = ((EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1)) * EPD_4IN2_V2_HEIGHT;

struct stickInput
{
    float x_axis;
    float y_axis;
    bool pressed;
};

stickInput GetInput()
{
    adc_select_input(1);
    uint16_t y_raw = adc_read();
    float y_norm = -(y_raw - 2048.0) / 2048.0;
    adc_select_input(0);
    uint16_t x_raw = adc_read();
    float x_norm = -(x_raw - 2048.0) / 2048.0;
    bool z_pressed = !gpio_get(22);
    if (abs(x_norm) > abs(y_norm))
    {
        y_norm = 0;
    }
    else
    {
        x_norm = 0;
    }
    return stickInput{
        x_axis: abs(x_norm) < 0.2 ? 0.0f : ((signbit(x_norm)) ? -1.0f : 1.0f),
        y_axis: abs(y_norm) < 0.2 ? 0.0f : ((signbit(y_norm)) ? -1.0f : 1.0f),
        pressed: z_pressed
    };
}

int main()
{
    stdio_init_all();

    adc_init();

    adc_gpio_init(26);
    adc_gpio_init(27);

    const uint LED_PIN = 16;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // epaper init
    if(DEV_Module_Init()!=0)
    {
        printf("fail");
        return -1;
    }

    EPD_4IN2_V2_Init();
    EPD_4IN2_V2_Clear();    
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    Paint_NewImage(BlackImage, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    EPD_4IN2_V2_Display(BlackImage);
    // DEV_Delay_ms(2000);
    // end epaper init

    grid gameGrid(1,1, 16, 16, 16, BlackImage);
    std::string playerName = "p";
    entity player (playerName, 1, 1, gameGrid);

    gameGrid.SetImage();
    gameGrid.DrawBox();
    gameGrid.Display();

    sleep_ms(500);

    player.Draw();
    int xspeed = 1;

    while (true) {
        gpio_put(LED_PIN, 0);
        
        stickInput input = GetInput();
        player.TryMove(input.x_axis, -input.y_axis);
        
        gpio_put(LED_PIN, 0);
    }

    return 0;
}
