import com.google.api.client.auth.oauth2.Credential;
import com.google.api.client.extensions.java6.auth.oauth2.AuthorizationCodeInstalledApp;
import com.google.api.client.extensions.jetty.auth.oauth2.LocalServerReceiver;
import com.google.api.client.googleapis.auth.oauth2.GoogleAuthorizationCodeFlow;
import com.google.api.client.googleapis.auth.oauth2.GoogleClientSecrets;
import com.google.api.client.googleapis.javanet.GoogleNetHttpTransport;
import com.google.api.client.http.HttpTransport;
import com.google.api.client.json.jackson2.JacksonFactory;
import com.google.api.client.json.JsonFactory;
import com.google.api.client.util.store.FileDataStoreFactory;
import com.google.api.client.util.DateTime;

import com.google.api.services.calendar.CalendarScopes;
import com.google.api.services.calendar.model.*;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.List;

public class AuthenticatedCalendar {
  private final String calendarID = "primary";

  /** Directory to store user credentials for this application. */
  private String credentialsName;
  private final java.io.File credentialsDataStoreDir;

  private FileDataStoreFactory dateStoreFactory;
  private final JsonFactory jsonFactory = JacksonFactory.getDefaultInstance();
  private HttpTransport httpTransport;

  /** Global instance of the scopes required by this quickstart.
   *
   * If modifying these scopes, delete your previously saved credentials
   * at ~/.credentials/calendar-java-quickstart
   */
  private final List<String> scopes = Arrays.asList(CalendarScopes.CALENDAR);

  public AuthenticatedCalendar(String credentialsName) {
    this.credentialsName = credentialsName;

    System.out.printf("Authenticating %s calendar \n", this.credentialsName);

    this.credentialsDataStoreDir = new java.io.File(
      System.getProperty("user.home"), ".credentials/calendar-sync-" + credentialsName
    );
    try {
      httpTransport = GoogleNetHttpTransport.newTrustedTransport();
      dateStoreFactory = new FileDataStoreFactory(credentialsDataStoreDir);
    } catch (Throwable t) {
      t.printStackTrace();
      System.exit(1);
    }
  }

  /**
   * Creates an authorized Credential object.
   * @return an authorized Credential object.
   * @throws IOException
   */
  public Credential authorize() throws IOException {
    // Load client secrets.
    InputStream inp = AuthenticatedCalendar.class.getResourceAsStream("/client_secret.json");
    GoogleClientSecrets clientSecrets = GoogleClientSecrets.load(jsonFactory, new InputStreamReader(inp));

    // Build flow and trigger user authorization request.
    GoogleAuthorizationCodeFlow flow =
            new GoogleAuthorizationCodeFlow.Builder(
                    httpTransport, jsonFactory, clientSecrets, scopes)
            .setDataStoreFactory(dateStoreFactory)
            .setAccessType("offline")
            .build();
    Credential credential = new AuthorizationCodeInstalledApp(flow, new LocalServerReceiver()).authorize("user");

    return credential;
  }

  /**
   * Build and return an authorized Calendar client service.
   * @return an authorized Calendar client service
   * @throws IOException
   */
  com.google.api.services.calendar.Calendar getCalendarService() throws IOException {
    Credential credential = authorize();
    return new com.google.api.services.calendar.Calendar.Builder(
            httpTransport, jsonFactory, credential)
            .setApplicationName(CalendarSync.APPLICATION_NAME)
            .build();
  }

  /**
   * Get all upcoming events for this Google Calendar
   * @return a list of all Event's
   * @throws IOException
  */
  public List<Event> getAllUpcomingEvents(int monthRange) throws IOException {
    com.google.api.services.calendar.Calendar service = getCalendarService();

    DateTime now = new DateTime(System.currentTimeMillis());
    DateTime max = new DateTime(System.currentTimeMillis() + (monthRange * 2629746000)); //2629746000 is aprox one month in milliseconds
    Events events = service.events().list(calendarID)
        .setTimeMin(now)
        .setTimeMax(max)
        .setOrderBy("startTime")
        .setSingleEvents(true)
        .execute();
    List<Event> items = events.getItems();

    return items;
  }

  /*
   * Create a new event and duplicate details from another event object
   * @return a duplicated event
  */
  private static Event duplicateEventObj(Event sourceEvent) {
    //Copy the summary, location, description, and start/end date to the new Event
    Event dupeEvent = new Event();

    //copy just the time data from the source event
    dupeEvent.setStart(sourceEvent.getStart());
    dupeEvent.setEnd(sourceEvent.getEnd());

    return dupeEvent;
  }

  /*
   * Duplicate and save an event to the Google Calendar
  */
  public void duplicateAndSaveEvent(Event event) {
    System.out.printf("Syncing event: %s \n", event.getSummary());

    //duplicate the details of the event
    Event eventDupe = this.duplicateEventObj(event);

    //change the colour of the duped event - so different from others
    eventDupe.setColorId("5");

    //mark it as a sync'd event
    Event.ExtendedProperties eventProps = new Event.ExtendedProperties();
    Map<String, String> eventPropsPrivate = new HashMap<String, String>();
    eventPropsPrivate.put('_has_synced_to_dest', 'true');
    eventProps.setPrivate(eventPropsPrivate);
    eventDupe.setExtendedProperties(eventProps);

    this.addEvent(eventDupe);
  }

  /*
   * Set the given Event as synced in it's properties
  */
  public void markAsSynced(Event event) {
    com.google.api.services.calendar.Calendar service = getCalendarService();

    Event.ExtendedProperties eventProps = event.getExtendedProperties();
    Map<String, String> eventPropsPrivate;

    if(eventProps == null) {
      //this event doesn't have any properites so they need to be created
      eventProps = new Event.ExtendedProperties();
      eventPropsPrivate = new HashMap<String, String>();
    } else {
      eventPropsPrivate = eventProps.getPrivate();
    }

    //set the has been synced value to true
    eventPropsPrivate.put('_has_synced_to_dest', 'true');

    //update the event properties
    eventProps.setPrivate(eventPropsPrivate);
    event.setExtendedProperties(eventProps);

    //send the update request to google calendar
    service.events().update(calendarID, event.getId(), event).execute();
  }

  /*
   * Add a given Event to the Google Calendar
  */
  public void addEvent(Event event) {
    com.google.api.services.calendar.Calendar service = getCalendarService();

    event = service.events().insert(calendarID, event).execute();
  }

  /*
   * Delete a given Event from the Google Calendar
  */
  public void deleteEvent(Event event) {
    com.google.api.services.calendar.Calendar service = getCalendarService();

    event = service.events().delete(calendarID, event.getId()).execute();
  }

  /*
   * Delete all events that were sync'd from this calendar
  */
  public void deleteAllSync() {
    com.google.api.services.calendar.Calendar service = getCalendarService();

    DateTime now = new DateTime(System.currentTimeMillis());
    Events events = service.events().list(calendarID)
        .setTimeMin(now)
        .setOrderBy("startTime")
        .setSingleEvents(true)
        .execute();
    List<Event> items = events.getItems();

    for (Event event : items) {
      Event.ExtendedProperties eventProps = event.getExtendedProperties();

      if(eventProps != null) {
        Map<String, String> eventPropsPrivate = eventProps.getPrivate();

        if(eventPropsPrivate.get('_has_synced_to_dest') != null) {
          this.deleteEvent(event);
        }
      }
    }
  }

}
