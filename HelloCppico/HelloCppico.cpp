extern "C" {
    #include "EPD_4in2_V2.h"
    #include "DEV_Config.h"
    #include "GUI_Paint.h"
}

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include <string.h>
#include <stdio.h>
#include <math.h>


const uint LED_PIN = 16;
const uint ADC_X = 26;
const uint ADC_Y = 27;
const uint SW_PIN = 22;
const float xDeadZone = 0.05f;
const float yDeadZone = 0.15f;

uint slice_num;
UBYTE *BlackImage;
UWORD Imagesize = ((EPD_4IN2_V2_WIDTH % 8 == 0)? (EPD_4IN2_V2_WIDTH / 8 ): (EPD_4IN2_V2_WIDTH / 8 + 1)) * EPD_4IN2_V2_HEIGHT;

void init()
{
    gpio_set_function(LED_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LED_PIN);
    pwm_set_wrap(slice_num, 255);
    pwm_set_enabled(slice_num, true);

    adc_init();

    // 2. Configure ADC Pins (GPIO 26 and 27)
    // Make sure there are no pull-ups/downs on these pins
    adc_gpio_init(ADC_X);
    adc_gpio_init(ADC_Y);

    // 3. Configure the Digital Switch (GPIO 22)
    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    // Most joysticks need a pull-up resistor on the switch
    gpio_pull_up(SW_PIN);

    // EPAPER INIT
    
    if(DEV_Module_Init()!=0)
    {
        printf("fail");
        return;
    }

    EPD_4IN2_V2_Init();
    EPD_4IN2_V2_Clear();    
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return;
    }
    Paint_NewImage(BlackImage, EPD_4IN2_V2_WIDTH, EPD_4IN2_V2_HEIGHT, 0, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    EPD_4IN2_V2_Display(BlackImage);
    DEV_Delay_ms(2000);
};

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
    bool z_pressed = !gpio_get(SW_PIN);
    return stickInput{
        x_axis: abs(x_norm) < xDeadZone ? 0.0f : ((signbit(x_norm)) ? -1.0f : 1.0f),
        y_axis: abs(y_norm) < yDeadZone ? 0.0f : ((signbit(y_norm)) ? -1.0f : 1.0f),
        pressed: z_pressed
    };
}

const int midX = EPD_4IN2_V2_WIDTH / 2;
const int midY = EPD_4IN2_V2_HEIGHT / 2;
const int lineWidth = 3;
const int halfSize = 60;
const int inputTextX = midX - halfSize + lineWidth + 1;
const int inputTextY = midY;
const int textWidth = halfSize * 2 - lineWidth * 4;
const int textHeight = 13;
const int inputTextXEnd = inputTextX + textWidth;
const int inputTextYEnd = inputTextY + textHeight;

void Render()
{
    EPD_4IN2_V2_Init();
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    printf("Drawing:BlackImage\r\n");
    Paint_DrawRectangle(midX - halfSize, midY - halfSize / 2, midX + halfSize, midY + halfSize / 2, BLACK, DOT_PIXEL_2X2, DRAW_FILL_EMPTY);
    Paint_DrawString_EN(inputTextX, midY - halfSize / 2, "BEN OS", &Font16, WHITE, BLACK);
    Paint_DrawString_EN(inputTextX, inputTextY, "INPUT", &Font12, WHITE, BLACK);

	EPD_4IN2_V2_Display(BlackImage);
}
void RenderUpdate(const char* text)
{
    Paint_Clear(WHITE);
    
    Paint_DrawRectangle(1, 1, textWidth, textHeight, WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawString_EN(1, 1, text, &Font12, WHITE, BLACK);

    EPD_4IN2_V2_PartialDisplay(BlackImage, inputTextX, inputTextY, inputTextXEnd, inputTextYEnd);
}

int main()
{ 
    init();

    Render();
    
	//free(BlackImage);
    DEV_Delay_ms(500);
    //DEV_Module_Exit();
    
    stickInput lastInput = GetInput();

    Paint_NewImage(BlackImage, textWidth, textHeight, 0, BLACK);

    while (true) {
        stickInput input = GetInput();
        if (!lastInput.pressed && input.pressed)
        {
            pwm_set_chan_level(slice_num, PWM_CHAN_A, 255);
            RenderUpdate("POW");
        }
        else
        {
            pwm_set_chan_level(slice_num, PWM_CHAN_A,  MIN(255,(int)(255 * (abs(input.x_axis) + abs(input.y_axis)))));
            if (input.x_axis < -0.25f)
            {
                RenderUpdate("LEFT");
            }
            else if (input.x_axis > 0.25f)
            {
                RenderUpdate("RIGHT");
            }
            else if (input.y_axis > 0.25f)
            {
                RenderUpdate("UP");
            }
            else if (input.y_axis < -0.25f)
            {
                RenderUpdate("DOWN");
            }
        }
        
        lastInput = input;
        sleep_ms(100);
    }
}
