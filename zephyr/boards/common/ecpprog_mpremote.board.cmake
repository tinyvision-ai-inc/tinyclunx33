# SPDX-License-Identifier: Apache-2.0

board_set_flasher_ifnset(ecpprog_mpremote)

board_finalize_runner_args(ecpprog_mpremote
  "--mpremote-script=${ZEPHYR_TINYCLUNX33_MODULE_DIR}/scripts/mpremote/full_reset.py")
