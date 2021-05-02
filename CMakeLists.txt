cmake_minimum_required(VERSION 3.12)

project(pineapple-metronome VERSION 0.0.1)

set (EXE_NAME pmetronome)
set (ASSETS_NAME pmetronome_assets)

find_package(JUCE CONFIG REQUIRED)
# or
# add_subdirectory("../JUCE" JUCE)

# juce_set_vst2_sdk_path(...)  # This setup should be done before calling `juce_add_gui_app`.

# `juce_add_gui_app` adds an executable target with the name passed as the first argument
# (GuiAppExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_gui_app(${EXE_NAME}
    # VERSION ...                       # Set this if the app version is different to the project version
    ICON_BIG  assets/app-icon.png       # ICON_* arguments specify a path to an image file to use as an icon
    ICON_SMALL assets/app-icon.png
    # DOCUMENT_EXTENSIONS ...           # Specify file extensions that should be associated with this app
    # COMPANY_NAME ...                  # Specify the name of the app's author
    PRODUCT_NAME "Pineapple Metronome"           # The name of the final executable, which can differ from the target name
)

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# juce_generate_juce_header(GuiAppExample)

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(${EXE_NAME}
    PRIVATE
        Source/Main.cpp
        Source/MainComponent.h
        Source/MainComponent.cpp
        Source/SpinButtonComponent.h
        Source/SpinButtonComponent.cpp
        Source/Metronome.h
        Source/Metronome.cpp
)

target_compile_definitions(${EXE_NAME}
    PRIVATE
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:${EXE_NAME},JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:${EXE_NAME},JUCE_VERSION>"
        JUCER_ENABLE_GPL_MODE=1
        JUCE_DISPLAY_SPLASH_SCREEN=0
)

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

juce_add_binary_data(${ASSETS_NAME}
    SOURCES
        assets/Snare.wav
        assets/translation_zh_CN.txt
)

# blumia: reserved for Windows SDK when building with audio features.
#         we don't need it right now with a very tiny modification to JUCE.

#target_include_directories(${EXE_NAME}
#    PRIVATE
#        "C:\\Program Files (x86)\\Windows Kits\\10\\Include\\10.0.19041.0"
#)

target_link_options(${EXE_NAME}
    PRIVATE
        -static
)

target_link_libraries(${EXE_NAME}
    PRIVATE
        ${ASSETS_NAME}            # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_formats
        juce::juce_audio_utils
        juce::juce_gui_extra
        juce::juce_opengl # to use OpenGLContext for Android
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
