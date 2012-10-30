@echo Copying translations...
copy statistiq_ru.qm ..\statistiq-build-debug\
copy statistiq_ru.qm ..\statistiq-build-release\
copy statistiq_ru.qm ..\statistiq-redist\
copy qt_ru.qm ..\statistiq-build-debug\
copy qt_ru.qm ..\statistiq-build-release\
copy qt_ru.qm ..\statistiq-redist\

@echo Copying executable...
copy ..\statistiq-build-release\release\statistiq.exe ..\statistiq-redist\

@pause
