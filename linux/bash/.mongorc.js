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
        query = {_id: ObjectId(query)};
    }
    return defaultFind.call(this, query, fields, limit, skip, batchSize, options);
};


const defaultUpdate =  DBCollection.prototype.update;
DBCollection.prototype.update = function (query, obj, upsert, multi) {
    if (typeof query === 'string') {
        query = {_id: ObjectId(query)};
    }
    if (!obj['$set'] || !obj['$unset']) {
        var needUnset = true;
        Object.keys(obj).forEach((k)=>{
            if (obj[k]!==null) {
                needUnset = false;
            }
        });

        if (needUnset) {
            Object.keys(obj).forEach((k)=>{
                obj[k] = 1;
            });
            obj = {$unset: obj};
        } else {
            obj = {$set: obj};
        }
    }
    return defaultUpdate.call(this, query, obj, upsert, multi);
};
