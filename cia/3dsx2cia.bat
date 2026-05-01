cxitool.exe -t 000400000FB6E000 -b banner.bnr -c CTR-H-FBGE ../fbge-installer.3dsx youCanSafelyDeleteThis.cxi
makerom.exe -f cia -o ../build/FBGE-INSTALLER.CIA -target t -i youCanSafelyDeleteThis.cxi:0:0 -ignoresign & REM Ignore CXI signature
del youCanSaeflyDeleteThis.cxi