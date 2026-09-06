project(FIO)

include(FetchContent)

set(FIO_GIT     "https://github.com/LeoTHPS/FIO.git")
set(FIO_GIT_TAG "origin/main")
FetchContent_Declare(fio_external GIT_REPOSITORY ${FIO_GIT} GIT_TAG ${FIO_GIT_TAG} EXCLUDE_FROM_ALL)
FetchContent_MakeAvailable(fio_external)
include(${fio_external_SOURCE_DIR}/FIO.cmake)
