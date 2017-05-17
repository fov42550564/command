var dialog = require('../dialog');

(async function() {
    var ret = await dialog.inputbox("input a number", 9);
    console.log(ret);
})();
