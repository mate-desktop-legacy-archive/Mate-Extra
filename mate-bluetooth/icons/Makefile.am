
icons = mate_apps_16x16_bluetooth.png \
	mate_apps_22x22_bluetooth.png \
	mate_apps_24x24_bluetooth.png \
	mate_apps_32x32_bluetooth.png \
	mate_apps_48x48_bluetooth.png \
	mate_apps_scalable_bluetooth.svg \
	mate_status_16x16_bluetooth-active.png \
	mate_status_22x22_bluetooth-active.png \
	mate_status_24x24_bluetooth-active.png \
	mate_status_32x32_bluetooth-active.png \
	mate_status_48x48_bluetooth-active.png \
	mate_status_16x16_bluetooth-disabled.png \
	mate_status_22x22_bluetooth-disabled.png \
	mate_status_24x24_bluetooth-disabled.png \
	mate_status_32x32_bluetooth-disabled.png \
	mate_status_48x48_bluetooth-disabled.png \
	mate_status_16x16_bluetooth-paired.png \
	mate_status_22x22_bluetooth-paired.png \
	mate_status_24x24_bluetooth-paired.png \
	mate_status_32x32_bluetooth-paired.png \
	mate_status_scalable_bluetooth-paired.svg


noinst_DATA = mate_apps_16x16_bluetooth.svg \
		mate_apps_22x22_bluetooth.svg \
		mate_apps_32x32_bluetooth.svg \
		mate_status_16x16_bluetooth-active.svg \
		mate_status_22x22_bluetooth-active.svg \
		mate_status_32x32_bluetooth-active.svg \
		mate_status_48x48_bluetooth-active.svg \
		mate_status_16x16_bluetooth-disabled.svg \
		mate_status_22x22_bluetooth-disabled.svg \
		mate_status_32x32_bluetooth-disabled.svg \
		mate_status_48x48_bluetooth-disabled.svg

EXTRA_DIST = $(icons) $(noinst_DATA)

MAINTAINERCLEANFILES = Makefile.in

install-data-local:
	for i in $(icons); do \
		THEME=`echo $$i | cut -d_ -f1`; \
		CONTEXT=`echo $$i | cut -d_ -f2`; \
		SIZE=`echo $$i | cut -d_ -f3`; \
		FILE=`echo $$i | cut -d_ -f4`; \
		mkdir -p $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT; \
		$(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$FILE; \
	done

uninstall-local:
	for i in $(icons); do \
		THEME=`echo $$i | cut -d_ -f1`; \
		CONTEXT=`echo $$i | cut -d_ -f2`; \
		SIZE=`echo $$i | cut -d_ -f3`; \
		FILE=`echo $$i | cut -d_ -f4`; \
		rm -f $(DESTDIR)$(datadir)/icons/$$THEME/$$SIZE/$$CONTEXT/$$FILE; \
	done

install-data-hook:
if ICON_UPDATE
	$(UPDATE_ICON_CACHE) -f -t $(DESTDIR)$(datadir)/icons/mate
endif

uninstall-hook:
if ICON_UPDATE
	$(UPDATE_ICON_CACHE) $(DESTDIR)$(datadir)/icons/mate
endif

