cmake_minimum_required(VERSION 3.28)
project(CoCo2026_TuesdayTeamB_Edich_Lenz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(BISON 3.8.2 REQUIRED)
find_package(FLEX REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
    ${CMAKE_CURRENT_BINARY_DIR}
)

BISON_TARGET(
    MyParser 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/parser.yy 
    ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
    COMPILE_FLAGS "-d"
)

FLEX_TARGET(
    MyScanner 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/lexer.ll 
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/Driver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/ASTNode.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/ASTPrinter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/ASTVisitor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/SourceLocation.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common/Types.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/src/symboltable/Symboltable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/symboltable/SymboltablePrinter.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ast/ASTSConstructor.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/manual/parser/MParser.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/manual/parser/Sets.hh
    ${CMAKE_CURRENT_SOURCE_DIR}/src/manual/lexer/lexer.hh
        ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "compiler")