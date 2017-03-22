import com.google.api.client.util.DateTime;
import com.google.api.services.calendar.model.*;

public class CalendarSync {
  private static final String APPLICATION_NAME = "Google Calendar Sync";

  public static void main(String[] params) {
    //Authenticate the source calendar
    AuthenticatedCalendar sourceCalendar = new AuthenticatedCalendar("source");
    //Authenticate the destination calendar
    AuthenticatedCalendar destinationCalendar = new AuthenticatedCalendar("destination");

    List<Event> upcomingSourceEvents = sourceCalendar.getAllUpcomingEvents();
    List<Event> upcomingDestinationEvents = sourceCalendar.getAllUpcomingEvents();

    for (Event event : upcomingSourceEvents) {
        DateTime start = event.getStart().getDateTime();
        if (start == null) {
            start = event.getStart().getDate();
        }

        //we are checking if this event has been marked as already synced
        //this is stored in the hidden properties of the google calendar event
        Event.ExtendedProperties eventProps = event.getExtendedProperties();

        if(eventProps == null) {
          sourceCalendar.markAsSynced(event);
          destinationCalendar.duplicateAndSaveEvent(event);
        } else {
          Map<String, String> eventPropsPrivate = eventProps.getPrivate();

          if(eventPropsPrivate.get('_has_synced_to_dest') == null) {
            sourceCalendar.markAsSynced(event);
            destinationCalendar.duplicateAndSaveEvent(event);
          }
        }
    }
  }
}
