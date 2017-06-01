TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    bc_bius.c \
    sock.c \
    bc_control_arg.c \
    bc_control_Low.c \
    bc_du.c \
    bc_dz.c \
    bc_main.c \
    rt_main.c \
    rt_bius.c \
    rt_du.c \
    rt_sd.c \
    rt_us.c \
    rt_dz.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    defs.h
#if defined(WIN32)
  #  LIBS += -lws2_32
#endif
LIBS += -lpthread

FORMS +=
