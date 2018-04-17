// DBQuery.prototype._prettyShell = true;

Object.defineProperty(this, "ls", {
    get: function() {
        const collections = db.getCollectionNames();
        collections.forEach(function(value) {
            print(value);
        });
    },
});


DBCollection.prototype.findById = function (id, fields, limit, skip, batchSize, options) {
    var cursor = new DBQuery(
        this._mongo,
        this._db,
        this,
        this._fullName,
        this._massageObject({_id: ObjectId(id)}),
        fields,
        limit,
        skip,
        batchSize,
        options || this.getQueryOptions()
    );

    var connObj = this.getMongo();
    var readPrefMode = connObj.getReadPrefMode();
    if (readPrefMode != null) {
        cursor.readPref(readPrefMode, connObj.getReadPrefTagSet());
    }

    return cursor;
};
