cmake_minimum_required(VERSION 3.29)
project(fpoo)

set(CMAKE_CXX_STANDARD 20)

add_executable(fpoo main.cpp
        DCaravana/Caravana.cpp
        DCaravana/Caravana.h
        Interface.cpp
        Interface.h
        Simulacao.cpp
        Simulacao.h
        DCaravana/Militar.cpp
        DCaravana/Militar.h
        DCaravana/Secreta.cpp
        DCaravana/Secreta.h
        DCaravana/Comercio.cpp
        DCaravana/Comercio.h
        Mapa.cpp
        Mapa.h
        DZona/Zona.cpp
        DZona/Zona.h
        DZona/Montanha.cpp
        DZona/Montanha.h
        DZona/Cidade.cpp
        DZona/Cidade.h
        DCaravana/Barbaro.cpp
        DCaravana/Barbaro.h
        DItem/Item.cpp
        DItem/Item.h
        DItem/Mina.cpp
        DItem/Mina.h
        DZona/Tempestade.cpp
        DZona/Tempestade.h
        DItem/Pandora.cpp
        DItem/Pandora.h
        DItem/Tesouro.cpp
        DItem/Tesouro.h
        DItem/Jaula.cpp
        DItem/Jaula.h
        DItem/Surpresa.cpp
        DItem/Surpresa.h
        Combate.cpp
        Combate.h
        Buffer.cpp
        Buffer.h
)
