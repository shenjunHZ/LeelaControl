# CMakeList.txt: LeelaControl CMake project
#
#
if(${WIN_ENV})
    INCLUDE_DIRECTORIES(
        C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/include
        C:/RootPath/Open_Resource/Qt-x86-5.10.1/5.10.1/msvc2017_64/include
        C:/RootPath/Open_Resource/boost_1_65_1
        C:/RootPath/Open_Resource/zeromq-4.2.2/include
        C:/RootPath/Open_Resource/zmqHelp
        C:/RootPath/Open_Resource/spdlog-1.x/include
    )
    # zmq need use dynamic, donot know why static failed
    LINK_DIRECTORIES(
        C:/RootPath/Open_Resource/Qt-x86-5.10.1/5.10.1/msvc2017_64/lib
        C:/RootPath/Open_Resource/boost_1_65_1/lib64-msvc-2017
        C:/RootPath/Open_Resource/zeromq-4.2.2/bin/x64/Release/v141/dynamic
    )
elseif(${ARM_LINUX})
    # required packages
    set(Boost_MIN_VERSION "1.58.0")
    set(Boost_USE_MULTITHREADED ON)
    find_package(Boost 1.58.0 REQUIRED program_options filesystem)

    find_package(Qt5Core REQUIRED)
    find_package(PkgConfig REQUIRED)
	pkg_check_modules(ZMQ libzmq)
	message(status, "${ZMQ_LIBRARY_DIRS}")

	link_directories(${ZMQ_LIBRARY_DIRS})

    # include
	include_directories(${ZMQ_INCLUDE_DIRS})
	message("ZMQ include path ${ZMQ_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
	message("Boost include path ${Boost_INCLUDE_DIRS}")
    include_directories(${Qt5Core_INCLUDE_DIRS})
    message("QT include path ${Qt5Core_INCLUDE_DIRS}")
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
     sockets/SocketMonitorNotifier.cpp
     sockets/tcp/TcpServEndpoint.cpp
     sockets/tcp/TcpSysCall.cpp
     sockets/tcp/TcpSysCallFactory.cpp
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
    sockets/SocketMonitorNotifier.hpp
    sockets/tcp/TcpServEndpoint.hpp
    sockets/tcp/TcpSysCall.hpp
    sockets/tcp/TcpSysCallFactory.hpp
    sockets/tcp/types/IpVersion.hpp
    sockets/tcp/types/TcpMessageEnvelope.hpp
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
            libzmq
            Qt5Core
            logger
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
