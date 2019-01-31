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
    #find_package(pkgconfig)
    #find_package(libzmq REQUIRED)
    INCLUDE_DIRECTORIES(
            /home/junshen/tools/zeromq-4.3.1/arm_bin/include
            /home/junshen/tools/zmqHelp
    )
    LINK_DIRECTORIES(/home/junshen/tools/zeromq-4.3.1/arm_bin/lib)

    # include
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(${Qt5Core_INCLUDE_DIRS})
    message("qt include path ${Qt5Core_INCLUDE_DIRS}")

elseif(${GCC_LINUX})
    INCLUDE_DIRECTORIES(
        /ephemeral/junshen/development/tools/Qt5.7.1/5.7/gcc_64/include
        /ephemeral/junshen/development/tools/boost_1_65_1/gcc_bin/include
        /ephemeral/junshen/development/tools/zeromq-4.3.1/gcc/include
        /ephemeral/junshen/development/tools/zmqHelp
    )

    LINK_DIRECTORIES(
        /ephemeral/junshen/development/tools/Qt5.7.1/5.7/gcc_64/lib
        /ephemeral/junshen/development/tools/boost_1_65_1/gcc_bin/lib
        /ephemeral/junshen/development/tools/zeromq-4.3.1/gcc/lib
    )
else()
    INCLUDE_DIRECTORIES(
            /home/shenjun/tools/Qt5.7.1/5.7/gcc_64/include
            /home/shenjun/tools/boost_1_65_1/gcc_bin/include
            /home/shenjun/tools/zeromq-4.3.1/gcc/include
            /home/shenjun/tools/zmqHelp
            /home/shenjun/tools/spdlog-1.x/include
    )

    LINK_DIRECTORIES(
            /home/shenjun/tools/Qt5.7.1/5.7/gcc_64/lib
            /home/shenjun/tools/boost_1_65_1/gcc_bin/lib
            /home/shenjun/tools/zeromq-4.3.1/gcc/lib
    )
endif()

set(IncludePath "${CMAKE_CURRENT_SOURCE_DIR}")

# add source to execute
set (SOURCES
     applications/AppInstance.cpp
     applications/UserApp.cpp
     configurations/AppConfiguration.cpp
     games/GameLeela.cpp
     games/ManagementJob.cpp
     games/Job.cpp
     helps/ParseHelp.cpp
     sockets/ZmqReceiver.cpp
     sockets/ServiceReceiver.cpp
     LeelaControl.cpp
    )

set(INCLUDE
    applications/AppInstance.hpp
    applications/UserApp.hpp
    configurations/AppConfiguration.hpp
    configurations/DefineView.hpp
    games/IManagementJob.hpp
    games/ManagementJob.hpp
    games/GameLeela.hpp
    games/IJob.hpp
    games/Job.hpp
    helps/ParseHelp.hpp
    sockets/DescriptorPoller.hpp
    sockets/ServiceReceiver.hpp
    sockets/ZmqReceiver.hpp
    sockets/ISocket.hpp
    applications/ServiceContext.hpp
    LeelaControl.hpp
    )

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
else()
    target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE
            boost_program_options
            boost_thread
            boost_system
            boost_filesystem
            zmq
            Qt5Core
            logger
        )
endif()
       
add_subdirectory(logger)

# TODO: 如有需要，请添加测试并安装目标。
