import evdev
import sys
import threading
import time

counter = 0


def listen(dev):
    global counter
    last_usec = 0
    for event in dev.read_loop():
        if event.usec != last_usec:
            last_usec = event.usec
            counter += 1


def wait():
    global counter
    while True:
        start = counter
        time.sleep(1)
        stop = counter
        print(stop - start)


dev = evdev.InputDevice(sys.argv[1])
listener = threading.Thread(target=listen, args=[dev]).start()
waiter = threading.Thread(target=wait).start()
