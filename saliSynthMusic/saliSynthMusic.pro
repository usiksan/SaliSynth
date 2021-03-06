QT += quick network multimedia
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Host/SvTime2x.cpp \
        SvQml/SvDir.cpp \
        SvQml/SvQmlHtmlImageProvider.cpp \
        SvQml/SvQmlJsonFile.cpp \
        SvQml/SvQmlJsonHistory.cpp \
        SvQml/SvQmlJsonModel.cpp \
        SvQml/SvQmlJsonObjectModelFile.cpp \
        SvQml/SvQmlJsonString.cpp \
        SvQml/SvQmlJsonValue.cpp \
        SvQml/SvQmlUtils.cpp \
        audioOut/SoundBufferIODevice.cpp \
        iff/IffReader.cpp \
        main.cpp \
        midiInput/MidiInput.cpp \
        objects/MidiProcessor.cpp \
        objects/QmlKeyboard.cpp \
        objects/QmlMidiFile.cpp \
        objects/QmlMidiTrack.cpp \
        objects/QmlStyleFile.cpp \
        soundFont/SfSynth.cpp \
        soundFont/SfSynthMetronom.cpp \
        soundFont/SfSynthNote.cpp \
        soundFont/SfSynthTrack.cpp \
        soundFont/SfSynthVoice.cpp \
        soundFont/SoundFont.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  Host/SvTime2x.h \
  SvQml/SvDir.h \
  SvQml/SvQmlHtmlImageProvider.h \
  SvQml/SvQmlJsonFile.h \
  SvQml/SvQmlJsonHistory.h \
  SvQml/SvQmlJsonModel.h \
  SvQml/SvQmlJsonObjectModelFile.h \
  SvQml/SvQmlJsonString.h \
  SvQml/SvQmlJsonValue.h \
  SvQml/SvQmlUtils.h \
  audioOut/SoundBufferIODevice.h \
  iff/IffReader.h \
  midiInput/MidiInput.h \
  objects/MidiProcessor.h \
  objects/QmlKeyboard.h \
  objects/QmlMidiFile.h \
  objects/QmlMidiTrack.h \
  objects/QmlStyleFile.h \
  soundFont/SfSynth.h \
  soundFont/SfSynthMetronom.h \
  soundFont/SfSynthNote.h \
  soundFont/SfSynthTrack.h \
  soundFont/SfSynthVoice.h \
  soundFont/SfVoiceId.h \
  soundFont/SoundFont.h \
  synthConfig.h
