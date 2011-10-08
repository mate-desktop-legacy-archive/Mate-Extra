import argtypes

class MatePrintReturnCodeArg(argtypes.IntArg):
    def write_return(self, ptype, ownsreturn, info):
        info.varlist.add('MatePrintReturnCode', 'ret')
	info.codeafter.append('    if (pymateprint_check_error(ret))\n'
			      '        return NULL;\n'
			      '    Py_INCREF(Py_None);\n'
			      '    return Py_None;')

argtypes.matcher.register('MatePrintReturnCode', MatePrintReturnCodeArg())

