from evolution import ecal
cal = ecal.open_calendar_source('file:///home/john/.evolution/calendar/local/system',ecal.CAL_SOURCE_TYPE_EVENT)
c = cal.get_all_objects()[0]

print c.get_as_string()
print cal.get_object_as_string(c)

