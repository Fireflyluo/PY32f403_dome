#
# File      : sdk_dist.py
# This file is part of RT-Thread RTOS
# COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License along
#  with this program; if not, write to the Free Software Foundation, Inc.,
#  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Change Logs:
# Date           Author       Notes
# 2024-01-01     RT-Thread    The first version

import os
import shutil

import shutil

def dist_do_building(BSP_ROOT, dist_dir):
    """
    Default dist handle for SDK.
    This function can be overridden by BSP specific dist_handle.
    """
    # Copy shared HAL libraries for py32 BSPs
    bsp_name = os.path.basename(BSP_ROOT)
    bsp_parent = os.path.dirname(BSP_ROOT)
    
    # Copy PY32F403_HAL if it exists in parent directory
    hal_dir = os.path.join(bsp_parent, 'PY32F403_HAL')
    if os.path.exists(hal_dir):
        dst_hal_dir = os.path.join(dist_dir, 'PY32F403_HAL')
        if os.path.exists(dst_hal_dir):
            shutil.rmtree(dst_hal_dir)
        shutil.copytree(hal_dir, dst_hal_dir, ignore=shutil.ignore_patterns('*.pyc', '*.o', '*.d', 'build'))
        print('=> copy PY32F403_HAL')
    
    # Copy libraries if it exists in parent directory
    libs_dir = os.path.join(bsp_parent, 'libraries')
    if os.path.exists(libs_dir):
        dst_libs_dir = os.path.join(dist_dir, 'libraries')
        if os.path.exists(dst_libs_dir):
            shutil.rmtree(dst_libs_dir)
        shutil.copytree(libs_dir, dst_libs_dir, ignore=shutil.ignore_patterns('*.pyc', '*.o', '*.d', 'build'))
        print('=> copy libraries')
