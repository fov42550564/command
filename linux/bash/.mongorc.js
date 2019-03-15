const REJECTS = {
    __all: ['salt', 'hash'],
    roadmaps: ['sendDoorList'],
};
const STRICT = false;

String.prototype.padRight = function(total, pad) {
    return (this+Array(total).join(pad || ' ')).slice(0, total);
}
Date.prototype.format = function(fmt)   {
    var o = {
        "M+" : this.getMonth()+1,                 //月份
        "D+" : this.getDate(),                    //日
        "H+" : this.getHours(),                   //小时
        "m+" : this.getMinutes(),                 //分
        "s+" : this.getSeconds(),                 //秒
        "q+" : Math.floor((this.getMonth()+3)/3), //季度
        "S"  : this.getMilliseconds()             //毫秒
    };
    if(/(Y+)/.test(fmt))
    fmt=fmt.replace(RegExp.$1, (this.getFullYear()+"").substr(4 - RegExp.$1.length));
    for(var k in o)
    if(new RegExp("("+ k +")").test(fmt))
    fmt = fmt.replace(RegExp.$1, (RegExp.$1.length==1) ? (o[k]) : (("00"+ o[k]).substr((""+ o[k]).length)));
    return fmt;
}

function forbidden(){
    print("非法操作");
};

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
    if (/^[a-z0-9]{24}$/.test(s)) {
        return ObjectId(s);
    }
    if (s && typeof s === 'object') {
        if (s instanceof Array) {
            return s.map(o => id(o));
        } else {
            if (Object.keys(s).length) {
                const ret = {};
                Object.keys(s).forEach(k => {ret[k] = id(s[k])});
                return ret;
            }
        }
    }
    return s;
}

function cloneDeep(doc) {
    if (typeof doc !== 'object') {
        return doc;
    }
    if (doc instanceof Array) {
        return doc.map(o=>cloneDeep(o));
    }
    const keys = Object.keys(doc);
    if (!keys.length) {
        return doc;
    }
    const obj = {};
    for (let key of keys) {
        if (key !== '_id' && key !== '__v') {
            obj[key] = cloneDeep(doc[key]);
        }
    }
    return obj;
}

function checkUseSelfValue(doc) {
    if (typeof doc !== 'object') {
        if (typeof doc === 'string') {
            return /_\.[0-9a-zA-Z_]+/.test(doc);
        }
        return false;
    }
    if (doc instanceof Array) {
        for (let item of doc) {
            if (checkUseSelfValue(item)) {
                return true;
            }
        }
    }
    const keys = Object.keys(doc);
    if (!keys.length) {
        return false;
    }
    for (let key of keys) {
        if (checkUseSelfValue(doc[key])) {
            return true;
        }
    }
    return false;
}

function replaceSelfValue(doc, value) {
    if (typeof doc !== 'object') {
        if (typeof doc === 'string') {
            const match = doc.match(/_\.[0-9a-zA-Z_]+/g);
            for (let m of match) {
                const name = m.replace('_.', '');
                doc = doc.replace(m, value[name]);
            }
            try {
                return eval(doc);
            } catch (e) {
                return doc;
            }
        }
        return doc;
    }
    if (doc instanceof Array) {
        return doc.map(o=>replaceSelfValue(o, value));
    }
    const keys = Object.keys(doc);
    if (!keys.length) {
        return doc;
    }
    const obj = {};
    for (let key of keys) {
        obj[key] = replaceSelfValue(doc[key], value);
    }
    return obj;
}

Object.defineProperty(this, "_h", {
    get: function() {
        print('show self defined command:');
        print('     l:     show databases and collections');
        print('     id:     wrapper ObjectId only if like 24 bit _id');
        print('     _:      switch pretty shell');
        print('     _n:     get/set shellBatchSize, e.g: _n=10');
        print('     _host:  show host');
        print('     _189:   switch 189 and localhost server');
        print('     find:   find(id/str/obj, \'[-]xx xx ...\'/{xx:1, ...})');
        print('     update: update(id/obj, {xx:xx, ...}, 1), 1 is multi');
        print('             update({xx:xx, ...}, 1)');
        print('             update({xx:xx, ...})');
        print('             unset can be { $unset: { xx: 1 } } or { xx: \'$unset\' }');
        print('             inc can be { $inc: { xx: 1 } } or { xx: \'+1\' }');
        print('     copy:   copy(id/str/obj, {xx:xx, ...}, count)');
        print('             {xx:xx, ...} is need modify, $$ will be replace to index, index is start at 1');
        print('             eg: { a: \'-$$*3\' }');
        print('             count is times');
    },
});

Object.defineProperty(this, "l", {
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
    get: function() {
        print('DBQuery.shellBatchSize = ' + DBQuery.shellBatchSize);
    },
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
DBCollection.prototype._find = defaultFind;
function parseQuery(self, query, _fields) {
    const name = self.getName();
    const fields = {};
    const keys = Object.keys(defaultFind.call(self, {}, {_id: 0, __v: 0}, 100, 0).toArray().reduce((r, n)=>Object.assign(r, n), {})).filter(o=>!((REJECTS.__all.indexOf(o) !== -1) || (REJECTS[name] && REJECTS[name].indexOf(o) !== -1)));

    if (!_fields) {
        fields['__v'] = 0;
        REJECTS.__all.forEach(o=>fields[o] = 0);
        if (REJECTS[name]) {
            REJECTS[name].forEach(o=>fields[o] = 0);
        }
    } else if (typeof _fields === 'string') {
        _fields = _fields.split(' ').filter(o=>!!o);
        let match = true;
        if (_fields[0][0] === '-') {
            _fields[0] = _fields[0].slice(1);
            match = false;
        }
        keys.filter(o => {
            for (let f of _fields) {
                if (new RegExp(f).test(o)) {
                    return true;
                }
            }
        }).forEach(o=>fields[o]= match ? 1 : 0);
        if (!match) {
            fields['__v'] = 0;
            REJECTS.__all.forEach(o=>fields[o] = 0);
            if (REJECTS[name]) {
                REJECTS[name].forEach(o=>fields[o] = 0);
            }
        }
    } else {
        fields = _fields;
    }
    if (typeof query === 'string') {
        if (/[a-z0-9]{24}/.test(query)) {
            query = {$or: keys.concat('_id').map(o=>({[o]: ObjectId(query)}))};
        } else {
            query = {$or: keys.map(o=>({[o]: new RegExp(query)}))};
        }
    } else if (typeof query === 'fieldsect') {
        Object.keys(query).forEach(k=>query[k]=id(query[k]));
    }
    return {query, fields};
};
DBCollection.prototype.find = function (query, fields, limit, skip, batchSize, options) {
    const params = parseQuery(this, query, fields);
    const it = defaultFind.call(this, params.query, params.fields, limit, skip, batchSize, options);
    const show = DBQuery.prototype._prettyShell ? printjson : printjsononeline;
    while (it.hasNext()) {
        const item = it.next();
        const obj = {};
        const keys = Object.keys(item);
        for (let key of keys) {
            if (item[key] instanceof Date) {
                obj[key] = item[key].format('YYYY-MM-DD HH:mm:ss');
            } else {
                obj[key] = item[key];
            }
        }
        show(obj);
    }
};

if (!STRICT) {
    const defaultUpdate = DBCollection.prototype.update;
    DBCollection.prototype._update = defaultUpdate;
    DBCollection.prototype.update = function (query, obj, options) {
        if (typeof obj !== 'object') {
            options = obj;
            obj = query;
            query = {};
        }
        const newObj = {};
        Object.keys(obj).forEach((k)=>{
            const item = obj[k];
            if (k[0] !== '$') {
                if (item === '$unset') {
                    !newObj['$unset'] && (newObj['$unset'] = {});
                    newObj['$unset'][k] = 1;
                } else if (/^(-|\+)\d+$/.test(item)) { // +n | -n
                    !newObj['$inc'] && (newObj['$inc'] = {});
                    newObj['$inc'][k] = +item;
                } else {
                    !newObj['$set'] &&( newObj['$set'] = {});
                    newObj['$set'][k] = id(item);
                }
            } else {
                newObj[k] = item;
            }
        });
        if (options === 1){
            options = { multi: true };
        }
        const params = parseQuery(this, query);
        if (!checkUseSelfValue(newObj)) {
            return defaultUpdate.call(this, params.query, newObj, options);
        }
        const it = defaultFind.call(this, params.query);
        while (it.hasNext()) {
            const doc = it.next();
            const value = replaceSelfValue(newObj, doc);
            this.update({_id: doc._id}, value);
            if (!(options||{}).multi) {
                break;
            }
        }
    };

    const defaultRemove = DBCollection.prototype.remove;
    DBCollection.prototype._remove = defaultRemove;
    DBCollection.prototype.remove = function (query, justOne) {
        const params = parseQuery(this, query);
        return defaultRemove.call(this, params.query, justOne);
    };

    DBCollection.prototype.copy = function (query, update, count = 1) {
        if (typeof update === 'number') {
            count = update;
            update = undefined;
        }
        const params = parseQuery(this, query);
        const it = defaultFind.call(this, params.query);
        let i = 0;
        while (it.hasNext()) {
            const item = it.next();
            const obj = cloneDeep(item);
            while (i++ < count) {
                obj._id = new ObjectId();
                this.insert(obj);
                if (update) {
                    const newUpdate = {};
                    Object.keys(update).forEach(k=>{
                        if (/\$\$/.test(update[k])) {
                            const value = eval(update[k].replace(/\$\$/, i));
                            newUpdate[k] = value > 0 ? '+'+value : value;
                        } else {
                            newUpdate[k] = update[k];
                        }
                    });
                    this.update({_id: obj._id}, newUpdate);
                }
            }
        }
    };
} else {
    db.dropDatabase = forbidden;
    DB.prototype.dropDatabase = forbidden;
    DBCollection.prototype.drop = forbidden;
    DBCollection.prototype.dropIndex = forbidden;
    DBCollection.prototype.dropIndexes = forbidden;
    DBCollection.prototype.update = forbidden;
    DBCollection.prototype.remove = forbidden;
    DBCollection.prototype.findOneAndReplace = forbidden;
    DBCollection.prototype.findOneAndUpdate = forbidden;
    DBCollection.prototype.findAndModify = forbidden;
    DBCollection.prototype.deleteMany = forbidden;
    DBCollection.prototype.deleteOne = forbidden;
}

// 登录直接进入pdshop
db = db.getMongo().getDB('pdshop');
