set(SRCS
	"src/board.cpp"
	"src/board.h"
	"src/error.cpp"
	"src/error.h"
	"src/eval_params.h"
	"src/eval_params.cpp"
	"src/extract.cpp"
	"src/extract.h"
	"src/main.cpp"
	"src/optimize.cpp"
	"src/optimize.h"
	"src/pos.cpp"
	"src/pos.h"
)

add_executable(tune ${SRCS})

target_compile_features(tune PRIVATE cxx_std_20)

# for Visual Studio/MSVC
set_target_properties(tune PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SRCS})