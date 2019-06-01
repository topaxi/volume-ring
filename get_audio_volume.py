from time import sleep
import select
import alsaaudio
import serial

def clamp(n, lo, hi):
    return max(lo, min(n, hi))

def get_volume_as_byte():
    return clamp(
        int(alsaaudio.Mixer().getvolume()[0] / 100.0 * 255),
        0,
        255
    )


def setup_poll():
    mixer = alsaaudio.Mixer()
    poll = select.poll()
    (fd, m) = mixer.polldescriptors()[0]
    poll.register(fd, m)
    return (poll, mixer)

def wait_for_mixer_change((poll, mixer) = setup_poll()):
    poll.poll()
    mixer.handleevents()

def sleep_ms(ms):
    sleep(1 / 1000 * ms)

with serial.Serial('/dev/ttyUSB0', 9600, timeout=5) as ser:
    current_volume = get_volume_as_byte()

    sleep(2)

    while True:
        wait_for_mixer_change()

        volume = get_volume_as_byte()

        if volume != current_volume:
            current_colume = volume
            ser.write(bytearray([volume]))

        sleep_ms(32)
