cmake_minimum_required(VERSION 3.25)
project(FSD_Rebased)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Qt/6.5.1/mingw_64/lib/cmake")
#add_subdirectory(Hiredis)
find_package(Qt6 COMPONENTS
        Core
        Network
        Sql
        Concurrent
        REQUIRED)



add_executable(FSD_Rebased
        main.cpp
        Global.cpp
        Global.h
        Settings.cpp
        Settings.h
        Server.cpp
        Server.h
        Client.cpp
        Client.h
        Mysql.cpp
        Mysql.h
        pdu/serializer.h
        pdu/serializer.cpp
        pdu/client_properties.h
        enums.h
        pdu/pdu/pdu_add_atc.cpp
        pdu/pdu/pdu_add_atc.h
        pdu/pdu/pdu_base.cpp
        pdu/pdu/pdu_base.h
        pdu/pdu/pdu_fast_pilot_position.h
        pdu/pdu/pdu_fast_pilot_position.cpp
        pdu/pdu/pdu_pilot_position.cpp
        pdu/pdu/pdu_pilot_position.h
        pdu/pdu/pdu_atc_position.cpp
        pdu/pdu/pdu_atc_position.h
        pdu/pdu/pdu_delete_atc.cpp
        pdu/pdu/pdu_delete_atc.h
        pdu/pdu/pdu_add_pilot.cpp
        pdu/pdu/pdu_add_pilot.h
        pdu/pdu/pdu_delete_pilot.cpp
        pdu/pdu/pdu_delete_pilot.h
        pdu/pdu/pdu_metar_response.cpp
        pdu/pdu/pdu_metar_response.h
        pdu/pdu/pdu_metar_request.cpp
        pdu/pdu/pdu_metar_request.h
        pdu/pdu/pdu_ping.cpp
        pdu/pdu/pdu_ping.h
        pdu/pdu/pdu_plane_info_request.cpp
        pdu/pdu/pdu_plane_info_request.h
        pdu/pdu/pdu_plane_info_response.cpp
        pdu/pdu/pdu_plane_info_response.h
        pdu/pdu/pdu_pong.cpp
        pdu/pdu/pdu_pong.cpp
        pdu/pdu/pdu_client_query.cpp
        pdu/pdu/pdu_client_query.h
        pdu/pdu/pdu_client_query_response.cpp
        pdu/pdu/pdu_client_query_response.h
        pdu/pdu/pdu_kill_request.cpp
        pdu/pdu/pdu_kill_request.h
        pdu/pdu/pdu_protocol_error.cpp
        pdu/pdu/pdu_protocol_error.h
        pdu/pdu/pdu_send_fast.cpp
        pdu/pdu/pdu_send_fast.h
        pdu/pdu/pdu_text_message.cpp
        pdu/pdu/pdu_text_message.h
        pdu/pdu/pdu_flight_plan.cpp
        pdu/pdu/pdu_flight_plan.h
        Weather.cpp
        Weather.h
        pdu/pdu/pdu_hand_off.h
        pdu/pdu/pdu_hand_off.cpp pdu/pdu/pdu_hand_off_acknowledge.cpp)
target_link_libraries(FSD_Rebased
        Qt::Core
        Qt::Network
        Qt::Sql
        Qt::Concurrent
        )

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Network Sql Concurrent)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()

install(TARGETS FSD_Rebased
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )
