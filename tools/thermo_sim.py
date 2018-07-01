import time


if __name__ == "__main__":

    for i in range(24):
        s = "%0d:00:00" % i
        f = open("/tmp/sim/time.cfg", 'w')
        f.write(s + "\n")
        f.close()
        print s
        time.sleep(0.2)


