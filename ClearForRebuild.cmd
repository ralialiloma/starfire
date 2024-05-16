@echo off

set "targetDirectory=Project"

echo Deleting folders named Intermediate and Binary in %targetDirectory%.

for /d %%d IN ("%targetDirectory%\Intermediate") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for /d %%d IN ("%targetDirectory%\DerivedDataCache") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for /d %%d IN ("%targetDirectory%\Binaries") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for /d %%d IN ("%targetDirectory%\.vs") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for /d %%d IN ("%targetDirectory%\.vscode") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for /d %%d IN ("%targetDirectory%\.idea") do (
    if exist "%%d" (
        echo Removing folder: %%d
        rd /s /q "%%d"
    )
)

for %%f IN ("%targetDirectory%\Project.sln") do (
    if exist "%%f" (
        echo Deleting file: %%f
        del /f /q "%%f"
    )
)

echo Done.

::set /p "recompileChoice=Would you like to recompile the Unreal project? (Y/N): "
::if /i "%recompileChoice%"=="Y" (
::    echo Recompiling the project...
::    REM Add your recompilation command here
::) else (
::    echo Project not recompiled.
::)

pause