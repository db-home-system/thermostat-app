import time
import os


ROOT_SET_PATH="../thermoapp/setting/"
ROOT_RUN_PATH="../thermoapp/run/"

run_files = [
    "mode.in",
    "pid.out",
    "status.out",
    "clock.in"
]

def cleanUp():
    for i in run_files:
        if os.path.exists(os.path.join(ROOT_RUN_PATH, i)):
            os.remove(os.path.join(ROOT_RUN_PATH, i))


def timeline(l):
    f = open("../settings/timeline" , 'w')
    for i in l:
        f.write(i + "\n")
    f.close()

def setMode(t):
    f = open(ROOT_RUN_PATH + "mode.in", 'w')
    f.write(t + "\n")
    f.close()

def timeClock(t):
    f = open(ROOT_RUN_PATH + "clock.in", 'w')
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


