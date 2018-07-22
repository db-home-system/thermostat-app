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

DEV_TEMP="/Users/asterix/src/dbhome-app/thermoapp/input/temp"
SENS_DATA="/Users/asterix/src/dbhome-app/thermoapp/input/sens"

def cleanUp():
    cleanRun()
    cleanTimeline()
    cleanData()

def cleanRun():
    for i in run_files:
        if os.path.exists(os.path.join(ROOT_RUN_PATH, i)):
            os.remove(os.path.join(ROOT_RUN_PATH, i))
def cleanPid():
    if os.path.exists(os.path.join(ROOT_RUN_PATH, "pid.out")):
        os.remove(os.path.join(ROOT_RUN_PATH, "pid.out"))

def cleanTimeline():
    for i in ['timeline.in']:
        if os.path.exists(os.path.join(ROOT_SET_PATH, i)):
            os.remove(os.path.join(ROOT_SET_PATH, i))

def cleanData():
    for i in [DEV_TEMP, SENS_DATA]:
        if os.path.exists(i):
            os.remove(i)


def timeline(l):
    f = open("../thermoapp/setting/timeline.in" , 'w')
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
    f = open(DEV_TEMP, 'w')
    f.write(t + "\n")
    f.close()

def sensTemp(sens):
    f = open(SENS_DATA, 'w')
    for i in sens:
        f.write(i + "\n")
    f.close()


def cfgTick(cfg):
    with open(cfg, r) as f:
        pid_tick = 250
        clock_tick = 1000
        for line in f:
            if "pid_tick" in line:
                pid_tick = int(line.split("=")[1].strip())
            if "clock_tick" in line:
                clock_tick = int(line.split("=")[1].strip())

    return (pid_tick, clock_tick)


def readPID():
    with open(os.path.join(ROOT_RUN_PATH, "pid.out"), 'r') as p:
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


