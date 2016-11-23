package harvest

class HydroponicsTagLib {
    static defaultEncodeAs = [taglib:'none']
    //static encodeAsForTags = [tagName: [taglib:'html'], otherTagName: [taglib:'none']]

    def ifHydro = { attrs, body ->
        Settings settings = Settings.first()
        if (settings.type == SiteType.HYDROPONICS) {
            out << body()
        }
    }

    def ifConventional = { attrs, body ->
        Settings settings = Settings.first()
        if (settings.type == SiteType.CONVENTIONAL) {
            out << body()
        }
    }
}
