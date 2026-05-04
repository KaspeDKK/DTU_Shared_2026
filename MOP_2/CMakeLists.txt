cmake_minimum_required(VERSION 4.2)
project(MOP_2 C)

set(CMAKE_C_STANDARD 11)

add_executable(MOP_2 main.c
        Model/model.c
        Model/model.h
        View/view.c
        View/view.h
        main.c
        Controller/controller.h
        Controller/controller.c
        Controller/CardServices.c
        Controller/CardServices.h
        Model/Types.h
        Model/deck.h
        Model/deck.c
)
