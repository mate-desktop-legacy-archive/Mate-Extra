# Copyright (C) 2006-2008 Gustavo J. A. M. Carneiro  <gjcarneiro@gmail.com>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

"""
A bzr plugin that allows bzr log to output in GNU ChangeLog format.

Installation:
  Copy this file to ~/.bazaar/plugins/

Usage:
  bzr log -v --log-format 'gnu'

Changes:
  - 2008-12-08: include information of fixed bugs (bzr commit --fixes)
"""

import time
import textwrap
from cStringIO import StringIO
import codecs

Encoder = codecs.getwriter('ascii')

import bzrlib.log
#from bzrlib.osutils import format_date

class GnuFormatter(bzrlib.log.LogFormatter):

    def __init__(self, *args, **kwargs):
        bzrlib.log.LogFormatter.__init__(self, *args, **kwargs)
        self._changes_buffer = StringIO()
        self._date_line = None
        self.supports_tags = True
        self.supports_delta = True

    def log_revision(self, lr):
        self.show(lr.revno, lr.rev, lr.delta, lr.tags)
    
    def show(self, revno, rev, delta, tags=None):
        to_file = Encoder(self.to_file, errors='replace')
        if tags is not None:
            self._flush()
            print >> to_file, "=== %s ===" % ', '.join(tags)
        date_str = time.strftime("%Y-%m-%d", time.localtime(rev.timestamp))
        author = rev.get_apparent_author().strip()
        date_line = date_str + "  " + author
        if date_line != self._date_line:
            self._flush()
            self._date_line = date_line
        self._show_changes(revno, rev, delta)

    def _flush(self):
        if self._date_line is None or not self._changes_buffer.getvalue():
            return
        to_file = Encoder(self.to_file, errors='replace')
        print >> to_file, self._date_line
        print >> to_file
        to_file.write(self._changes_buffer.getvalue())
        self._changes_buffer.truncate(0)

    def begin_log(self):
        pass

    def end_log(self):
        self._flush()

    def __del__(self): # this is evil, I need bzr to call a method on me when it's finished..
        self._flush()

    def _show_changes(self, revno, rev, delta):
        to_file = Encoder(self._changes_buffer, errors='replace')
        output_standalone_message = True

        message = "[%s] %s" % (revno, rev.message)
        bugs_text = rev.properties.get('bugs', '')
        if bugs_text:
            if message[-1] in '.;':
                message = message[:-1]
            message = "%s; %s" % (message, bugs_text)
        
        if delta is not None:
            ## special case when only text modifications exist
            if delta.modified and not (delta.added or delta.removed
                                       or delta.renamed):
                for num, (path, id, kind, text_modified, meta_modified) \
                        in enumerate(delta.modified):
                    if num == len(delta.modified) - 1:
                        for line in textwrap.wrap(
                                "%s: %s" % (path, message),
                                width=70,
                                initial_indent="* ",
                                subsequent_indent=""):
                            print >> to_file, "\t" + line
                    else:
                        print >> to_file, "\t* %s," % (path,)
                output_standalone_message = False
                print >> to_file

            else:

                self._flush()
                
                if delta.added:
                    for num, (path, id, kind) in enumerate(delta.added):
                        if num == len(delta.added) - 1:
                            print >> to_file, "\t* %s: Added." % (path,)
                            print >> to_file
                        else:
                            print >> to_file, "\t* %s," % (path,)

                if delta.removed:
                    for num, (path, id, kind) in enumerate(delta.removed):
                        if num == len(delta.removed) - 1:
                            print >> to_file, "\t* %s: Removed." % (path,)
                            print >> to_file
                        else:
                            print >> to_file, "\t* %s," % (path,)

                if delta.modified:
                    for num, (path, id, kind, text_modified, meta_modified) \
                            in enumerate(delta.modified):
                        if num == len(delta.modified) - 1:
                            print >> to_file, "\t* %s: Modified." % (path,)
                            print >> to_file
                        else:
                            print >> to_file, "\t* %s," % (path,)

                if delta.renamed:
                    for (oldpath, newpath, id, kind,
                         text_modified, meta_modified) in delta.renamed:
                        if text_modified:
                            line = ("%s: Renamed to %s and modified."\
                                    % (oldpath, newpath))
                        else:
                            line = "%s: Renamed to %s." % (oldpath, newpath)
                        for l1 in textwrap.wrap(line, 70,
                                                initial_indent="* ",
                                                subsequent_indent="  "):
                            print >> to_file, "\t" + l1
                    print >> to_file

        if output_standalone_message:
            ## The log message, on a line by itself
            if not message:
                print >> to_file,  '\t(no message)'
            else:
                for l2 in textwrap.wrap(message, 70):
                    print >> to_file,  '\t' + l2

            print >> to_file
            self._flush()


bzrlib.log.register_formatter('gnu', GnuFormatter)

