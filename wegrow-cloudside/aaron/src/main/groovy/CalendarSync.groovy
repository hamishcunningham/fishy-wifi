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

        System.out.printf("%s (%s)\n", event.getSummary(), start);
        Event eventDupe = destinationCalendar.duplicateEvent(event);
        destinationCalendar.addEvent(eventDupe);
    }
  }
}
