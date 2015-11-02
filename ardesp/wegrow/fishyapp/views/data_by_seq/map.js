function (doc) {
    if (doc._id[0] !== '_') {
        emit(doc._local_seq, null);
    }
}
