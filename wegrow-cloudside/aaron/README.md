Calendar Sync
===

To setup this application you will need to create an application in the Google
Developer dashboard and download a `client_secret.json` file containing your
OAuth details.

Instructions on how to generate these can be found on the
https://developers.google.com/google-apps/calendar/quickstart/java page at the
top.

When you have enabled the APIs, and added a set of credentials (for a "other
UI" client), go to https://console.developers.google.com/apis/credentials (for
your API project) and download "native client 1" from the "oauth 2 client
ids").

Rename this file `client_secret.json` and place it in src/main/resources


==

To run `gradle -q run`

To run with a custom month range `gradle -q run -PmaxSyncMonth="5"`

To delete all sync'd events from the destination calendar `gradle -q run -PdeleteAllSync`
