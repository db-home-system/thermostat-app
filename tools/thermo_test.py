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
        for i in range(24):
            s = "%0.2d:00:00" % i
            f = open("/tmp/sim/time", 'w')
            f.write(s + "\n")
            f.close()
            print s
            time.sleep(1.2)

    def test_timeline(self):
        filename = "../settings/timeline" 
        f = open(filename, 'w')
        for i in range(24):
            s = "%d;%d;0.0" % (i, 1)
            f.write(s + "\n")
        f.close()

        time.sleep(1)

        f = open(filename, 'w')
        for i in range(24):
            s = "%d;%d;0.0" % (i, 0)
            f.write(s + "\n")
        f.close()

        time.sleep(1)

        f = open(filename, 'w')
        flag = True
        for i in range(24):
            s = "%d;%d;0.0" % (i, flag)
            f.write(s + "\n")
            flag = not flag
        f.close()

    def test_pid(self):
        f = open("/tmp/sim/time", 'w')
        f.write("06:00:00\n")
        f.close()

        f = open("../settings/timeline", 'w')
        f.write("5;1;18.0\n")
        f.write("6;1;25.0\n")
        f.write("7;1;20.0\n")
        f.close()


        dev_temp = open("../input/device_temp", 'w')
        dev_temp.write("20.13\n")
        dev_temp.close()

        sens = [
            "0;intTemp;23.0;casa",
            "1;extTemp;19.13;finestra",
            "2;intTemp;27.0;finestra",
        ]
        sens_temp = open("../input/sensor_data", 'w')
        for i in sens:
            sens_temp.write(i + "\n")
        sens_temp.close()

        #h;devTemp;intTemp;extTemp;Sp;Pt;
        check = "6;20.13;25.67;19.13;25;22.9;0;\n"
        pid = open("../output/pid.log", 'r')
        print
        for i in pid:
            if "#" in i:
                continue
            self.assertEqual(i,check)



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
    suite.addTest(TestThermo("test_pid"))
    unittest.TextTestRunner(
        stream=sys.stdout,
        verbosity=options.verbose).run(suite)
