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
    createmultitag_basicparams.cpp \
    createmultitag_nameconflict.cpp \
    createmultitag_notagswarning.cpp \
    createmultitag_promptlinktags.cpp \
    createplan_budget.cpp \
    createplan_confirmation.cpp \
    createplan_creationdisplayerror.cpp \
    createplan_filename.cpp \
    createplan_generatorcrashed.cpp \
    createplan_inprogress.cpp \
    createplan_length.cpp \
    createplan_nomealsortagswarning.cpp \
    createplan_success.cpp \
    createtag_basicparams.cpp \
    createtag_nameconflict.cpp \
    createtag_nomealswarning.cpp \
    createtag_promptlinkmeals.cpp \
    deletemeal_confirmation.cpp \
    deletemultitag_confirmation.cpp \
    deletetag_confirmation.cpp \
    editmeal_assignedtags.cpp \
    editmeal_basicparams.cpp \
    editmealswindow.cpp \
    editmultitag_assignedtags.cpp \
    editmultitag_assigntagmeals.cpp \
    editmultitag_basicparams.cpp \
    editmultitagswindow.cpp \
    edittag_assignedmeals.cpp \
    edittag_basicparams.cpp \
    edittagswindow.cpp \
    error_loaddatafailed.cpp \
    main.cpp \
    mainwindow.cpp \
    manual_summaryinfo.cpp \
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
    createmultitag_basicparams.h \
    createmultitag_nameconflict.h \
    createmultitag_notagswarning.h \
    createmultitag_promptlinktags.h \
    createplan_budget.h \
    createplan_confirmation.h \
    createplan_creationdisplayerror.h \
    createplan_filename.h \
    createplan_generatorcrashed.h \
    createplan_inprogress.h \
    createplan_length.h \
    createplan_nomealsortagswarning.h \
    createplan_success.h \
    createtag_basicparams.h \
    createtag_nameconflict.h \
    createtag_nomealswarning.h \
    createtag_promptlinkmeals.h \
    deletemeal_confirmation.h \
    deletemultitag_confirmation.h \
    deletetag_confirmation.h \
    editmeal_assignedtags.h \
    editmeal_basicparams.h \
    editmealswindow.h \
    editmultitag_assignedtags.h \
    editmultitag_assigntagmeals.h \
    editmultitag_basicparams.h \
    editmultitagswindow.h \
    edittag_assignedmeals.h \
    edittag_basicparams.h \
    edittagswindow.h \
    error_loaddatafailed.h \
    mainwindow.h \
    manual_summaryinfo.h \
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
    createmultitag_basicparams.ui \
    createmultitag_nameconflict.ui \
    createmultitag_notagswarning.ui \
    createmultitag_promptlinktags.ui \
    createplan_budget.ui \
    createplan_confirmation.ui \
    createplan_creationdisplayerror.ui \
    createplan_filename.ui \
    createplan_generatorcrashed.ui \
    createplan_inprogress.ui \
    createplan_length.ui \
    createplan_nomealsortagswarning.ui \
    createplan_success.ui \
    createtag_basicparams.ui \
    createtag_nameconflict.ui \
    createtag_nomealswarning.ui \
    createtag_promptlinkmeals.ui \
    deletemeal_confirmation.ui \
    deletemultitag_confirmation.ui \
    deletetag_confirmation.ui \
    editmeal_assignedtags.ui \
    editmeal_basicparams.ui \
    editmealswindow.ui \
    editmultitag_assignedtags.ui \
    editmultitag_assigntagmeals.ui \
    editmultitag_basicparams.ui \
    editmultitagswindow.ui \
    edittag_assignedmeals.ui \
    edittag_basicparams.ui \
    edittagswindow.ui \
    error_loaddatafailed.ui \
    mainwindow.ui \
    manual_summaryinfo.ui \
    manualwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
