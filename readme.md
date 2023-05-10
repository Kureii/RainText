# RainText

## Install
### Windows
- install msys2
- open msys2 console and run 

    ``` msys2
    pacman -S mingw-w64-x86_64-gtk4
    pacman -S mingw-w64-x86_64-toolchain base-devel
    pacman -S mingw-w64-x86_64-sqlite3
    ```
### Ubuntu
run:
  ``` bash
  sudo apt update
  sudo apt upgrade
  sudo apt install libgtk-4-1 libgtk-4-dev sqlite3 libsqlite3-dev
  ```

## Time
 - batabase hash: ~0.16s
 - key: ~5.7s

## Known issues
1. if you try delete item after close and open app, the app crash with stack overflow
