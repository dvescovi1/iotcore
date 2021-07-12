:: Copyright (c) Microsoft Corporation. All rights reserved.
:: Licensed under the MIT License.

set BATCH_HOME=%~dp0
call ..\..\..\tools\SetupBuildEnv.bat


if "%PLATFORM%"=="ARM" (

    REM ARM32 based Platforms

    cd /d %BATCH_HOME%
    echo "Building EVK_iMX6UL_512MB FFU"
    call BuildImage EVK_iMX6UL_512MB EVK_iMX6UL_512MB_TestOEMInput.xml
)

if "%PLATFORM%"=="ARM64" (

    REM ARM64 based Platforms

    REM cd /d %BATCH_HOME%
    REM echo "Building NXPEVK_iMX8M_4GB FFU"
    REM call BuildImage NXPEVK_iMX8M_4GB NXPEVK_iMX8M_4GB_TestOEMInput.xml

    cd /d %BATCH_HOME%
    echo "Building RPI4_RPI64_4GB FFU"
    call BuildImage RPI4_RPI64_4GB RPI4_RPI64_4GB_TestOEMInput.xml
)
