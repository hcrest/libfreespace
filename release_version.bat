SET MAJOR_NUM=0
SET MINOR_NUM=6
SET REVISION_NUM=1
SET IS_RC=no

IF "%IS_RC%"=="yes" (
SET RELEASE_VERSION=%MAJOR_NUM%.%MINOR_NUM%rc%REVISION_NUM%
) ELSE (
SET RELEASE_VERSION=%MAJOR_NUM%.%MINOR_NUM%
)

REM Generate config-win32.h and the line below when the version changes.
echo #define VERSION "%RELEASE_VERSION%"> config-win32.h

