const FILTERS = {
    roadmaps: ['sendDoorList'],
};

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

Object.defineProperty(this, "_h", {
    get: function() {
        print('show self defined command:');
        print('     ls:     show databases and collections');
        print('     id:     wrapper ObjectId only if like 24 bit _id');
        print('     _:      switch pretty shell');
        print('     _n:     set shellBatchSize, e.g: _n=10');
        print('     _host:  show host');
        print('     _189:   switch to 189 server');
        print('     find:   find(id/str/obj, \'[-]xx xx ...\'/{xx:1, ...})');
        print('     update: find(id/obj, {xx:xx, ...}), unset can be { $unset: { xx: 1 } } or { xx: \'$unset\' }');
    },
});

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

Object.defineProperty(this, "_host", {
    get: function() {
        print(db.getMongo().host);
    },
});

Object.defineProperty(this, "_189", {
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
    const name = this.getName();
    let obj = {};
    let keys = Object.keys(defaultFind.call(this, {}, {_id: 0, __v: 0}, 1, 0).toArray()[0]||{}).filter(o=>{
        if (!FILTERS[name] || FILTERS[name].indexOf(o) === -1) {
            return true;
        }
    });

    if (!fields) {
        obj['__v'] = 0;
        if (FILTERS[name]) {
            FILTERS[name].forEach(o=>obj[o] = 0);
        }
    } else if (fields && typeof fields === 'string') {
        fields = fields.split(' ').filter(o=>!!o);
        let match = true;
        if (fields[0][0] === '-') {
            fields[0] = fields[0].slice(1);
            match = false;
        }
        keys.filter(o => {
            for (let f of fields) {
                if (new RegExp(f).test(o)) {
                    return true;
                }
            }
        }).forEach(o=>obj[o]= match ? 1 : 0);
        if (!match) {
            obj['__v'] = 0;
            if (FILTERS[name]) {
                FILTERS[name].forEach(o=>obj[o] = 0);
            }
        }
    } else {
        obj = fields;
    }

    if (typeof query === 'string') {
        if (/[a-z0-9]{24}/.test(query)) {
            query = {$or: keys.concat('_id').map(o=>({[o]: ObjectId(query)}))};
        } else {
            query = {$or: keys.map(o=>({[o]: new RegExp(query)}))};
        }
    } else if (typeof query === 'object') {
        Object.keys(query).forEach(k=>query[k]=id(query[k]));
    }

    return defaultFind.call(this, query, obj, limit, skip, batchSize, options);
};


const defaultUpdate =  DBCollection.prototype.update;
DBCollection.prototype.update = function (query, obj, upsert, multi) {
    if (typeof query === 'string') {
        query = {_id: ObjectId(query)};
    } else if (typeof query === 'object') {
        Object.keys(query).forEach(k=>query[k]=id(query[k]));
    }
    if (!obj['$set'] || !obj['$unset']) {
        const newObj = {};
        Object.keys(obj).forEach((k)=>{
            if (obj[k] !== '$unset') {
                !newObj['$set'] &&( newObj['$set'] = {});
                newObj['$set'][k] = id(obj[k]);
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
