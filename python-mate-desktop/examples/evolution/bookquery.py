import evolution.ebook as e
import unittest

class TestBookQuery(unittest.TestCase):
    
    def setUp(self):
        pass

    def testUninitialized(self):
        q1 = e.EBookQuery()
        self.assertEqual(q1.__str__(), "<Uninitialized>")

    def testFieldExists(self):
        q1 = e.EBookQuery.field_exists(e.CONTACT_PHOTO)
        self.assertEqual(q1.__str__(), "(exists \"photo\")")

    def testFieldExists_Raise(self):
        self.assertRaises(TypeError, e.EBookQuery.field_exists)
        self.assertRaises(TypeError, e.EBookQuery.field_exists, "foobar")

    def testVCardFieldExists(self):
        q1 = e.EBookQuery.vcard_field_exists("FN")
        self.assertEqual(q1.__str__(), "(exists_vcard \"FN\")")

    def testVCardFieldExists_Raise(self):
        self.assertRaises(TypeError, e.EBookQuery.vcard_field_exists)
        self.assertRaises(TypeError, e.EBookQuery.vcard_field_exists, 3)

    def testFieldTest_BeginsWith(self):
        q1 = e.EBookQuery.field_test(e.CONTACT_NAME,
                                    e.BOOK_QUERY_BEGINS_WITH,
                                    "dummy")
        self.assertEqual(q1.__str__(),
                         "(beginswith \"name\"  \"dummy\")")
        
    def testFieldTest_Contains(self):
        q1 = e.EBookQuery.field_test(e.CONTACT_NAME, e.BOOK_QUERY_CONTAINS,
                                    "dummy")
        self.assertEqual(q1.__str__(),
                         "(contains \"name\"  \"dummy\")")

    def testFieldTest_EndsWith(self):
        q1 = e.EBookQuery.field_test(e.CONTACT_NAME,
                                     e.BOOK_QUERY_ENDS_WITH,
                                     "dummy")
        self.assertEqual(q1.__str__(),
                         "(endswith \"name\"  \"dummy\")")
    def testFieldTest_Is(self):
        q1 = e.EBookQuery.field_test(e.CONTACT_NAME, e.BOOK_QUERY_IS,
                           "dummy")
        self.assertEqual(q1.__str__(),
                         "(is \"name\"  \"dummy\")")

    def testAnyFieldContains(self):
        q1 = e.EBookQuery.any_field_contains("dummy")
        self.assertEqual(q1.__str__(),
                         "(contains \"x-evolution-any-field\" \"dummy\")")

    def testAnd(self):
        q1 = e.EBookQuery.field_exists(e.CONTACT_PHOTO)
        q2 = e.EBookQuery.field_exists(e.CONTACT_NAME)
        q3 = e.EBookQuery.any_field_contains("dummy")

        q4 = q1._and([q2, q3])

        self.assertEqual(q4.__str__(),
        """(and (exists \"photo\") (exists \"name\") \
(contains \"x-evolution-any-field\" \"dummy\") )""")

        q5 = e.EBookQuery()
        q6 = q5._and(queries=[q1,q2])

        self.assertEqual(q6.__str__(),
                    "(and (exists \"photo\") (exists \"name\") )")

    def testOr(self):
        q1 = e.EBookQuery.field_exists(e.CONTACT_PHOTO)
        q2 = e.EBookQuery.field_exists(e.CONTACT_NAME)
        q3 = e.EBookQuery.any_field_contains("dummy")

        q4 = q1._or([q2, q3])

        self.assertEqual(q4.__str__(),
        """(or (exists \"photo\") (exists \"name\") \
(contains \"x-evolution-any-field\" \"dummy\") )""")

        q5 = e.EBookQuery()
        q6 = q5._or([q1,q2])

        self.assertEqual(q6.__str__(),
                    "(or (exists \"photo\") (exists \"name\") )")

    def testNot(self):
        q1 = e.EBookQuery.field_exists(e.CONTACT_EMAIL)
        q2 = q1._not()

    def testFromString(self):
        q1 = e.EBookQuery.from_string("(exists \"name\")")
        self.assertEqual(q1.__str__(), "(exists \"name\")")
if __name__ == "__main__":
    unittest.main()
