
BASE_CPPFLAGS = \
    -Wall -Werror -Wextra

bin_PROGRAMS = cmdemo2daemon cmdemo2cli

cmdemo2daemon_SOURCES = \
    daemon/src/main.cpp
cmdemo2daemon_CPPFLAGS = $(BASE_CPPFLAGS) $(LIBCM_CFLAGS) $(LIBCMEXAMPLE2_CFLAGS)
cmdemo2daemon_LDADD = $(LIBCM_LIBS) $(LIBCMEXAMPLE2_LIBS)

cmdemo2cli_SOURCES = \
    cli/src/main.cpp \
    cli/include/commandvector.hpp \
    cli/include/commandparser.hpp \
    cli/include/commandexecutor.hpp \
    cli/src/commandparser.cpp \
    cli/src/commandexecutor.cpp
cmdemo2cli_CPPFLAGS = $(BASE_CPPFLAGS) $(LIBCM_CFLAGS) $(LIBCMEXAMPLE2_CFLAGS)
cmdemo2cli_LDADD = $(LIBCM_LIBS) $(LIBCMEXAMPLE2_LIBS)

if HAVE_SYSTEMD
nodist_systemdsystemunit_DATA = \
    cmdemo2daemon.service
endif

cmdemo2daemon.service: cmdemo2daemon.service.in
	sed -e 's![@]bindir[@]!$(bindir)!g' \
        $< > $@

if ENABLE_CM_CONF
activatordir = $(CM_CONF_DIR)
nodist_activator_DATA = \
    cmdemo2daemonactivator.json
endif

clean-local:
	rm -f cmdemo2daemon.service

EXTRA_DIST = \
    cmdemo2daemon.service.in \
    cmdemo2daemonactivator.json
