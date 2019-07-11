
from pynput.mouse import Button, Controller

mouse = Controller()
mouse.position = (10, 20)
mouse.click(Button.left, 1)
