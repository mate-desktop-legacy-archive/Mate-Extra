
#ifndef __totempluginviewer_marshal_MARSHAL_H__
#define __totempluginviewer_marshal_MARSHAL_H__

#include	<glib-object.h>

G_BEGIN_DECLS

/* VOID:UINT,UINT (marshal.list:1) */
G_GNUC_INTERNAL void totempluginviewer_marshal_VOID__UINT_UINT (GClosure     *closure,
                                                                GValue       *return_value,
                                                                guint         n_param_values,
                                                                const GValue *param_values,
                                                                gpointer      invocation_hint,
                                                                gpointer      marshal_data);

/* VOID:UINT,UINT,STRING (marshal.list:2) */
G_GNUC_INTERNAL void totempluginviewer_marshal_VOID__UINT_UINT_STRING (GClosure     *closure,
                                                                       GValue       *return_value,
                                                                       guint         n_param_values,
                                                                       const GValue *param_values,
                                                                       gpointer      invocation_hint,
                                                                       gpointer      marshal_data);

/* VOID:STRING,BOXED (marshal.list:3) */
G_GNUC_INTERNAL void totempluginviewer_marshal_VOID__STRING_BOXED (GClosure     *closure,
                                                                   GValue       *return_value,
                                                                   guint         n_param_values,
                                                                   const GValue *param_values,
                                                                   gpointer      invocation_hint,
                                                                   gpointer      marshal_data);

G_END_DECLS

#endif /* __totempluginviewer_marshal_MARSHAL_H__ */

