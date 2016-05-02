import falcon
import json
import datetime
import os
from middleware import RequireJSON, JSONTranslator


class JSONWaterElfCollector:
    """Accepts JSON encoded data in the request body"""
    # The keys should be the same as the keys output by the WaterElf
    # Used for validation 
    KEY_TIMESTAMP = "timestamp"
    KEY_WATER_TEMP = "waterTemp"
    KEY_AIR_TEMP= "airTemp"
    KEY_PH = "pH"
    KEY_HUMIDITY = "humidity"
    KEY_LUX = "lux"
    
    def __init__(self):
        self.file_ext = "txt"
        # Set this to validate incoming data for particular key
        # eg [self.KEY_TIMESTAMP, self.KEY_WATER_TEMP, self.KEY_AIR]
        # No validation is currently set
        self.required_keys = None

    def on_get(self, req, resp, uuid):
        # Accept JSON data from the body as a dict via middleware
        data = req.context['json_input']

        self.check_data(data)
        output_path = self.generate_path(uuid)
        self.write_data_line(output_path, json.dumps(data)) 

    def write_data_line(self, path, line):
        with open("%s" % path, "a") as f:
            f.write(line + "\n")

    def generate_path(self, uuid):
        # Specify which type of path schematic to use
        path = self._uuid_dir_path(uuid)
        return "%s.%s" % (path, self.file_ext)

    def _flat_path(self, uuid):
        # Store files flat
        today = datetime.datetime.now().date().strftime("%Y-%m-%d")
        path = "%s_%s" % (today, uuid)
        return path

    def _uuid_dir_path(self, uuid):
        # Nest files in directories by uuid
        today = datetime.datetime.now().date().strftime("%Y-%m-%d")
        self._create_dir(uuid)
        path = "%s/%s_%s" % (uuid, today, uuid)
        return path
    
    def _create_dir(self, directory):
        # Check a dir exists - if not then create it
        if not os.path.exists(directory):
            os.makedirs(directory)

    def check_data(self, data):
        # Check that keys specified at the top exist in the data
        if self.required_keys is not None:
            for key in self.required_keys:
                if key not in data.keys():
                    raise falcon.HTTPMissingParam(key)

# Middleware enforces the requirement for 'application/json' in 'content-type' and 'accept'.
# Middleware decodes JSON data into a python object
api = falcon.API(middleware=[RequireJSON(), JSONTranslator()])
api.add_route("/collect/{uuid}", JSONWaterElfCollector())

