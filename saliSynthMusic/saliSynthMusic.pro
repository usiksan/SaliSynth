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
        audioOut/SoundMidiChannel.cpp \
        audioOut/SoundMidiOut.cpp \
        audioOut/SoundPolyphonyChannel.cpp \
        audioOut/SoundPolyphonyManager.cpp \
        iff/IffReader.cpp \
        main.cpp \
        midiFile/MidiFile.cpp \
        midiInput/MidiInput.cpp \
        objects/MidiProcessor.cpp \
        soundFont/SfSynth.cpp \
        soundFont/SfSynthNote.cpp \
        soundFont/SfSynthPreset.cpp \
        soundFont/SfSynthTrack.cpp \
        soundFont/SoundFont.cpp \
        soundFont/SoundFontMap.cpp

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
  audioOut/SoundMidiChannel.h \
  audioOut/SoundMidiOut.h \
  audioOut/SoundPolyphonyChannel.h \
  audioOut/SoundPolyphonyManager.h \
  iff/IffReader.h \
  midiFile/MidiFile.h \
  midiInput/MidiInput.h \
  objects/MidiProcessor.h \
  soundFont/SfSynth.h \
  soundFont/SfSynthNote.h \
  soundFont/SfSynthPreset.h \
  soundFont/SfSynthTrack.h \
  soundFont/SoundFont.h \
  soundFont/SoundFontMap.h \
  synthConfig.h
