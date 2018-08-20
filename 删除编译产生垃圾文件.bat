copy Debug\Exe\ExecutableFile.hex ExecutableFile.hex
copy Release\Exe\ExecutableFile.hex ExecutableFile.hex

del /Q Debug\Exe\*.*
del /Q Debug\List\*.*
del /Q Debug\Obj\*.*

del /Q Release\Exe\*.*
del /Q Release\List\*.*
del /Q Release\Obj\*.*

del /Q settings\*.*

del /Q *.dep
del /Q *.ewt

exit