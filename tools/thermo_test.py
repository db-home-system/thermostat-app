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

TICK = 1 # second

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
            time.sleep(1.2)

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
        time.sleep(1)

        timeClock("06:00:00")

        l = [
            "5;1;18000",
            "6;1;25000",
            "7;1;20000",
        ]
        timeline(l)

        devTemp("20.13")

        sens = [
            "0;intTemp;23000;casa",
            "1;extTemp;19130;finestra",
            "2;intTemp;27000;finestra",
        ]
        sensTemp(sens)

        time.sleep(2.2)

        #h;devTemp;intTemp;extTemp;Sp;Pt;
        check = "6;20.13;25.67;19.13;25;22.9;0;\n"
        pid = open("../output/pid.log", 'r').readlines()
        print
        for i in pid:
            if "#" in i:
                continue
            self.assertEqual(i,check)

    def test_pidOnOff(self):

        cleanUp()
        time.sleep(1)

        timeClock("00:00:00")

        l = [
            "5;1;18000",
            "6;1;25000",
            "7;1;20000",
        ]
        timeline(l)

        devTemp("20000")
        time.sleep(TICK * 2)


        #h;devTemp;intTemp;extTemp;Sp;Pt;
        check = [
            "6;20.13;25.67;19.13;25;22.9;1;\n",
        ]

        timeClock("05:00:00")
        print
        for i in ["18000", "18300", "18500"]:
            devTemp(i)
            print readPID()
            time.sleep(TICK)


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
    suite.addTest(TestThermo("test_timeline"))
    #suite.addTest(TestThermo("test_pid"))
    #suite.addTest(TestThermo("test_pidOnOff"))
    unittest.TextTestRunner(
        stream=sys.stdout,
        verbosity=options.verbose).run(suite)
