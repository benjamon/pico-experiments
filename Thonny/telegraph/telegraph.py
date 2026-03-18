from lcd1602 import LCD
from machine import Pin
import time

class fpm:
    def __init__(self, pin_number):
            self.pin = machine.Pin(pin_number, machine.Pin.IN, machine.Pin.PULL_UP)
            self._state = self.pin.value()
            self.pin.irq(handler=self._handle_trigger, trigger=machine.Pin.IRQ_FALLING | machine.Pin.IRQ_RISING)

    # Compiles this function to native ARM assembly
    @micropython.native
    def _handle_trigger(self, pin):
        self._state = pin.value()
        
    def value(self):
        """Returns the most recent state captured by the interrupt."""
        return self._state

lcd = LCD()
btn = fpm(18)
led = Pin(16, Pin.OUT)

text = '> '
current = ''
full = ''
isOn = False
reset = False
start_time = 0
stop_time = -1

DIT_TIME = 150
RESET_TIME = 2000
LETTER_TIME = 400

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
    if not btn.value():
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
            if time.ticks_ms() - start_time < DIT_TIME:
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
            
