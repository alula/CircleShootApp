project(SexyAppFramework LANGUAGES CXX)

add_library(SexyAppFramework STATIC
    BassLoader.cpp
    BassMusicInterface.cpp
    Buffer.cpp
    ButtonWidget.cpp
    Checkbox.cpp
    Color.cpp
    Common.cpp
    CritSect.cpp
    CursorWidget.cpp
    D3D8Helper.cpp
    D3DInterface.cpp
    D3DTester.cpp
    DDImage.cpp
    DDInterface.cpp
    Debug.cpp
    DescParser.cpp
    DialogButton.cpp
    Dialog.cpp
    DirectXErrorString.cpp
    DSoundInstance.cpp
    DSoundManager.cpp
    EditWidget.cpp
    Flags.cpp
    Font.cpp
    Graphics.cpp
    HTTPTransfer.cpp
    HyperlinkWidget.cpp
    Image.cpp
    ImageFont.cpp
    Insets.cpp
    KeyCodes.cpp
    ListWidget.cpp
    MemoryImage.cpp
    ModVal.cpp
    MTRand.cpp
    MusicInterface.cpp
    NativeDisplay.cpp
    PerfTimer.cpp
    PropertiesParser.cpp
    Quantize.cpp
    Ratio.cpp
    ResourceManager.cpp
    ScrollbarWidget.cpp
    ScrollbuttonWidget.cpp
    SEHCatcher.cpp
    SexyAppBase.cpp
    SexyMatrix.cpp
    SharedImage.cpp
    Slider.cpp
    SWTri.cpp
    SysFont.cpp
    TextWidget.cpp
    WidgetContainer.cpp
    Widget.cpp
    WidgetManager.cpp
    XMLParser.cpp

    ogg/bitwise.c
    ogg/block.c
    ogg/codebook.c
    ogg/floor0.c
    ogg/floor1.c
    ogg/framing.c
    ogg/info.c
    ogg/mapping0.c
    ogg/mdct.c
    ogg/registry.c
    ogg/res012.c
    ogg/sharedbook.c
    ogg/synthesis.c
    ogg/vorbisfile.c
    ogg/window.c    
)

target_include_directories(SexyAppFramework PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/d3d8
)

target_link_libraries(SexyAppFramework ImageLib PakLib ws2_32 winmm gdi32 ole32)