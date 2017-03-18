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
  /** Directory to store user credentials for this application. */
  private final java.io.File credentialsDataStoreDir;

  private FileDataStoreFactory dateStoreFactory;
  private final JsonFactory jsonFactory = JacksonFactory.getDefaultInstance();
  private HttpTransport httpTransport;

  /** Global instance of the scopes required by this quickstart.
   *
   * If modifying these scopes, delete your previously saved credentials
   * at ~/.credentials/calendar-java-quickstart
   */
  private final List<String> scopes = Arrays.asList(CalendarScopes.CALENDAR_READONLY);

  public AuthenticatedCalendar(String credentialsName) {
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
    System.out.println("Credentials saved to " + credentialsDataStoreDir.getAbsolutePath());
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

  public void printAllUpcomingTest() throws IOException {
    // Build a new authorized API client service.
    // Note: Do not confuse this class with the
    //   com.google.api.services.calendar.model.Calendar class.
    com.google.api.services.calendar.Calendar service =
        getCalendarService();

    // List the next 10 events from the primary calendar.
    DateTime now = new DateTime(System.currentTimeMillis());
    Events events = service.events().list("primary")
        .setMaxResults(10)
        .setTimeMin(now)
        .setOrderBy("startTime")
        .setSingleEvents(true)
        .execute();
    List<Event> items = events.getItems();
    if (items.size() == 0) {
        System.out.println("No upcoming events found.");
    } else {
        System.out.println("Upcoming events");
        for (Event event : items) {
            DateTime start = event.getStart().getDateTime();
            if (start == null) {
                start = event.getStart().getDate();
            }
            System.out.printf("%s (%s)\n", event.getSummary(), start);
        }
    }
  }

}
