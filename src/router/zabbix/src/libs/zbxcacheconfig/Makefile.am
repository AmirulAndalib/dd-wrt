## Process this file with automake to produce Makefile.in

noinst_LIBRARIES = libzbxcacheconfig.a

libzbxcacheconfig_a_SOURCES = \
	dbhistoryconfig.c \
	dbconfig.c \
	dbconfig.h \
	dbconfig_dump.c \
	dbconfig_maintenance.c \
	dbsync.c \
	dbsync.h \
	inventory.c \
	lld_macro.c \
	trigger.c \
	user_macro.c \
	user_macro.h \
	vps_monitor.c \
	vps_monitor.h \
	proxy_group.c \
	proxy_group.h

libzbxcacheconfig_a_CFLAGS = \
	-I$(top_srcdir)/src/zabbix_server/ \
	$(TLS_CFLAGS) \
	$(TEST_FLAGS)
