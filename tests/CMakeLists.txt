cmake_minimum_required(VERSION 3.5)

ucm_add_files(
	"tests.cpp"
	"tests_spas.cpp"
	"Semaphore.h"
	"testsPCH.h"
	"testsPCH.cpp"
	TO TESTS_SRC
	)

#ADD_MSVC_PRECOMPILED_HEADER("testsPCH.h" "testsPCH.cpp" TESTS_SRC)
message( "CMAKE_EXE_LINKER_FLAGS = " ${CMAKE_EXE_LINKER_FLAGS})
message( "COMPILER FLAGS USED")
message( "CMAKE_CXX_FLAGS                = " ${CMAKE_CXX_FLAGS})
message( "CMAKE_CXX_FLAGS_DEBUG          = " ${CMAKE_CXX_FLAGS_DEBUG})
message( "CMAKE_CXX_FLAGS_RELEASE        = " ${CMAKE_CXX_FLAGS_RELEASE})
message( "CMAKE_CXX_FLAGS_MINSIZEREL     = " ${CMAKE_CXX_FLAGS_MINSIZEREL})
message( "CMAKE_CXX_FLAGS_RELWITHDEBINFO = " ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

add_executable(tests ${TESTS_SRC})
cz_set_precompiled_header(tests "testsPCH.h" "testsPCH.cpp")

cz_set_postfix(tests)
target_link_libraries(tests czspas UnitTest++)

