set(PROJECT_NAME MaroRenderer)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "Engine/Shaders/background.fs"
    "Engine/Shaders/background.vs"
    "Engine/Shaders/basic2d.fs"
    "Engine/Shaders/basic2d.vs"
    "Engine/Shaders/brdf.fs"
    "Engine/Shaders/brdf.vs"
    "Engine/Shaders/cubemap.vs"
    "Engine/Shaders/equi_to_cubemap.fs"
    "Engine/Shaders/irradiance.fs"
    "Engine/Shaders/normal_mapping.fs"
    "Engine/Shaders/normal_mapping.vs"
    "Engine/Shaders/pbr.fs"
    "Engine/Shaders/pbr.vs"
    "Engine/Shaders/prefilter.fs"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "Application/Application.h"
    "Application/Events/ApplicationEvent.h"
    "Application/Events/Event.h"
    "Application/Events/KeyEvent.h"
    "Application/Events/MouseEvent.h"
    "Application/File/FileHandler.h"
    "Application/GUI/GUILayer.h"
    "Application/Window.h"
    "Engine/Camera/ArcballCamera.h"
    "Engine/Camera/Camera.h"
    "Engine/Camera/PerspectiveCamera.h"
    "Engine/Lighting/Light.h"
    "Engine/Lighting/PointLight.h"
    "Engine/Objects/Actor.h"
    "Engine/Objects/Mesh.h"
    "Engine/Objects/Model.h"
    "Engine/Objects/Scene.h"
    "Engine/Objects/SceneNode.h"
    "Engine/Objects/Skybox.h"
    "Engine/Rendering/Framebuffer.h"
    "Engine/Rendering/Renderbuffer.h"
    "Engine/Rendering/Renderer.h"
    "Engine/Rendering/Shader.h"
    "include/imgui/imconfig.h"
    "include/imgui/imgui.h"
    "include/imgui/imgui_impl_glfw.h"
    "include/imgui/imgui_impl_opengl3.h"
    "include/imgui/imgui_internal.h"
    "include/imgui/imstb_rectpack.h"
    "include/imgui/imstb_textedit.h"
    "include/imgui/imstb_truetype.h"
    "include/stb_image.h"
    "Logging/Logger.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Application/Application.cpp"
    "Application/GUI/GUILayer.cpp"
    "Application/main.cpp"
    "Application/Window.cpp"
    "Engine/Camera/ArcballCamera.cpp"
    "Engine/Camera/PerspectiveCamera.cpp"
    "Engine/Lighting/Light.cpp"
    "Engine/Lighting/PointLight.cpp"
    "Engine/Objects/Mesh.cpp"
    "Engine/Objects/Model.cpp"
    "Engine/Objects/Scene.cpp"
    "Engine/Objects/SceneNode.cpp"
    "Engine/Objects/Skybox.cpp"
    "Engine/Rendering/Framebuffer.cpp"
    "Engine/Rendering/Renderbuffer.cpp"
    "Engine/Rendering/Renderer.cpp"
    "Engine/Rendering/Shader.cpp"
    "glad.c"
    "include/imgui/imgui.cpp"
    "include/imgui/imgui_demo.cpp"
    "include/imgui/imgui_draw.cpp"
    "include/imgui/imgui_impl_glfw.cpp"
    "include/imgui/imgui_impl_opengl3.cpp"
    "include/imgui/imgui_widgets.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE MaroRenderer)

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/include;"
            "${CMAKE_CURRENT_SOURCE_DIR}/.;"
            "${CMAKE_CURRENT_SOURCE_DIR}/include/assimp"
        ">"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG:FULL;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "kernel32;"
            "user32;"
            "gdi32;"
            "winspool;"
            "comdlg32;"
            "advapi32;"
            "shell32;"
            "ole32;"
            "oleaut32;"
            "uuid;"
            "odbc32;"
            "odbccp32;"
            "glfw3;"
            "assimp-vc142-mtd"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "glfw3"
        ">"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//include/assimp/lib"
        ">"
    )
endif()

