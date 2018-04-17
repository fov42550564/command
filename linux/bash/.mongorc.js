// DBQuery.prototype._prettyShell = true;

Object.defineProperty(this, "ls", {
    get: function() {
        const collections = db.getCollectionNames();
        collections.forEach(function(value) {
            print(value);
        });
    },
});

const defaultFind = DBCollection.prototype.find;
DBCollection.prototype.find = function (query, fields, limit, skip, batchSize, options) {
    if (typeof query === 'string') {
        return defaultFind.call(this, {_id: ObjectId(query)}, fields, limit, skip, batchSize, options);
    }
    return defaultFind.call(this, query, fields, limit, skip, batchSize, options);
};
