package harvest;
/**
 * Created by dominic on 23/11/2016.
 */
public enum SiteType {
    CONVENTIONAL('Conventional Growing'),
    HYDROPONICS('Hydroponics')

    String name

    SiteType(String name) {
        this.name = name
    }

    public static getDefault() {
        return CONVENTIONAL
    }
}
