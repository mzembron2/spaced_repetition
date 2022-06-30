cmake_minimum_required(VERSION 3.21)
project(spaced_repetition)

enable_testing()

include(FetchContent)
       FetchContent_Declare(
          googletest
          URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Multimedia
        REQUIRED)

add_executable(spaced_repetition
        src/Main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        UI/mainwindow.ui
        src/EditWindow.cpp
        src/EditWindow.h
        UI/editwindow.ui
        src/AddNewDeckWindow.cpp
        src/AddNewDeckWindow.h
        UI/addnewdeckwindow.ui
        src/Algorithm.h
        src/Algorithm.cpp
        src/DatabaseController.cpp
        src/DatabaseController.h
        src/Flashcard.h src/Flashcard.cpp
        src/LearnWidget.h src/LearnWidget.cpp
        UI/learnwidget.ui
        src/StartWidget.h src/StartWidget.cpp
        UI/startwidget.ui
        src/ImageWidget.h src/ImageWidget.cpp
        UI/imagewidget.ui
        src/SoundWidget.h src/SoundWidget.cpp
        UI/soundwidget.ui
        src/FlashcardWidget.h src/FlashcardWidget.cpp
        src/TextWidget.h src/TextWidget.cpp
        UI/textwidget.ui)

target_link_libraries(spaced_repetition
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Sql
        Qt5::Multimedia)


# comment out everything below if windows does not see gtest or qt (section only required for unit testing)
add_executable(
     unit_tests
       tests/UnitTestsDatabase.cpp
       tests/UnitTestsFlashcard.cpp
       tests/UnitTestsAlgorithm.cpp
       tests/UnitTestsSoundWidget.cpp
       tests/UnitTestsImageWidget.cpp
       src/MainWindow.cpp
       src/MainWindow.h
       UI/mainwindow.ui
       src/EditWindow.cpp
       src/EditWindow.h
       UI/editwindow.ui
       src/AddNewDeckWindow.cpp
       src/AddNewDeckWindow.h
       UI/addnewdeckwindow.ui
       src/Algorithm.h
       src/Algorithm.cpp
       src/DatabaseController.cpp
       src/DatabaseController.h
       src/Flashcard.h src/Flashcard.cpp
       src/LearnWidget.h src/LearnWidget.cpp
       UI/learnwidget.ui
       src/StartWidget.h src/StartWidget.cpp
       UI/startwidget.ui
       src/ImageWidget.h src/ImageWidget.cpp
       UI/imagewidget.ui
       src/SoundWidget.h src/SoundWidget.cpp
       UI/soundwidget.ui
       src/FlashcardWidget.h src/FlashcardWidget.cpp
       src/TextWidget.h src/TextWidget.cpp
       UI/textwidget.ui)

target_link_libraries(
    unit_tests
    gtest_main
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::Sql
    Qt5::Multimedia)


include(GoogleTest)
gtest_discover_tests(unit_tests)


