/* MateConf
 * Copyright (C) 1999, 2000 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "mateconf-error.h"
#include "mateconf-internals.h"
#include <stdarg.h>

static const gchar* err_msgs[] = {
  N_("Success"),
  N_("Failed"),
  N_("Configuration server couldn't be contacted"),
  N_("Permission denied"),
  N_("Couldn't resolve address for configuration source"),
  N_("Bad key or directory name"),
  N_("Parse error"),
  N_("Corrupt data in configuration source database"),
  N_("Type mismatch"),
  N_("Key operation on directory"),
  N_("Directory operation on key"),
  N_("Can't overwrite existing read-only value"),
  N_("Object Activation Framework error"),
  N_("Operation not allowed without configuration server"),
  N_("Failed to get a lock"),
  N_("No database available to save your configuration")
};

static const int n_err_msgs = sizeof(err_msgs)/sizeof(err_msgs[0]);

static const gchar* 
mateconf_strerror       (MateConfError en)
{
  g_return_val_if_fail (en < n_err_msgs, NULL);

  return _(err_msgs[en]);    
}

GQuark
mateconf_error_quark (void)
{
  static GQuark err_q = 0;

  if (err_q == 0)
    err_q = g_quark_from_static_string ("mateconf-error-quark");

  return err_q;
}

static GError* 
mateconf_error_new_valist(MateConfError en, const gchar* fmt, va_list args)
{
  GError *err;
  gchar* orig;
  
  orig = g_strdup_vprintf(fmt, args);

  err = g_error_new (MATECONF_ERROR, en, "%s: %s",
                     mateconf_strerror (en),
                     orig);  

  g_free(orig);
  
  return err;
}

GError*
mateconf_error_new(MateConfError en, const gchar* fmt, ...)
{
  GError* err;
  va_list args;
  
  va_start (args, fmt);
  err = mateconf_error_new_valist(en, fmt, args);
  va_end (args);

  return err;
}

void
mateconf_set_error      (GError** err, MateConfError en, const gchar* fmt, ...)
{
  GError* obj;
  va_list args;

  if (err == NULL)
    return;

  /* Warn if we stack up errors on top
   * of each other. Keep the "deepest"
   * error
   */
  g_return_if_fail(*err == NULL);
  
  va_start (args, fmt);
  obj = mateconf_error_new_valist(en, fmt, args);
  va_end (args);

  *err = obj;
}

/* This function should die. */
GError*
mateconf_compose_errors (GError* err1, GError* err2)
{
  if (err1 == NULL && err2 == NULL)
    return NULL;
  else if (err1 == NULL)
    return g_error_copy(err2);
  else if (err2 == NULL)
    return g_error_copy(err1);
  else
    {
      GError *n;

      n = g_error_new (MATECONF_ERROR, MATECONF_ERROR_FAILED, " ");

      if (err1->code == err2->code)
        n->code = err1->code;
      else
        n->code = MATECONF_ERROR_FAILED;

      g_free (n->message);
      
      n->message = g_strconcat(err1->message, "\n", err2->message, NULL);

      return n;
    }
}

