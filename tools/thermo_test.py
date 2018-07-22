#! /usr/bin/env python
# -*- coding: utf-8 -*-
#
# MergeBom is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#
# Copyright 2018 Daniele Basile <asterix24@gmail.com>
#

import sys
import os
import time
import unittest
from thermo_sim import *

CLOCK_TICK=1 # Sec

class TestThermo(unittest.TestCase):
    """
    Thermo test suite
    """
    def __init__(self, testname):
        super(TestThermo, self).__init__(testname)
        pass

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_timelineMark(self):
        setMode("test")
        for i in range(24):
            timeClock("%0.2d:00:00" % i)
            print i
            time.sleep(CLOCK_TICK * 1.1)

    def test_timeline(self):

        def pattern(st):
            flag = st
            ll = []
            for i in range(24):
                ll.append("%d;%d;0" % (i, flag))
                flag = not flag
            return ll

        l = [
            [ "%d;1;0" % i for i in range(24) ],
            [ "%d;0;0" % i for i in range(24) ],
            pattern(True),
            pattern(False),
        ]

        cleanUp()
        setMode("test")
        time.sleep(1)

        for x in l:
            timeline(x)
            time.sleep(1)

    def test_pid(self):

        cleanUp()
        setMode("test")

        timeClock("06:00:00")

        l = [
            "5;1;18000",
            "6;1;25000",
            "7;1;20000",
            "8;0;15000",
        ]
        timeline(l)

        devTemp("20130")

        sens = [
            "0;intTemp;23000;casa",
            "1;extTemp;19130;finestra",
            "2;intTemp;27000;finestra",
        ]
        sensTemp(sens)

        time.sleep(1.2)

        #h;devTemp;intTemp;extTemp;Sp;Pt;status;
        check = ["6","20130","25000","19130","25000","22565","1",""]
        pid = readPID()
        print pid
        self.assertEqual(pid, check)

        timeClock("08:00:00")
        time.sleep(CLOCK_TICK * 1.1)

        #h;devTemp;intTemp;extTemp;Sp;Pt;status;
        check = ["8","20130","25000","19130","15000","22565","0",""]
        pid = readPID()
        print pid
        self.assertEqual(pid, check)

    def test_pidOnOff(self):

        DATA = [
            {
                "timeline": [
                    "5;0;18000",
                    "6;1;25000",
                    "7;1;20000",
                    "8;0;15000",
                    "22;1;22000",
                    "23;0;15000",
                ],

                "data": [
                    {
                        "clock": "05:00:00", "temp": "18000",
                        "sens": [ ],
                        "clean": [ ],
                        "check": ['5', '18000', '-273000', '-273000', '18000', '18000', '0', '']
                    }, {
                        "clock": "06:00:00", "temp": "24000",
                        "sens":  [ "0;intTemp;22000;casa", "2;intTemp;19000;finestra", ],
                        "clean": [ "0;intTemp;-273000;;", "2;intTemp;-273000;;", ],
                        "check": [ "6","24000","20500","-273000","25000","22250","1","", ]
                    }, {
                        "clock": "07:00:00", "temp": "20130",
                        "sens":  [ "1;extTemp;19130;f1 a", "2;extTemp;27000;f2 b", "5;extTemp;10000;f3 c"],
                        "clean": [ "1;extTemp;-273000;f1 a",
                                  "2;extTemp;-273000;f2 b", "5;extTemp;-273000;f3 c"],
                        "check": [
                            "7","20130","-273000","18710","20000","20130","0","", ]
                    }, {
                        "clock": "08:00:00", "temp": "20130",
                        "sens":  [ "0;intTemp;17000;casa", "1;extTemp;11000;finestra", "2;intTemp;18000;finestra", ],
                        "clean": [ "0;intTemp;-273000;casa",
                                  "1;extTemp;-273000;finestra",
                                  "2;intTemp;-273000;finestra", ],
                        "check": [
                            "8","20130","17500","11000","15000","18815","0","", ]
                    }, {
                        "clock": "22:00:00", "temp": "19000",
                        "sens":  [ "0;intTemp;17000;casa", "1;extTemp;20000;finestra", "2;intTemp;18000;finestra", ],
                        "clean": [ "0;intTemp;-273000;;",
                                   "1;extTemp;-273000;;",
                                   "2;intTemp;-273000;;", ],
                        "check": [
                            "22","19000","17500","20000","22000","18250","1","", ]
                    }, {
                        "clock": "23:00:00", "temp": "18000",
                        "sens":  ["0;intTemp; 0;casa",
                                  "2;intTemp;-15000;finestra",
                                  "1;extTemp;-10000;finestra",
                                  "7;extTemp;-10000;finestra", ],
                        "clean": ["0;intTemp;-273000;;",
                                  "2;intTemp;-273000;;",
                                  "1;extTemp;-273000;;",
                                  "7;extTemp;-273000;;", ],
                        "check": [
                            "23","18000","-7500","-10000","15000","5250","0","", ]
                    }
                    #h;devTemp;intTemp;extTemp;Sp;Pt;status;
                ]
            }
        ]

        def resetStatus():
            cleanData()
            cleanPid()
            devTemp("-27300")

        print
        for sim in DATA:
            cleanUp()
            setMode("test")
            timeline(sim["timeline"])

            for data in sim["data"]:
                resetStatus()
                timeClock(data["clock"])
                devTemp(data["temp"])
                sensTemp(data["sens"])
                print "Clk:%s; Temp:%s; Sens:%s;" % (data["clock"], data["temp"], data["sens"])
                sys.stdout.flush()
                time.sleep(CLOCK_TICK * 1.1)

                pid = readPID()
                sensTemp(data["clean"])
                time.sleep(CLOCK_TICK * 1.1)
                print pid
                self.assertEqual(pid, data["check"])

if __name__ == "__main__":
    from optparse import OptionParser

    parser = OptionParser()
    parser.add_option(
        "-v",
        "--verbose",
        dest="verbose",
        default='2',
        help="Output verbosity")
    (options, args) = parser.parse_args()
    print args

    suite = unittest.TestSuite()
    #suite.addTest(TestThermo("test_timelineMark"))
    #suite.addTest(TestThermo("test_timeline"))
    #suite.addTest(TestThermo("test_pid"))
    suite.addTest(TestThermo("test_pidOnOff"))
    unittest.TextTestRunner(
        stream=sys.stdout,
        verbosity=options.verbose).run(suite)
