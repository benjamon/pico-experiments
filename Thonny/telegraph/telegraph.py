from lcd1602 import LCD
from machine import ADC, Pin
import time

lcd = LCD()
btn = ADC(Pin(27))
led = Pin(16, Pin.OUT)

text = '> '
current = ''
full = ''
isOn = False
reset = False
start_time = 0
stop_time = -1

IGNORE_TIME = 30
DIT_TIME = 300
RESET_TIME = 2000
LETTER_TIME = 1000

letters = {
        '.-': 'A',
        '-...': 'B',
        '-.-.': 'C',
        '-..': 'D',
        '.': 'E',
        '..-.': 'F',
        '--.': 'G',
        '....': 'H',
        '..': 'I',
        '.---': 'J',
        '-.-': 'K',
        '.-..': 'L',
        '--': 'M',
        '-.': 'N',
        '---': 'O',
        '.--.': 'P',
        '--.-': 'Q',
        '.-.': 'R',
        '...': 'S',
        '-': 'T',
        '..-': 'U',
        '...-': 'V',
        '.--': 'W',
        '-..-': 'X',
        '-.--': 'Y',
        '--..': 'Z',
        '.....': ' '
    }

def update_text():
    lcd.write(0,0, full + " " + current)
    lcd.write(0,1, text)

while True:
    btnPressed = False
    volt = btn.read_u16()
    if volt > 1000:
        btnPressed = True
    if not btnPressed:
        led.value(1)
        if not isOn:
            isOn = True
            start_time = time.ticks_ms()
            text = '>>    '
            lcd.write(0,1, text)
        elif time.ticks_ms() - start_time > RESET_TIME:
            text = '>       '
            current = ''
            full = ''
            lcd.write(0,0, '                ')
            lcd.write(0,1, '                ')
            start_time = time.ticks_ms()
            isOn = False
            reset = True
            update_text()
        elif time.ticks_ms() - start_time > DIT_TIME:
            text = 'DAH -'
            update_text()
    else:
        led.value(0)
        if isOn:
            isOn = False
            stop_time = time.ticks_ms()
            duration = time.ticks_ms() - start_time
            if duration < IGNORE_TIME:
                text = ">    "
                update_text()
            elif duration < DIT_TIME:
                text = 'DIT *'
                current += '.'
                update_text()
            else:
                text = 'DAH -'
                current += '-'
                update_text()
        elif time.ticks_ms() - stop_time > LETTER_TIME:
            if reset:
                reset = False
                full = ''
            else:
                full += letters.get(current, '')
            current = ''
            text = '>        '
            lcd.write(0,0,full + '        ')
            update_text()
        time.sleep_ms(10)
            
