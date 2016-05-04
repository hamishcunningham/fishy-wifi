import os


def _generate_changes_path(file_path, uuid):
    path_split = file_path.split("/")
    uuid_idx = path_split.index(uuid)
    path_split.insert(uuid_idx, "changes")
    final_path = "/".join(path_split)
    return final_path

def write_change_flag(file_path, uuid):
    path = _generate_changes_path(file_path, uuid)
    _create_path(path)
    with open(path, "w") as f:
        pass

def _create_path(filename):
    if not os.path.exists(os.path.dirname(filename)):
        try:
            os.makedirs(os.path.dirname(filename))
        except OSError as exc: # Guard against race condition
            if exc.errno != errno.EEXIST:
                raise

