public class CalendarSync {
  private static final String APPLICATION_NAME = "Google Calendar Sync";

  public static void main(String[] params) {
    //Authenticate the source calendar
    AuthenticatedCalendar sourceCalendar = new AuthenticatedCalendar("source");
    sourceCalendar.printAllUpcomingTest();
  }
}
