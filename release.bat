@echo Copying translations...
copy processdata_ru.qm ..\statistiq-build-debug\
copy processdata_ru.qm ..\statistiq-build-release\
copy processdata_ru.qm ..\processdata-redist\
copy qt_ru.qm ..\statistiq-build-debug\
copy qt_ru.qm ..\statistiq-build-release\
copy qt_ru.qm ..\processdata-redist\

@echo Copying executable...
copy ..\statistiq-build-release\release\processdata.exe ..\processdata-redist\

@echo Copying readme...
copy README.md ..\processdata-redist\README-en.md
copy README-ru.md ..\processdata-redist\README.txt

@pause
