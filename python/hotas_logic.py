
import pygame

DEADZONE = 0.05


def init_joystick():
    pygame.init()
    pygame.joystick.init()

    if pygame.joystick.get_count() == 0:
        raise RuntimeError("No joystick found")

    joystick = pygame.joystick.Joystick(0)
    print("Connected:", joystick.get_name())
    return joystick


def read_state(joystick):
    pygame.event.pump()

    # Analog axes
    axes = []
    for i in range(joystick.get_numaxes()):
        value = joystick.get_axis(i)

        if abs(value) < DEADZONE:
            value = 0.0

        axes.append(round(value, 3))

    # Regular buttons: B0-B5
    buttons = {
        i: bool(joystick.get_button(i))
        for i in range(6)
    }

    # Hat 0: Actual POV hat
    hat0 = (
        joystick.get_hat(0)
        if joystick.get_numhats() > 0
        else (0, 0)
    )

    # Hat 1: Buttons 6, 7, 8, 9
    hat1 = (
        int(joystick.get_button(7))
        - int(joystick.get_button(9)),
        int(joystick.get_button(6))
        - int(joystick.get_button(8))
    )

    # Hat 2: Buttons 10, 11, 12, 13
    hat2 = (
        int(joystick.get_button(11))
        - int(joystick.get_button(13)),
        int(joystick.get_button(10))
        - int(joystick.get_button(12))
    )

    return {
        "axes": axes,
        "buttons": buttons,
        "hats": [hat0, hat1, hat2]
    }


def close_joystick():
    pygame.quit()
