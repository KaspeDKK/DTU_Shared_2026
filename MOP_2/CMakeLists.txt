cmake_minimum_required(VERSION 4.2)
project(MOP_2 C)

set(CMAKE_C_STANDARD 11)

# Text-based game executable
add_executable(MOP_2 main.c
        Model/model.c
        Model/model.h
        View/view.c
        View/view.h
        Controller/controller.h
        Controller/controller.c
        Controller/CardServices.c
        Controller/CardServices.h
        Model/Types.h
        Model/deck.h
        Model/deck.c
)

# Socket server executable (for GUI communication)
add_executable(server
        Controller/server.c
        Model/model.c
        Model/model.h
        View/view.c
        View/view.h
        Controller/controller.h
        Controller/controller.c
        Controller/CardServices.c
        Controller/CardServices.h
        Model/Types.h
        Model/deck.h
        Model/deck.c
)

if (WIN32)
    target_link_libraries(MOP_2 ws2_32)
    target_link_libraries(server ws2_32)
endif ()