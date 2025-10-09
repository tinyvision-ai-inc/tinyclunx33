# Copyright (c) 2024 tinyVision.ai Inc.
#
# SPDX-License-Identifier: Apache-2.0

'''
Runner for the ecpprog programming tool for Lattice FPGAs.

https://github.com/gregdavill/ecpprog

This version with extra mpremote before and after to reboot the board
or perform other actions.

A license-related 'protection feature' of some Lattice devices.
'''

import os

from runners.core import BuildConfiguration, RunnerCaps, ZephyrBinaryRunner


class EcpprogHookBinaryRunner(ZephyrBinaryRunner):
    '''Runner front-end for programming the FPGA flash at some offset.'''

    def __init__(self, cfg, host=None, script=[], ecpprog=None, mpremote=None, device=None,
                 port=None):
        super().__init__(cfg)
        self.host = host
        self.ecpprog = ecpprog
        self.device = device
        self.mpremote = mpremote
        self.port = port
        self.script = script

    @classmethod
    def name(cls):
        return 'ecpprog_mpremote'

    @classmethod
    def capabilities(cls):
        return RunnerCaps(commands={'flash'})

    @classmethod
    def do_add_parser(cls, parser):
        parser.add_argument(
            '--host', dest='host',
            help='SSH host to connect to (optional)'
        )
        parser.add_argument(
            '--mpremote', dest='mpremote', default='mpremote',
            help='Path to the mpremote binary on the remote host'
        )
        parser.add_argument(
            '--mpremote-port', dest='port',
            help='Device to connect to using ecpprog -d flag (optional)'
        )
        parser.add_argument(
            '--mpremote-script', dest='script', action='append', required=True,
            help='Script to load with mpremote. Set multiple times to run several scripts.'
        )
        parser.add_argument(
            '--ecpprog', dest='ecpprog', default='ecpprog',
            help='Path to the ecpprog binary on the remote host'
        )
        parser.add_argument(
            '--ecpprog-device', dest='device',
            help='Device to connect to using ecpprog -d flag (optional)'
        )

    @classmethod
    def do_create(cls, cfg, args):
        return EcpprogHookBinaryRunner(cfg, host=args.host, script=args.script,
                                       mpremote=args.mpremote, ecpprog=args.ecpprog,
                                       device=args.device, port=args.port)

    def do_run_mpremote(self, prefix):
        command = list(prefix)

        command.append(self.mpremote)

        if self.port is not None:
            command.extend(('connect', self.port))

        command.extend(('run', '/dev/stdin'))

        for script in self.script:
            self.logger.info(' '.join(command) + ' <' + script)
            fd = os.open(script , os.O_RDONLY)
            os.dup2(fd, 0)
            self.check_call(command)

    def do_run_ecpprog(self, prefix):
        command = list(prefix)
        build_conf = BuildConfiguration(self.cfg.build_dir)
        load_offset = build_conf.get('CONFIG_FLASH_LOAD_OFFSET', 0)
        command.extend((self.ecpprog, '-o', hex(load_offset)))

        if self.device is not None:
            command.extend(('-d', self.device))

        command.append('-')

        self.logger.info(' '.join(command) + ' <' + self.cfg.bin_file)
        fd = os.open(self.cfg.bin_file , os.O_RDONLY)
        os.dup2(fd, 0)
        self.check_call(command)

    def do_run(self, command, **kwargs):
        if self.host is not None:
            prefix = ('ssh', self.host)
        else:
            prefix = ()

        self.do_run_mpremote(prefix)
        self.do_run_ecpprog(prefix)
        self.do_run_mpremote(prefix)
