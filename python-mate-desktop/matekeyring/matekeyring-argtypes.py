import argtypes

class AttrList(argtypes.ArgType):
    def write_param(self, ptype, pname, pdflt, pnull, info):
        info.varlist.add('MateKeyringAttributeList *', pname)
        info.varlist.add('PyObject *', 'py_' + pname)
        info.add_parselist('O', ['&py_' + pname], [pname])
        info.codebefore.append(
            ("    %(pname)s = pymate_keyring_attribute_list_from_pyobject(py_%(pname)s);\n")
            % vars())
        info.arglist.append(pname)
        info.codeafter.append("    mate_keyring_attribute_list_free(%(pname)s);\n" % vars())
    def write_return(self, ptype, ownsreturn, info):
        info.varlist.add('MateKeyringAttributeList*', 'ret')
        info.codeafter.append('return pymate_keyring_attribute_list_as_pyobject(ret);')

argtypes.matcher.register('MateKeyringAttributeList*', AttrList())

class MateKeyringResultArg(argtypes.IntArg):
    def write_return(self, ptype, ownsreturn, info):
        info.varlist.add('MateKeyringResult', 'ret')
	info.codeafter.append('    if (pymatekeyring_result_check(ret))\n'
			      '        return NULL;\n'
			      '    Py_INCREF(Py_None);\n'
			      '    return Py_None;')

argtypes.matcher.register('MateKeyringResult', MateKeyringResultArg())
