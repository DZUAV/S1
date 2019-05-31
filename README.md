# ArduPilot Project


/***********************************************************************************************************************
*文件说明：应用文件头文件
*文件功能：函数任务
*修改日期：2018-9-7
*修改作者：cihang_uav
*备注信息： 编译代码：./waf configure --board zuav
*备注信息： 下载代码：./waf --targets bin/arducopter --upload
*备注信息： 更新子模块：git submodule update --init --recursive
*备注信息： Code commit statistics can be found here: https://github.com/ArduPilot/ardupilot/graphs/contributors
          Wiki: http://copter.ardupilot.org/
          Requires modified version of Arduino, which can be found here: http://ardupilot.com/downloads/?category=6
配置秘钥：  git config --global user.name "Noneardupilot"
          git config --global user.email  "2551804348@qq.com"
          ssh-keygen -t rsa -C "2551804348@qq.com"
          
         2018-10-25:
上传ubuntu固件到github仓库：
          git init
          git add .
          git commit -m ""
          git remote rm origin
          git remote add origin https://github.com/Noneardupilot/ardupilot.git
          git push -u origin master  /git push -u origin +master 
仓库地址：  https://github.com/Noneardupilot/ardupilot
代码修改备注：

          2018-10-14:增加日本手和美国手的切换
          2018-10-15:修改实现植保AB点控制移植，其中采用八通道进行AB点打点操作，
          2018-10-16:修改内八上锁，外八解锁
          2018-10-18:仿真命令：sudo apt-get install build-essential---初始化编辑器
                             cd ArduCopter
                             ../Tools/autotest/sim_vehicle.py --console --map
                             output add 192.168.159.1:14550
          2018-10-24:修复自己的SITL仿真问题：
                             case RECOND_ZIGZAG:do_aux_function(ch_option, ch_flag);

          2018-10-25:增加U型控制函数
          2018-10-26:增加美国手，日本手切换，需要修改参数radio_mode=1，设置日本手，然后校准，radio_mode=2修改美国手
          2018-11-7:编译IO：./waf configure --board iomcu，代码：./waf iofirmware
          2018-11-21:增加IIC4做外部LED
          2019-1-7:上次.bin文件：git add -f (加上文件加目录)
          2019-1-29:
                    git init
                    git add README.md
                    git commit -m "first commit"
                    git remote rm origin
                    git remote add origin https://github.com/UAVcihang/Copter_3.6.5_master.git
                    git push -u origin +master 
          2019-1-30:添加歌尔气压计SPL06—001驱动
          2019-4-3:创建分支：git branch dev
                   再添加到暂存区： git add .
                   提交到版本库 git commit -m "分支的内容"
                   提交到远程仓库 git push origin dev
                   切换到master分支 git checkout master
                   把dev分支合并到master分支 git merge dev
                   提交到远程仓库 git push -u origin master
                   
*************************************************************************************************************************/




[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ArduPilot/ardupilot?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![Build SemaphoreCI](https://semaphoreci.com/api/v1/ardupilot/ardupilot/branches/master/badge.svg)](https://semaphoreci.com/ardupilot/ardupilot)

[![Build Travis](https://travis-ci.org/ArduPilot/ardupilot.svg?branch=master)](https://travis-ci.org/ArduPilot/ardupilot)

[![Coverity Scan Build Status](https://scan.coverity.com/projects/5331/badge.svg)](https://scan.coverity.com/projects/ardupilot-ardupilot)

## The ArduPilot project is made up of: ##

- ArduCopter (or APM:Copter) : [code](https://github.com/ArduPilot/ardupilot/tree/master/ArduCopter), [wiki](http://ardupilot.org/copter/index.html)

- ArduPlane (or APM:Plane) : [code](https://github.com/ArduPilot/ardupilot/tree/master/ArduPlane), [wiki](http://ardupilot.org/plane/index.html)

- ArduRover (or APMrover2) : [code](https://github.com/ArduPilot/ardupilot/tree/master/APMrover2), [wiki](http://ardupilot.org/rover/index.html)

- ArduSub (or APM:Sub) : [code](https://github.com/ArduPilot/ardupilot/tree/master/ArduSub), [wiki](http://ardusub.com/)

- Antenna Tracker : [code](https://github.com/ArduPilot/ardupilot/tree/master/AntennaTracker), [wiki](http://ardupilot.org/antennatracker/index.html)

## User Support & Discussion Forums ##

- Support Forum: <http://discuss.ardupilot.org/>

- Community Site: <http://ardupilot.org>

## Developer Information ##

- Github repository: <https://github.com/ArduPilot/ardupilot>

- Main developer wiki: <http://dev.ardupilot.org>

- Developer discussion: <http://discuss.ardupilot.org>

- Developer email group: drones-discuss@googlegroups.com. Deprecated November 2016. Included for historical reference.

## Contributors ##

- [Github statistics](https://github.com/ArduPilot/ardupilot/graphs/contributors)

## How To Get Involved ##

- The ArduPilot project is open source and we encourage participation and code contributions: [guidelines for contributors to the ardupilot codebase](http://dev.ardupilot.org/wiki/guidelines-for-contributors-to-the-apm-codebase)

- We have an active group of Beta Testers especially for ArduCopter to help us find bugs: [release procedures](http://dev.ardupilot.org/wiki/release-procedures)

- Desired Enhancements and Bugs can be posted to the [issues list](https://github.com/ArduPilot/ardupilot/issues).

- Helping other users with log analysis on [http://discuss.ardupilot.org/](http://discuss.ardupilot.org/) is always appreciated:

- There is a group of wiki editors as well in case documentation is your thing: <ardu-wiki-editors@googlegroups.com>

- Developer discussions occur on <drones-discuss@google-groups.com>

## License ##

The ArduPilot project is licensed under the GNU General Public
License, version 3.

- [Overview of license](http://dev.ardupilot.com/wiki/license-gplv3)

- [Full Text](https://github.com/ArduPilot/ardupilot/blob/master/COPYING.txt)

## Maintainers ##

Ardupilot is comprised of several parts, vehicles and boards. The list below
contains the people that regularly contribute to the project and are responsible
for reviewing patches on their specific area. See [CONTRIBUTING.md](.github/CONTRIBUTING.md) for more information.

- [Andrew Tridgell](https://github.com/tridge):
  - ***Vehicle***: Plane, AntennaTracker
  - ***Board***: APM1, APM2, Pixhawk, Pixhawk2, PixRacer
- [Randy Mackay](https://github.com/rmackay9):
  - ***Vehicle***: Copter, AntennaTracker
- [Grant Morphett](https://github.com/gmorph):
  - ***Vehicle***: Rover
- [Tom Pittenger](https://github.com/magicrub):
  - ***Vehicle***: Plane
- [Chris Olson](https://github.com/ChristopherOlson) and [Bill Geyer](https://github.com/bnsgeyer):
  - ***Vehicle***: TradHeli
- [Paul Riseborough](https://github.com/priseborough):
  - ***Subsystem***: AP_NavEKF2
  - ***Subsystem***: AP_NavEKF3
- [Lucas De Marchi](https://github.com/lucasdemarchi):
  - ***Subsystem***: Linux
- [Peter Barker](https://github.com/peterbarker):
  - ***Subsystem***: DataFlash
  - ***Subsystem***: Tools
- [Michael du Breuil](https://github.com/WickedShell):
  - ***Subsystem***: SMBus Batteries
  - ***Subsystem***: GPS
- [Francisco Ferreira](https://github.com/oxinarf):
  - ***Bug Master***
- [Matthias Badaire](https://github.com/badzz):
  - ***Subsystem***: FRSky
- [Eugene Shamaev](https://github.com/EShamaev):
  - ***Subsystem***: CAN bus
  - ***Subsystem***: UAVCAN
- [Víctor Mayoral Vilches](https://github.com/vmayoral):
  - ***Board***: PXF, Erle-Brain 2, PXFmini
- [Mirko Denecke](https://github.com/mirkix):
  - ***Board***: BBBmini, BeagleBone Blue, PocketPilot
- [Georgii Staroselskii](https://github.com/staroselskii):
  - ***Board***: NavIO
- [Emile Castelnuovo](https://github.com/emilecastelnuovo):
  - ***Board***: VRBrain
- [Julien BERAUD](https://github.com/jberaud):
  - ***Board***: Bebop & Bebop 2
- [Matt Lawrence](https://github.com/Pedals2Paddles):
  - ***Vehicle***: 3DR Solo & Solo based vehicles
- [Gustavo José de Sousa](https://github.com/guludo):
  - ***Subsystem***: Build system
- [Craig Elder](https://github.com/CraigElder):
  - ***Administration***: ArduPilot Technical Community Manager
- [Jacob Walser](https://github.com/jaxxzer):
  - ***Vehicle***: Sub
