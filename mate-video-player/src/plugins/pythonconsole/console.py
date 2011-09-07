# -*- coding: utf-8 -*-

# console.py -- Console widget
#
# Copyright (C) 2006 - Steve Frécinaux
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2, or (at your option)
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

# Parts from "Interactive Python-GTK Console" (stolen from epiphany's console.py)
#     Copyright (C), 1998 James Henstridge <james@daa.com.au>
#     Copyright (C), 2005 Adam Hooper <adamh@densi.com>
# Bits from gedit Python Console Plugin
#     Copyrignt (C), 2005 Raphaël Slinckx
#
# The Totem project hereby grant permission for non-gpl compatible GStreamer
# plugins to be used and distributed together with GStreamer and Totem. This
# permission are above and beyond the permissions granted by the GPL license
# Totem is covered by.
#
# Monday 7th February 2005: Christian Schaller: Add exception clause.
# See license_change file for details.

import string
import sys
import re
import traceback
import gobject
import pango
import gtk

class PythonConsole(gtk.ScrolledWindow):
	def __init__(self, namespace = {}, destroy_cb = None):
		gtk.ScrolledWindow.__init__(self)

		self.destroy_cb = destroy_cb
		self.set_policy(gtk.POLICY_NEVER, gtk.POLICY_AUTOMATIC);
		self.set_shadow_type(gtk.SHADOW_IN)
		self.view = gtk.TextView()
		self.view.modify_font(pango.FontDescription('Monospace'))
		self.view.set_editable(True)
		self.view.set_wrap_mode(gtk.WRAP_WORD_CHAR)
		self.add(self.view)
		self.view.show()

		buffer = self.view.get_buffer()
		self.normal = buffer.create_tag("normal")
		self.error  = buffer.create_tag("error")
		self.error.set_property("foreground", "red")
		self.command = buffer.create_tag("command")
		self.command.set_property("foreground", "blue")

		self.__spaces_pattern = re.compile(r'^\s+')		
		self.namespace = namespace

		self.block_command = False

		# Init first line
		buffer.create_mark("input-line", buffer.get_end_iter(), True)
		buffer.insert(buffer.get_end_iter(), ">>> ")
		buffer.create_mark("input", buffer.get_end_iter(), True)

		# Init history
		self.history = ['']
		self.history_pos = 0
		self.current_command = ''
		self.namespace['__history__'] = self.history

		# Set up hooks for standard output.
		self.stdout = OutFile(self, sys.stdout.fileno(), self.normal)
		self.stderr = OutFile(self, sys.stderr.fileno(), self.error)
		
		# Signals
		self.view.connect("key-press-event", self.__key_press_event_cb)
		buffer.connect("mark-set", self.__mark_set_cb)
		
 		
	def __key_press_event_cb(self, view, event):
		if event.keyval == gtk.keysyms.d and event.state == gtk.gdk.CONTROL_MASK:
			self.destroy()
		
		elif event.keyval == gtk.keysyms.Return and event.state == gtk.gdk.CONTROL_MASK:
			# Get the command
			buffer = view.get_buffer()
			inp_mark = buffer.get_mark("input")
			inp = buffer.get_iter_at_mark(inp_mark)
			cur = buffer.get_end_iter()
			line = buffer.get_text(inp, cur)
			self.current_command = self.current_command + line + "\n"
			self.history_add(line)

			# Prepare the new line
			cur = buffer.get_end_iter()
			buffer.insert(cur, "\n... ")
			cur = buffer.get_end_iter()
			buffer.move_mark(inp_mark, cur)
			
			# Keep indentation of precendent line
			spaces = re.match(self.__spaces_pattern, line)
			if spaces is not None:
				buffer.insert(cur, line[spaces.start() : spaces.end()])
				cur = buffer.get_end_iter()
				
			buffer.place_cursor(cur)
			gobject.idle_add(self.scroll_to_end)
			return True
		
		elif event.keyval == gtk.keysyms.Return:
			# Get the marks
			buffer = view.get_buffer()
			lin_mark = buffer.get_mark("input-line")
			inp_mark = buffer.get_mark("input")

			# Get the command line
			inp = buffer.get_iter_at_mark(inp_mark)
			cur = buffer.get_end_iter()
			line = buffer.get_text(inp, cur)
			self.current_command = self.current_command + line + "\n"
			self.history_add(line)

			# Make the line blue
			lin = buffer.get_iter_at_mark(lin_mark)
			buffer.apply_tag(self.command, lin, cur)
			buffer.insert(cur, "\n")
			
			cur_strip = self.current_command.rstrip()

			if cur_strip.endswith(":") \
			or (self.current_command[-2:] != "\n\n" and self.block_command):
				# Unfinished block command
				self.block_command = True
				com_mark = "... "
			elif cur_strip.endswith("\\"):
				com_mark = "... "
			else:
				# Eval the command
				self.__run(self.current_command)
				self.current_command = ''
				self.block_command = False
				com_mark = ">>> "

			# Prepare the new line
			cur = buffer.get_end_iter()
			buffer.move_mark(lin_mark, cur)
			buffer.insert(cur, com_mark)
			cur = buffer.get_end_iter()
			buffer.move_mark(inp_mark, cur)
			buffer.place_cursor(cur)
			gobject.idle_add(self.scroll_to_end)
			return True

		elif event.keyval == gtk.keysyms.KP_Down or event.keyval == gtk.keysyms.Down:
			# Next entry from history
			view.emit_stop_by_name("key_press_event")
			self.history_down()
			gobject.idle_add(self.scroll_to_end)
			return True

		elif event.keyval == gtk.keysyms.KP_Up or event.keyval == gtk.keysyms.Up:
			# Previous entry from history
			view.emit_stop_by_name("key_press_event")
			self.history_up()
			gobject.idle_add(self.scroll_to_end)
			return True

		elif event.keyval == gtk.keysyms.KP_Left or event.keyval == gtk.keysyms.Left or \
		     event.keyval == gtk.keysyms.BackSpace:
			buffer = view.get_buffer()
			inp = buffer.get_iter_at_mark(buffer.get_mark("input"))
			cur = buffer.get_iter_at_mark(buffer.get_insert())
			return inp.compare(cur) == 0

		elif event.keyval == gtk.keysyms.Home:
			# Go to the begin of the command instead of the begin of the line
			buffer = view.get_buffer()
			inp = buffer.get_iter_at_mark(buffer.get_mark("input"))
			if event.state == gtk.gdk.SHIFT_MASK:
				buffer.move_mark_by_name("insert", inp)
			else:
				buffer.place_cursor(inp)
			return True
		
	def __mark_set_cb(self, buffer, iter, name):
		input = buffer.get_iter_at_mark(buffer.get_mark("input"))
		pos   = buffer.get_iter_at_mark(buffer.get_insert())
		self.view.set_editable(pos.compare(input) != -1)

	def get_command_line(self):
		buffer = self.view.get_buffer()
		inp = buffer.get_iter_at_mark(buffer.get_mark("input"))
		cur = buffer.get_end_iter()
		return buffer.get_text(inp, cur)
	
	def set_command_line(self, command):
		buffer = self.view.get_buffer()
		mark = buffer.get_mark("input")
		inp = buffer.get_iter_at_mark(mark)
		cur = buffer.get_end_iter()
		buffer.delete(inp, cur)
		buffer.insert(inp, command)
		buffer.select_range(buffer.get_iter_at_mark(mark), buffer.get_end_iter())
		self.view.grab_focus()
	
	def history_add(self, line):
		if line.strip() != '':
			self.history_pos = len(self.history)
			self.history[self.history_pos - 1] = line
			self.history.append('')
	
	def history_up(self):
		if self.history_pos > 0:
			self.history[self.history_pos] = self.get_command_line()
			self.history_pos = self.history_pos - 1
			self.set_command_line(self.history[self.history_pos])
			
	def history_down(self):
		if self.history_pos < len(self.history) - 1:
			self.history[self.history_pos] = self.get_command_line()
			self.history_pos = self.history_pos + 1
			self.set_command_line(self.history[self.history_pos])
	
	def scroll_to_end(self):
		iter = self.view.get_buffer().get_end_iter()
		self.view.scroll_to_iter(iter, 0.0)
		return False

	def write(self, text, tag = None):
		buffer = self.view.get_buffer()
		if tag is None:
			buffer.insert(buffer.get_end_iter(), text)
		else:
			buffer.insert_with_tags(buffer.get_end_iter(), text, tag)
		gobject.idle_add(self.scroll_to_end)
 	
 	def eval(self, command, display_command = False):
		buffer = self.view.get_buffer()
		lin = buffer.get_mark("input-line")
		buffer.delete(buffer.get_iter_at_mark(lin),
		              buffer.get_end_iter())
 
		if isinstance(command, list) or isinstance(command, tuple):
 			for c in command:
		 		if display_command:
		 			self.write(">>> " + c + "\n", self.command)
 				self.__run(c)
		else:
	 		if display_command:
	 			self.write(">>> " + c + "\n", self.command)
			self.__run(command) 

		cur = buffer.get_end_iter()
		buffer.move_mark_by_name("input-line", cur)
		buffer.insert(cur, ">>> ")
		cur = buffer.get_end_iter()
		buffer.move_mark_by_name("input", cur)
		self.view.scroll_to_iter(buffer.get_end_iter(), 0.0)
	
 	def __run(self, command):
		sys.stdout, self.stdout = self.stdout, sys.stdout
		sys.stderr, self.stderr = self.stderr, sys.stderr
		
		try:
			try:
				r = eval(command, self.namespace, self.namespace)
				if r is not None:
					print `r`
			except SyntaxError:
				exec command in self.namespace
		except:
			if hasattr(sys, 'last_type') and sys.last_type == SystemExit:
				self.destroy()
			else:
				traceback.print_exc()

		sys.stdout, self.stdout = self.stdout, sys.stdout
		sys.stderr, self.stderr = self.stderr, sys.stderr

	def destroy(self):
		pass
		#gtk.ScrolledWindow.destroy(self)
		
class OutFile:
	"""A fake output file object. It sends output to a TK test widget,
	and if asked for a file number, returns one set on instance creation"""
	def __init__(self, console, fn, tag):
		self.fn = fn
		self.console = console
		self.tag = tag
	def close(self):         pass
	def flush(self):         pass
	def fileno(self):        return self.fn
	def isatty(self):        return 0
	def read(self, a):       return ''
	def readline(self):      return ''
	def readlines(self):     return []
	def write(self, s):      self.console.write(s, self.tag)
	def writelines(self, l): self.console.write(l, self.tag)
	def seek(self, a):       raise IOError, (29, 'Illegal seek')
	def tell(self):          raise IOError, (29, 'Illegal seek')
	truncate = tell
