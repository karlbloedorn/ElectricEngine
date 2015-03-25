project(EETestGame)
file(GLOB SUB_SRCS
	"Graphics/*.cpp"
	"Scene/*.cpp"
	"*.cpp"
	"Graphics/*.c"
)

add_executable(EETestGame Main.cpp ${SUB_SRCS})
target_link_libraries(EETestGame ${SDL2_LIBRARY} ${JSONCPP_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARY})
install(TARGETS EETestGame RUNTIME DESTINATION ${BIN_DIR})