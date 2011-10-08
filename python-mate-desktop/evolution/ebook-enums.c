#include <ebook-enums.h>

/* Generated data (by glib-mkenums) */


/* enumerations from "/usr/include/evolution-data-server-1.4/libebook/e-contact.h" */
GType
e_contact_field_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { E_CONTACT_UID, "E_CONTACT_UID", "uid" },
      { E_CONTACT_FILE_AS, "E_CONTACT_FILE_AS", "file-as" },
      { E_CONTACT_BOOK_URI, "E_CONTACT_BOOK_URI", "book-uri" },
      { E_CONTACT_FULL_NAME, "E_CONTACT_FULL_NAME", "full-name" },
      { E_CONTACT_GIVEN_NAME, "E_CONTACT_GIVEN_NAME", "given-name" },
      { E_CONTACT_FAMILY_NAME, "E_CONTACT_FAMILY_NAME", "family-name" },
      { E_CONTACT_NICKNAME, "E_CONTACT_NICKNAME", "nickname" },
      { E_CONTACT_EMAIL_1, "E_CONTACT_EMAIL_1", "email-1" },
      { E_CONTACT_EMAIL_2, "E_CONTACT_EMAIL_2", "email-2" },
      { E_CONTACT_EMAIL_3, "E_CONTACT_EMAIL_3", "email-3" },
      { E_CONTACT_EMAIL_4, "E_CONTACT_EMAIL_4", "email-4" },
      { E_CONTACT_MAILER, "E_CONTACT_MAILER", "mailer" },
      { E_CONTACT_ADDRESS_LABEL_HOME, "E_CONTACT_ADDRESS_LABEL_HOME", "address-label-home" },
      { E_CONTACT_ADDRESS_LABEL_WORK, "E_CONTACT_ADDRESS_LABEL_WORK", "address-label-work" },
      { E_CONTACT_ADDRESS_LABEL_OTHER, "E_CONTACT_ADDRESS_LABEL_OTHER", "address-label-other" },
      { E_CONTACT_PHONE_ASSISTANT, "E_CONTACT_PHONE_ASSISTANT", "phone-assistant" },
      { E_CONTACT_PHONE_BUSINESS, "E_CONTACT_PHONE_BUSINESS", "phone-business" },
      { E_CONTACT_PHONE_BUSINESS_2, "E_CONTACT_PHONE_BUSINESS_2", "phone-business-2" },
      { E_CONTACT_PHONE_BUSINESS_FAX, "E_CONTACT_PHONE_BUSINESS_FAX", "phone-business-fax" },
      { E_CONTACT_PHONE_CALLBACK, "E_CONTACT_PHONE_CALLBACK", "phone-callback" },
      { E_CONTACT_PHONE_CAR, "E_CONTACT_PHONE_CAR", "phone-car" },
      { E_CONTACT_PHONE_COMPANY, "E_CONTACT_PHONE_COMPANY", "phone-company" },
      { E_CONTACT_PHONE_HOME, "E_CONTACT_PHONE_HOME", "phone-home" },
      { E_CONTACT_PHONE_HOME_2, "E_CONTACT_PHONE_HOME_2", "phone-home-2" },
      { E_CONTACT_PHONE_HOME_FAX, "E_CONTACT_PHONE_HOME_FAX", "phone-home-fax" },
      { E_CONTACT_PHONE_ISDN, "E_CONTACT_PHONE_ISDN", "phone-isdn" },
      { E_CONTACT_PHONE_MOBILE, "E_CONTACT_PHONE_MOBILE", "phone-mobile" },
      { E_CONTACT_PHONE_OTHER, "E_CONTACT_PHONE_OTHER", "phone-other" },
      { E_CONTACT_PHONE_OTHER_FAX, "E_CONTACT_PHONE_OTHER_FAX", "phone-other-fax" },
      { E_CONTACT_PHONE_PAGER, "E_CONTACT_PHONE_PAGER", "phone-pager" },
      { E_CONTACT_PHONE_PRIMARY, "E_CONTACT_PHONE_PRIMARY", "phone-primary" },
      { E_CONTACT_PHONE_RADIO, "E_CONTACT_PHONE_RADIO", "phone-radio" },
      { E_CONTACT_PHONE_TELEX, "E_CONTACT_PHONE_TELEX", "phone-telex" },
      { E_CONTACT_PHONE_TTYTDD, "E_CONTACT_PHONE_TTYTDD", "phone-ttytdd" },
      { E_CONTACT_ORG, "E_CONTACT_ORG", "org" },
      { E_CONTACT_ORG_UNIT, "E_CONTACT_ORG_UNIT", "org-unit" },
      { E_CONTACT_OFFICE, "E_CONTACT_OFFICE", "office" },
      { E_CONTACT_TITLE, "E_CONTACT_TITLE", "title" },
      { E_CONTACT_ROLE, "E_CONTACT_ROLE", "role" },
      { E_CONTACT_MANAGER, "E_CONTACT_MANAGER", "manager" },
      { E_CONTACT_ASSISTANT, "E_CONTACT_ASSISTANT", "assistant" },
      { E_CONTACT_HOMEPAGE_URL, "E_CONTACT_HOMEPAGE_URL", "homepage-url" },
      { E_CONTACT_BLOG_URL, "E_CONTACT_BLOG_URL", "blog-url" },
      { E_CONTACT_CATEGORIES, "E_CONTACT_CATEGORIES", "categories" },
      { E_CONTACT_CALENDAR_URI, "E_CONTACT_CALENDAR_URI", "calendar-uri" },
      { E_CONTACT_FREEBUSY_URL, "E_CONTACT_FREEBUSY_URL", "freebusy-url" },
      { E_CONTACT_ICS_CALENDAR, "E_CONTACT_ICS_CALENDAR", "ics-calendar" },
      { E_CONTACT_VIDEO_URL, "E_CONTACT_VIDEO_URL", "video-url" },
      { E_CONTACT_SPOUSE, "E_CONTACT_SPOUSE", "spouse" },
      { E_CONTACT_NOTE, "E_CONTACT_NOTE", "note" },
      { E_CONTACT_IM_AIM_HOME_1, "E_CONTACT_IM_AIM_HOME_1", "im-aim-home-1" },
      { E_CONTACT_IM_AIM_HOME_2, "E_CONTACT_IM_AIM_HOME_2", "im-aim-home-2" },
      { E_CONTACT_IM_AIM_HOME_3, "E_CONTACT_IM_AIM_HOME_3", "im-aim-home-3" },
      { E_CONTACT_IM_AIM_WORK_1, "E_CONTACT_IM_AIM_WORK_1", "im-aim-work-1" },
      { E_CONTACT_IM_AIM_WORK_2, "E_CONTACT_IM_AIM_WORK_2", "im-aim-work-2" },
      { E_CONTACT_IM_AIM_WORK_3, "E_CONTACT_IM_AIM_WORK_3", "im-aim-work-3" },
      { E_CONTACT_IM_GROUPWISE_HOME_1, "E_CONTACT_IM_GROUPWISE_HOME_1", "im-groupwise-home-1" },
      { E_CONTACT_IM_GROUPWISE_HOME_2, "E_CONTACT_IM_GROUPWISE_HOME_2", "im-groupwise-home-2" },
      { E_CONTACT_IM_GROUPWISE_HOME_3, "E_CONTACT_IM_GROUPWISE_HOME_3", "im-groupwise-home-3" },
      { E_CONTACT_IM_GROUPWISE_WORK_1, "E_CONTACT_IM_GROUPWISE_WORK_1", "im-groupwise-work-1" },
      { E_CONTACT_IM_GROUPWISE_WORK_2, "E_CONTACT_IM_GROUPWISE_WORK_2", "im-groupwise-work-2" },
      { E_CONTACT_IM_GROUPWISE_WORK_3, "E_CONTACT_IM_GROUPWISE_WORK_3", "im-groupwise-work-3" },
      { E_CONTACT_IM_JABBER_HOME_1, "E_CONTACT_IM_JABBER_HOME_1", "im-jabber-home-1" },
      { E_CONTACT_IM_JABBER_HOME_2, "E_CONTACT_IM_JABBER_HOME_2", "im-jabber-home-2" },
      { E_CONTACT_IM_JABBER_HOME_3, "E_CONTACT_IM_JABBER_HOME_3", "im-jabber-home-3" },
      { E_CONTACT_IM_JABBER_WORK_1, "E_CONTACT_IM_JABBER_WORK_1", "im-jabber-work-1" },
      { E_CONTACT_IM_JABBER_WORK_2, "E_CONTACT_IM_JABBER_WORK_2", "im-jabber-work-2" },
      { E_CONTACT_IM_JABBER_WORK_3, "E_CONTACT_IM_JABBER_WORK_3", "im-jabber-work-3" },
      { E_CONTACT_IM_YAHOO_HOME_1, "E_CONTACT_IM_YAHOO_HOME_1", "im-yahoo-home-1" },
      { E_CONTACT_IM_YAHOO_HOME_2, "E_CONTACT_IM_YAHOO_HOME_2", "im-yahoo-home-2" },
      { E_CONTACT_IM_YAHOO_HOME_3, "E_CONTACT_IM_YAHOO_HOME_3", "im-yahoo-home-3" },
      { E_CONTACT_IM_YAHOO_WORK_1, "E_CONTACT_IM_YAHOO_WORK_1", "im-yahoo-work-1" },
      { E_CONTACT_IM_YAHOO_WORK_2, "E_CONTACT_IM_YAHOO_WORK_2", "im-yahoo-work-2" },
      { E_CONTACT_IM_YAHOO_WORK_3, "E_CONTACT_IM_YAHOO_WORK_3", "im-yahoo-work-3" },
      { E_CONTACT_IM_MSN_HOME_1, "E_CONTACT_IM_MSN_HOME_1", "im-msn-home-1" },
      { E_CONTACT_IM_MSN_HOME_2, "E_CONTACT_IM_MSN_HOME_2", "im-msn-home-2" },
      { E_CONTACT_IM_MSN_HOME_3, "E_CONTACT_IM_MSN_HOME_3", "im-msn-home-3" },
      { E_CONTACT_IM_MSN_WORK_1, "E_CONTACT_IM_MSN_WORK_1", "im-msn-work-1" },
      { E_CONTACT_IM_MSN_WORK_2, "E_CONTACT_IM_MSN_WORK_2", "im-msn-work-2" },
      { E_CONTACT_IM_MSN_WORK_3, "E_CONTACT_IM_MSN_WORK_3", "im-msn-work-3" },
      { E_CONTACT_IM_ICQ_HOME_1, "E_CONTACT_IM_ICQ_HOME_1", "im-icq-home-1" },
      { E_CONTACT_IM_ICQ_HOME_2, "E_CONTACT_IM_ICQ_HOME_2", "im-icq-home-2" },
      { E_CONTACT_IM_ICQ_HOME_3, "E_CONTACT_IM_ICQ_HOME_3", "im-icq-home-3" },
      { E_CONTACT_IM_ICQ_WORK_1, "E_CONTACT_IM_ICQ_WORK_1", "im-icq-work-1" },
      { E_CONTACT_IM_ICQ_WORK_2, "E_CONTACT_IM_ICQ_WORK_2", "im-icq-work-2" },
      { E_CONTACT_IM_ICQ_WORK_3, "E_CONTACT_IM_ICQ_WORK_3", "im-icq-work-3" },
      { E_CONTACT_REV, "E_CONTACT_REV", "rev" },
      { E_CONTACT_NAME_OR_ORG, "E_CONTACT_NAME_OR_ORG", "name-or-org" },
      { E_CONTACT_ADDRESS, "E_CONTACT_ADDRESS", "address" },
      { E_CONTACT_ADDRESS_HOME, "E_CONTACT_ADDRESS_HOME", "address-home" },
      { E_CONTACT_ADDRESS_WORK, "E_CONTACT_ADDRESS_WORK", "address-work" },
      { E_CONTACT_ADDRESS_OTHER, "E_CONTACT_ADDRESS_OTHER", "address-other" },
      { E_CONTACT_CATEGORY_LIST, "E_CONTACT_CATEGORY_LIST", "category-list" },
      { E_CONTACT_PHOTO, "E_CONTACT_PHOTO", "photo" },
      { E_CONTACT_LOGO, "E_CONTACT_LOGO", "logo" },
      { E_CONTACT_NAME, "E_CONTACT_NAME", "name" },
      { E_CONTACT_EMAIL, "E_CONTACT_EMAIL", "email" },
      { E_CONTACT_IM_AIM, "E_CONTACT_IM_AIM", "im-aim" },
      { E_CONTACT_IM_GROUPWISE, "E_CONTACT_IM_GROUPWISE", "im-groupwise" },
      { E_CONTACT_IM_JABBER, "E_CONTACT_IM_JABBER", "im-jabber" },
      { E_CONTACT_IM_YAHOO, "E_CONTACT_IM_YAHOO", "im-yahoo" },
      { E_CONTACT_IM_MSN, "E_CONTACT_IM_MSN", "im-msn" },
      { E_CONTACT_IM_ICQ, "E_CONTACT_IM_ICQ", "im-icq" },
      { E_CONTACT_WANTS_HTML, "E_CONTACT_WANTS_HTML", "wants-html" },
      { E_CONTACT_IS_LIST, "E_CONTACT_IS_LIST", "is-list" },
      { E_CONTACT_LIST_SHOW_ADDRESSES, "E_CONTACT_LIST_SHOW_ADDRESSES", "list-show-addresses" },
      { E_CONTACT_BIRTH_DATE, "E_CONTACT_BIRTH_DATE", "birth-date" },
      { E_CONTACT_ANNIVERSARY, "E_CONTACT_ANNIVERSARY", "anniversary" },
      { E_CONTACT_X509_CERT, "E_CONTACT_X509_CERT", "x509-cert" },
      { E_CONTACT_FIELD_LAST, "E_CONTACT_FIELD_LAST", "field-last" },
      { E_CONTACT_FIELD_FIRST, "E_CONTACT_FIELD_FIRST", "field-first" },
      { E_CONTACT_LAST_SIMPLE_STRING, "E_CONTACT_LAST_SIMPLE_STRING", "last-simple-string" },
      { E_CONTACT_FIRST_PHONE_ID, "E_CONTACT_FIRST_PHONE_ID", "first-phone-id" },
      { E_CONTACT_LAST_PHONE_ID, "E_CONTACT_LAST_PHONE_ID", "last-phone-id" },
      { E_CONTACT_FIRST_EMAIL_ID, "E_CONTACT_FIRST_EMAIL_ID", "first-email-id" },
      { E_CONTACT_LAST_EMAIL_ID, "E_CONTACT_LAST_EMAIL_ID", "last-email-id" },
      { E_CONTACT_FIRST_ADDRESS_ID, "E_CONTACT_FIRST_ADDRESS_ID", "first-address-id" },
      { E_CONTACT_LAST_ADDRESS_ID, "E_CONTACT_LAST_ADDRESS_ID", "last-address-id" },
      { E_CONTACT_FIRST_LABEL_ID, "E_CONTACT_FIRST_LABEL_ID", "first-label-id" },
      { E_CONTACT_LAST_LABEL_ID, "E_CONTACT_LAST_LABEL_ID", "last-label-id" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("EContactField", values);
  }
  return etype;
}
GType
e_contact_photo_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { E_CONTACT_PHOTO_TYPE_INLINED, "E_CONTACT_PHOTO_TYPE_INLINED", "inlined" },
      { E_CONTACT_PHOTO_TYPE_URI, "E_CONTACT_PHOTO_TYPE_URI", "uri" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("EContactPhotoType", values);
  }
  return etype;
}

/* Generated data ends here */

/* Generated data (by glib-mkenums) */


/* enumerations from "/usr/include/evolution-data-server-1.4/libebook/e-book-query.h" */
GType
e_book_query_test_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { E_BOOK_QUERY_IS, "E_BOOK_QUERY_IS", "is" },
      { E_BOOK_QUERY_CONTAINS, "E_BOOK_QUERY_CONTAINS", "contains" },
      { E_BOOK_QUERY_BEGINS_WITH, "E_BOOK_QUERY_BEGINS_WITH", "begins-with" },
      { E_BOOK_QUERY_ENDS_WITH, "E_BOOK_QUERY_ENDS_WITH", "ends-with" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("EBookQueryTest", values);
  }
  return etype;
}

/* Generated data ends here */

