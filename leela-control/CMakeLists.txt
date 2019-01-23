# CMakeList.txt: LeelaControl CMake project
#
#
if(${WIN_ENV})
    INCLUDE_DIRECTORIES(
        "C:/Qt/Qt5.7.1(32)/5.7/msvc2015/include"
        "D:/Develop_Code/Development/PabulumElement"
    )

    LINK_DIRECTORIES(
        "C:/Qt/Qt5.7.1(32)/5.7/msvc2015/lib"
        "D:/Develop_Code/Development/PabulumElement/boost/boost_win32"
    )
elseif(${ARM_LINUX})
    # required packages
    set(Boost_MIN_VERSION "1.58.0")
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost 1.58.0 REQUIRED program_options filesystem)

    find_package(Qt5Core REQUIRED)

    # include
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${Qt5Core_INCLUDE_DIRS})
    message("qt include path ${Qt5Core_INCLUDE_DIRS}")

endif()
set(IncludePath "${CMAKE_CURRENT_SOURCE_DIR}")

# add source to execute
set (SOURCES
     applications/AppInstance.cpp
     configurations/AppConfiguration.cpp
     games/GameLeela.cpp
     games/ManagementJob.cpp
     games/Job.cpp
     LeelaControl.cpp
)
set(INCLUDE
     LeelaControl.hpp
     ParseHelp.hpp
     games/ManagementJob.hpp
     games/GameLeela.hpp
     games/IJob.hpp
     games/Job.hpp
        sockets/ServiceSocket.hpp sockets/ISocket.hpp applications/ServiceContext.hpp)

set(INCLUDEPATH
    ${CMAKE_CURRENT_LIST_DIR}
)
include_directories(${INCLUDEPATH})

add_executable (${EXECUTABLE_NAME} ${SOURCES} ${INCLUDE})

if(${WIN_ENV})
    target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
            boost_program_options-vc140-mt-1_65
            boost_system-vc140-mt-1_65
            Qt5Core
    )
elseif(${ARM_LINUX})
    target_link_libraries(${EXECUTABLE_NAME}
            PRIVATE
            ${Boost_LIBRARIES}
            boost_thread
            ${Qt5Core_LIBRARIES}
    )
endif()
       


# TODO: 如有需要，请添加测试并安装目标。
