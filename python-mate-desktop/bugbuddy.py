import sys

_saved_except_hook = None
_appname = sys.argv[0]

def _bug_catcher(exctype, value, tb):
    import traceback
    import tempfile
    import os
    if exctype is not KeyboardInterrupt:
        msg = "".join(traceback.format_exception(exctype, value, tb))
        print >> sys.stderr, msg
        fd, name = tempfile.mkstemp()
        try:
            os.write(fd,msg)
            os.system("bug-buddy --include=\"%s\" --appname=\"%s\"" % (name, _appname))
        finally:
            os.unlink(name)
    raise SystemExit


def install(appname=None):
    global _saved_except_hook, _appname
    if appname is None:
        appname = sys.argv[0]
    _appname = appname
    if sys.excepthook is not _bug_catcher:
        _saved_except_hook = sys.excepthook
        sys.excepthook = _bug_catcher


def uninstall():
    global _saved_except_hook
    if sys.excepthook is _bug_catcher:
        sys.excepthook = _saved_except_hook
        _saved_except_hook = None


if not sys.stderr.isatty():
    install()
