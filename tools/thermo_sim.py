import time
import os

def cleanUp():
    f = [
        "../settings/timeline",
        "/tmp/sim/time",
        "../input/device_temp",
        "../input/sensor_data",
        "../output/pid.log"
    ]

    for i in f:
        if os.path.exists(i):
            os.remove(i)


def timeline(l):
    f = open("../settings/timeline" , 'w')
    for i in l:
        f.write(i + "\n")
    f.close()

def timeClock(t):
    f = open("/tmp/sim/time", 'w')
    f.write(t + "\n")
    f.close()

def devTemp(t):
    f = open("../input/device_temp", 'w')
    f.write(t + "\n")
    f.close()

def sensTemp(sens):
    f = open("../input/sensor_data", 'w')
    for i in sens:
        f.write(i + "\n")
    f.close()

def readPID():
    with open("../output/pid.log", 'r') as p:
        for i in p:
            if "#" in i:
                continue
            l = i.strip()

    return l.split(";")

if __name__ == "__main__":

    for i in range(24):
        s = "%0.2d:00:00" % i
        f = open("/tmp/sim/time", 'w')
        f.write(s + "\n")
        f.close()
        print s
        time.sleep(0.2)


