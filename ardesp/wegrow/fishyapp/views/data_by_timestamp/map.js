function (doc) {
    if (doc.timestamp) {
        emit(doc.timestamp, null);
    }
}
