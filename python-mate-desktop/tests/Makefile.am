tests = 

PYTHONPATH = $(top_builddir):$(top_srcdir):$(shell echo $$PYTHONPATH)

check-local:
	@PYTHONPATH=$(PYTHONPATH) $(PYTHON) -W error $(srcdir)/runtests.py

EXTRA_DIST = $(tests) common.py runtests.py ltihooks.py
