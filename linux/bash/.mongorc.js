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
            return /\$[0-9a-zA-Z_]+/.test(doc) || /\$\$/.test(doc);
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

function replaceSelfValue(self, doc, value, options) {
    if (typeof doc !== 'object') {
        if (typeof doc === 'string') {
            doc = doc.replace(/'?"?\$\$"?'?/, options['$$']);
            if (doc.slice(0, 4) === '_mo.') {
                const result = self.aggregate([
                    { $match: { _id: value._id } },
                    { $project: {__result: eval(doc)}}
                ])._batch[0].__result;
                return result;
            } else {
                const match = doc.match(/\$[0-9a-zA-Z_]+/g)||[];
                for (let m of match) {
                    const name = m.replace('$', '');
                    doc = doc.replace(m, value[name]);
                }
                try {
                    return eval(doc);
                } catch (e) {
                    return doc;
                }
            }
        }
        return doc;
    }
    if (doc instanceof Array) {
        return doc.map(o=>replaceSelfValue(self, o, value, options));
    }
    const keys = Object.keys(doc);
    if (!keys.length) {
        return doc;
    }
    const obj = {};
    for (let key of keys) {
        obj[key] = replaceSelfValue(self, doc[key], value, options);
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
        print('             find(\'3d*5d\'), must have * in query');
        print('     update: update(id/obj, {xx:xx, ...}, 1), 1 is multi');
        print('             update({xx:xx, ...}, 1)');
        print('             update({xx:xx, ...})');
        print('             unset can be { $unset: { xx: 1 } } or { xx: \'null\' }');
        print('             inc can be { $inc: { xx: 1 } } or { xx: \'$xx+1\' } or { xx: mo.add(\'$xx\', 1) }');
        print('             mo can be { xx: mo.add(\'$a\', \'$b\')');
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
        } else if (/\*/.test(query) && /^[a-z*0-9]+$/.test(query)) {
            const result = self.aggregate([
                { $project: { _id: { $toString: '$_id' } } },
                { $match: { _id: new RegExp(`^${query.replace('*', '.*')}$`) } },
            ])._batch;
            query = { _id: { $in: result.map(o=>new ObjectId(o._id)) } };
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
                if (item === 'null') {
                    !newObj['$unset'] && (newObj['$unset'] = {});
                    newObj['$unset'][k] = 1;
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
        const result = [];
        while (it.hasNext()) {
            const doc = it.next();
            const value = replaceSelfValue(this, newObj, doc, options);
            const ret = defaultUpdate.call(this, {_id: doc._id}, value);
            result.push(ret);
            if (!(options||{}).multi) {
                break;
            }
        }
        return result.reduce((r, n)=>{r.nMatched+=n.nMatched;r.nUpserted+=n.nUpserted;r.nModified+=n.nModified;return r;}, { nMatched : 0, nUpserted : 0, nModified : 0 });
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
        const result = [];
        let ret;
        let i = 0;
        while (it.hasNext()) {
            const item = it.next();
            const obj = cloneDeep(item);
            while (i++ < count) {
                obj._id = new ObjectId();
                ret = this.insert(obj);
                result.push(ret);
                if (update) {
                    ret = this.update({_id: obj._id}, update, { '$$': i });
                    result.push(ret);
                }
            }
        }
        return result.reduce((r, n)=>{r.nInserted+=(n.nInserted||0);r.nModified+=(n.nModified||0);return r;}, { nInserted : 0, nModified : 0 });
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

/*** mo define start ***/
const _mo = {};
const mo = {};
!function (_mo, mo) {
    const id = o => new ObjectId(o);
    const isID = o => /^[a-z0-9]{24}$/.test(o);
    const get = (o, k) => ({ $let: { vars: { o }, in: `$$o.${k}` } });
    const set = o => ({ $literal: o });
    const at = (array, index = 0) => ({ $arrayElemAt: [ array, index ] });
    const if_fn = (criteria, trueValue = true, falseValue = false) => ({ $cond: { if: criteria, then: trueValue, else: falseValue } });
    const filter = (array, itemName, cond) => ({ $filter: { input: array, as: itemName, cond } });
    const find = (array, itemName, cond) => ({ $arrayElemAt: [ { $filter: { input: array, as: itemName, cond } }, 0 ] });
    const findLast = (array, itemName, cond) => ({ $arrayElemAt: [ { $filter: { input: array, as: itemName, cond } }, -1 ] });
    const map = (array, itemName, val) => ({ $map: { input: array, as: itemName, in: val } });
    const pick = (o, keys) => ({ $arrayToObject: { $filter: { input: { $objectToArray: o }, as: 'item_', cond: { $in: [ '$$item_.k', keys ] } } } });
    const let_fn = (vars, val) => (val !== undefined ? { $let: { vars, in: val } } : { $let: { vars: {}, in: vars } });
    const reduce = (array, initialValue, fn) => ({ $reduce: { input: array, initialValue, in: fn('$$value', '$$this') } });
    const bit2 = (n, fn) => ({ $divide: [ fn({ $multiply: [ n, 100 ] }), 100 ] }); // fn maybe _mo.round/_mo.ceil/_mo.floor
    const bitN = (n, fn, rank = 0) => { rank = Math.pow(10, rank); return { $divide: [ fn({ $multiply: [ n, rank ] }), rank ] }; };
    const round = n => ({ $floor: { $add: [ n, 0.5 ] } });
    const ceil = n => ({ $cond: { if: { $lte: [ { $subtract: [ n, { $ceil: { $subtract: [ n, 1 ] } } ] }, 0.000001 ] }, then: { $ceil: { $subtract: [ n, 1 ] } }, else: { $add: [ { $ceil: { $subtract: [ n, 1 ] } }, 1 ] } } });
    const floor = n => ({ $cond: { if: { $lte: [ { $subtract: [ { $floor: { $add: [ n, 1 ] } }, n ] }, 0.000001 ] }, then: { $floor: { $add: [ n, 1 ] } }, else: { $subtract: [ { $floor: { $add: [ n, 1 ] } }, 1 ] } } });
    const include = (array, val) => ({ $ne: [ { $indexOfArray: [ array, val ] }, -1 ] });
    const gsum = (key) => ({ $sum: key });
    const time = (date, fmt = '%Y-%m-%d %H:%M:%S') => {
        return { $dateToString: { format: fmt, date, timezone: '+08:00' } };
    };
    const date = (date, fmt = '%Y-%m-%d') => {
        return { $dateToString: { format: fmt, date, timezone: '+08:00' } };
    };
    // unit = 1 表示米, 默认是1000,表示公里
    const distance = (val, unit = 1000, distanceField = 'distance') => ({
        near: val,
        distanceField,
        spherical: true,
        distanceMultiplier: 6378137 / unit,
    });

    const maps = {
        add: 'add',
        sum: 'sum',
        sub: 'subtract',
        div: 'divide',
        mul: 'multiply',
        gt: 'gt',
        gte: 'gte',
        lt: 'lt',
        lte: 'lte',
        eq: 'eq',
        ne: 'ne',
        max: 'max',
        min: 'min',
        and: 'and',
        or: 'or',
        not: 'not',
        ifNull: 'ifNull',
        in: 'in',
        concat: 'concat',
        slice: 'slice',
        substr: 'substr',
        mergeObjects: 'mergeObjects',
        concatArrays: 'concatArrays',
        range: 'range',
        abs: true,
        trunc: true,
        size: true,
        toString: true,
        toInt: true,
        first: true,
        push: true,
        dayOfMonth: true,
        id,
        isID,
        get,
        set,
        let: let_fn,
        reduce,
        at,
        if: if_fn,
        filter,
        find,
        findLast,
        map,
        pick,
        bit2,
        bitN,
        round,
        ceil,
        floor,
        time,
        date,
        distance,
        include,
        gsum,
    };
    for (let k in maps) {
        const o = maps[k];
        if (typeof o === 'function') {
            _mo[k] = o;
        } else if (o === true) {
            _mo[k] = m => ({ ['$' + k]: m });
        } else {
            _mo[k] = (...m) => ({ ['$' + o]: m });
        }
        const func = '_mo.'+k;
        mo[k] = (...m)=> func + `(${m.map(v => typeof v==='string' && v.slice(0, 4)!=='_mo.' ? `'${v}'` : v)})`;
    }
}(_mo, mo);
/*** mo define end ***/
