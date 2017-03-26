# Power Monitor #

Power Monitor is to measure power consumption of har disks. It requires Hard Disk Power Monitor used INA 226, and six serial ports.

## MUST DO IT ##

You must modify your account to be add to the `dialout` group.

```
#!shell
$ sudo usermod -a -G dialout MY_USER_NAME
```

NOTE: sometimes you should reboot.
