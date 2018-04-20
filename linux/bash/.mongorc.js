String.prototype.padRight = function(total, pad) {
    return (this+Array(total).join(pad || ' ')).slice(0, total);
}

function showList(list) {
    let cnt = 0;
    let line = '';
    for (let i in list) {
        const item = list[i];
        if (cnt++ < 4) {
            line += '    ' + item.padRight(30);
        } else {
            print(line);
            line = '';
            cnt = 0;
        }
    }
    line && print(line);
}

function id(s) {
    if (/[a-z0-9]{24}/.test(s)) {
        return ObjectId(s);
    }
    return s;
}

Object.defineProperty(this, "ls", {
    get: function() {
        const collections = db.getCollectionNames();
        if (!collections.length) {
            const databases = db.getMongo().getDBs().databases.map(o=>o.name);
            print('show databases:');
            showList(databases);
        } else {
            const dbname = db.getName();
            print(`show collections in ${dbname}:`);
            showList(collections);
        }
    },
});

Object.defineProperty(this, "_", {
    get: function() {
        if (DBQuery.prototype._prettyShell) {
            DBQuery.prototype._prettyShell = false;
        } else {
            DBQuery.prototype._prettyShell = true;
        }
        print('pretty format change to ' + DBQuery.prototype._prettyShell);
    },
});

Object.defineProperty(this, "_n", {
    set: function(n) {
        DBQuery.shellBatchSize = n;
        print('set DBQuery.shellBatchSize to ');
    },
});

Object.defineProperty(this, "__host", {
    get: function() {
        print(db.getMongo().host);
    },
});

Object.defineProperty(this, "__189", {
    get: function() {
        if (db.getMongo().host === '127.0.0.1:27017') {
            db = new Mongo("192.168.1.189").getDB('pdshop');
            print('change to 192.168.1.189');
        } else {
            db = new Mongo("127.0.0.1").getDB('pdshop');
            print('change to 127.0.0.1');
        }
    },
});

const defaultFind = DBCollection.prototype.find;
DBCollection.prototype.find = function (query, fields, limit, skip, batchSize, options) {
    let obj = {};
    let keys = Object.keys(defaultFind.call(this, {}, {_id: 0, __v: 0}, 1, 0).toArray()[0]||{});
    if (typeof fields === 'string' || fields instanceof RegExp) {
        fields = [ fields ];
    }
    if (fields instanceof Array) {
        fields = fields.map(o=>new RegExp(o));
        keys.filter(o => {
            for (let f of fields) {
                if (f.test(o)) {
                    return true;
                }
            }
        }).forEach(o=>obj[o]=1);
    } else {
        obj = fields;
    }

    if (typeof query === 'string') {
        if (/[a-z0-9]{24}/.test(query)) {
            query = {$or: keys.concat('_id').map(o=>({[o]: ObjectId(query)}))};
        } else {
            query = new RegExp(query);
        }
    } else if (typeof query === 'object' && !(query instanceof RegExp)) {
        Object.keys(query).map(k=>{
            if (/[a-z0-9]{24}/.test(query[k])) {
                query[k] = ObjectId(query[k]);
            }
        });
    }
    if (query instanceof RegExp) {
         query = {$or: keys.map(o=>({[o]: query}))};
    }

    return defaultFind.call(this, query, obj, limit, skip, batchSize, options);
};


const defaultUpdate =  DBCollection.prototype.update;
DBCollection.prototype.update = function (query, obj, upsert, multi) {
    if (typeof query === 'string') {
        query = {_id: ObjectId(query)};
    }
    if (!obj['$set'] || !obj['$unset']) {
        const newObj = {};
        Object.keys(obj).forEach((k)=>{
            if (obj[k] !== 'unset') {
                !newObj['$set'] &&( newObj['$set'] = {});
                newObj['$set'][k] = obj[k];
            } else {
                !newObj['$unset'] &&( newObj['$unset'] = {});
                newObj['$unset'][k] = 1;
            }
        });
        obj = newObj;
    }
    return defaultUpdate.call(this, query, obj, upsert, multi);
};

// 登录直接进入pdshop

db = db.getMongo().getDB('pdshop');
