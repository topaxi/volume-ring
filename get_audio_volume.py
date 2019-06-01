#!/usr/bin/env python

import os
from time import sleep
import select
import alsaaudio
import serial

SERIAL_DEVICE = os.environ.get('ARDUINO_SERIAL_PORT', '/dev/ttyUSB0')
AUDIO_DEVICE = os.environ.get('AUDIO_DEVICE', 'default')

def get_mixer():
    return alsaaudio.Mixer(device=AUDIO_DEVICE)

def clamp(n, lo, hi):
    return max(lo, min(n, hi))

def get_volume_as_byte():
    return clamp(
        int(get_mixer().getvolume()[0] / 100.0 * 255),
        0,
        255
    )


def setup_poll():
    mixer = get_mixer()
    poll = select.poll()
    (fd, m) = mixer.polldescriptors()[0]
    poll.register(fd, m)
    return (poll, mixer)

def wait_for_mixer_change((poll, mixer) = setup_poll()):
    poll.poll()
    mixer.handleevents()

def sleep_ms(ms):
    sleep(1 / 1000 * ms)

with serial.Serial(SERIAL_DEVICE, 9600, timeout=5) as ser:
    current_volume = get_volume_as_byte()

    sleep(2)

    while True:
        wait_for_mixer_change()

        volume = get_volume_as_byte()

        if volume != current_volume:
            current_colume = volume
            ser.write(bytearray([volume]))

        sleep_ms(32)
