'''
    Test runner main module.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

from __future__ import absolute_import, print_function
import argparse
import os
import subprocess
import sys
import timeit


def parse_args():
    '''
    Parse arguments.
    Forward the return from argparse.ArgumentParser().parse_args().
    '''

    '''
    default_testdir = os.path.abspath(
        os.path.join(os.path.dirname(__file__), '..', 't'))
    '''

    parser = argparse.ArgumentParser()
    parser.add_argument('-t', '--test_dir', default='t', type=str,
                        help='the directory containing tests')
    parser.add_argument('-l', '--log', default=sys.stdout,
                        type=argparse.FileType('w'),
                        help='the file where the output should be written')
    return parser.parse_args()


def _main(args):
    '''
    Main method.
    '''

    tests = []

    try:
        for f in os.listdir(args.test_dir):
            if '.' in f:
                continue
            tests += [os.path.abspath(os.path.join(args.test_dir, f))]
    except:
        pass

    if 0 == len(tests):
        args.log.write('make test\n')
        return 1

    msg = '1..{}\n'.format(len(tests))
    args.log.write(msg)

    test_nr = 1
    for path in tests:
        got = '{}.got'.format(path)
        expected = '{}.expected'.format(path)

        command = 'rm -f {}'.format(got)
        result = subprocess.check_output(command, shell=True)

        command = '{} > {}'.format(path, got)
        try:
            result = subprocess.check_output(command, shell=True)
        except:
            pass

        command = 'diff -up {} {}'.format(expected, got)
        try:
            subprocess.check_output(command, shell=True)
        except Exception as e:
            args.log.write(
                'not ok ' + str(test_nr) + ' - ' + os.path.basename(path) + '\n')
            args.log.write(bytes.decode(e.output))
            break

        args.log.write(
            'ok ' + str(test_nr) + ' - ' + os.path.basename(path) + '\n')

        test_nr += 1

    return 0

if __name__ == '__main__':
    args = parse_args()
    start_time = timeit.default_timer()
    ret = _main(args)
    if ret == 0:
        args.log.write('Took {:0.2f} ms\n'.format(
            1000 * (timeit.default_timer() - start_time)))
    args.log.close()
    exit(ret)
