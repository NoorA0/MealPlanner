QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    attributionsdialogue.cpp \
    basetag.cpp \
    createmeal_basicparams.cpp \
    createmeal_nameconflict.cpp \
    createmeal_notagswarning.cpp \
    createmeal_promptlinktags.cpp \
    createplan_budget.cpp \
    createplan_confirmation.cpp \
    createplan_filename.cpp \
    createplan_length.cpp \
    createtag_basicparams.cpp \
    createtag_nameconflict.cpp \
    createtag_nomealswarning.cpp \
    createtag_promptlinkmeals.cpp \
    deletemeal_confirmation.cpp \
    deletetag_confirmation.cpp \
    editmeal_assignedtags.cpp \
    editmeal_basicparams.cpp \
    editmealswindow.cpp \
    edittag_assignedmeals.cpp \
    edittag_basicparams.cpp \
    edittagswindow.cpp \
    error_loaddatafailed.cpp \
    main.cpp \
    mainwindow.cpp \
    manualwindow.cpp \
    meal.cpp \
    mealmanager.cpp \
    multitag.cpp \
    settingswindow.cpp \
    tag.cpp

HEADERS += \
    attributionsdialogue.h \
    basetag.h \
    createmeal_basicparams.h \
    createmeal_nameconflict.h \
    createmeal_notagswarning.h \
    createmeal_promptlinktags.h \
    createplan_budget.h \
    createplan_confirmation.h \
    createplan_filename.h \
    createplan_length.h \
    createtag_basicparams.h \
    createtag_nameconflict.h \
    createtag_nomealswarning.h \
    createtag_promptlinkmeals.h \
    deletemeal_confirmation.h \
    deletetag_confirmation.h \
    editmeal_assignedtags.h \
    editmeal_basicparams.h \
    editmealswindow.h \
    edittag_assignedmeals.h \
    edittag_basicparams.h \
    edittagswindow.h \
    error_loaddatafailed.h \
    mainwindow.h \
    manualwindow.h \
    meal.h \
    mealmanager.h \
    multitag.h \
    settingswindow.h \
    tag.h

FORMS += \
    attributionsdialogue.ui \
    createmeal_basicparams.ui \
    createmeal_nameconflict.ui \
    createmeal_notagswarning.ui \
    createmeal_promptlinktags.ui \
    createplan_budget.ui \
    createplan_confirmation.ui \
    createplan_filename.ui \
    createplan_length.ui \
    createtag_basicparams.ui \
    createtag_nameconflict.ui \
    createtag_nomealswarning.ui \
    createtag_promptlinkmeals.ui \
    deletemeal_confirmation.ui \
    deletetag_confirmation.ui \
    editmeal_assignedtags.ui \
    editmeal_basicparams.ui \
    editmealswindow.ui \
    edittag_assignedmeals.ui \
    edittag_basicparams.ui \
    edittagswindow.ui \
    error_loaddatafailed.ui \
    mainwindow.ui \
    manualwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
