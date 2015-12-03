set(BIN_NAME final)

set(CPP_FILES
    main.cpp
    stb.cpp
    Camera.cpp
    Image.cpp
    Mesh.cpp
    Renderer.cpp
    SceneParser.cpp
    Texture.cpp
    VecUtils.cpp
    ArgParser.cpp
    CubeMap.cpp
    Noise.cpp
    ObjTriangle.cpp
    Octree.cpp
    PerlinNoise.cpp
    stb.cpp
    Texture.cpp
    MyNoise.cpp
    )

set(CPP_HEADERS
    Camera.h
    Group.h
    Hit.h
    Image.h
    Light.h
    Material.h
    Mesh.h
    Object3D.h
    Plane.h
    Ray.h
    Renderer.h
    SceneParser.h
    Sphere.h
    Transform.h
    Triangle.h
    VecUtils.h
    ArgParser.h
    CubeMap.h
    Noise.h
    ObjTriangle.h
    Octree.h
    PerlinNoise.h
    stb_image.h
    stb_image_write.h
    Texture.h
    MyNoise.h
    )

set(LIBS_USED
    vecmath
    )

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(LIBS_USED ${LIBS_USED} dl)
endif()

add_executable(${BIN_NAME} ${CPP_FILES} ${CPP_HEADERS})
target_link_libraries(${BIN_NAME} ${LIBS_USED})
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})
