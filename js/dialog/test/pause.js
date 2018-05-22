var dialog = require('..');

(async function() {
    await dialog.pause();
    console.log("=========1");
    await dialog.pause();
    console.log("=========2");
     dialog.pauseOff();
})();
