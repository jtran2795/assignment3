ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h TutorialApplication.h GameObject.h Simulator.h OgreMotionState.h SoundSystem.h GameState.h
CEGUI_CFLAGS = -pthread -I/lusr/opt/cegui-0.8.7/include/cegui-0 -I/usr/include/OGRE
CEGUI_LIBS = -L/lusr/opt/cegui-0.8.7/lib -lCEGUIOgreRenderer-0 -lOgreMain -lpthread -lCEGUIBase-0

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp TutorialApplication.cpp GameObject.cpp Simulator.cpp OgreMotionState.cpp SoundSystem.cpp GameState.cpp
OgreApp_CXXFLAGS= -I/usr/include/OGRE -pthread $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(SDL_mixer_CFLAGS) $(SDLnet_CFLAGS) $(CEGUI_CFLAGS)
OgreApp_LDADD= -L/usr/lib/x86_64-linux-gnu/OGRE-1.9.0 -lOgreMain -lpthread $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_mixer_LIBS)  $(SDLnet_LIBS) $(CEGUI_LIBS)
OgreApp_LDFLAGS= -lOgreOverlay -lboost_system -R/lusr/opt/cegui-0.8.7

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign


