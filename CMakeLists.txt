cmake_minimum_required(VERSION 3.24.2)
project(gtwd)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(./Decoder Decoder_SOURCES)
aux_source_directory(./Encoder Encoder_SOURCES)
aux_source_directory(./Utils Utils_SOURCES)

include_directories(CLI)
include_directories(Decoder)
include_directories(Encoder)
include_directories(Utils)

add_executable(gtwd
        main.cpp
        ${Decoder_SOURCES}
        ${Encoder_SOURCES}
        ${Utils_SOURCES})
